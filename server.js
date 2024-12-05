require('dotenv').config();
const express = require('express');
const http = require('http');
const mysql = require('mysql2');
const bcrypt = require('bcrypt');   
const jwt = require('jsonwebtoken');

const app = express();
const server = http.createServer(app);
const WebSocket = require('ws');;
const wss = new WebSocket.Server({ port: 3001 });

const SECRET_KEY = process.env.SECRET_KEY;

app.use(express.urlencoded({ extended: true }));
app.use(express.json());

app.set('view engine', 'ejs');
app.use(express.static('public'));


const db = mysql.createConnection({
    host: process.env.DB_HOST,
    user: process.env.DB_USER,
    password: process.env.DB_PASSWORD,
    database: process.env.DB_NAME,
});

db.connect(err => {
    if (err) {
        console.error('Error connecting to MySQL:', err);
        return;
    }
    console.log('Connected to MySQL!');
});

app.get('/', (req, res) => {
    res.render('login', { errorMessage: null });
});

app.post('/login', (req, res) => {
    const { username, password } = req.body;

    // Check for user in the database
    const sql = 'SELECT * FROM users WHERE username = ?';
    db.query(sql, [username], async (err, results) => {
        if (err) {
            console.error(err);
            return res.render('login', { errorMessage: 'Database error' });
        }

        if (results.length === 0) {
            return res.render('login', { errorMessage: 'Invalid username or password' });
        }

        const user = results[0];

        // Verify the password
        const isPasswordValid = await bcrypt.compare(password, user.password);
        if (!isPasswordValid) {
            return res.render('login', { errorMessage: 'Invalid username or password' });
        }

        // Redirect to the dashboard
        res.render('dashboard', { username: user.username });
    });
});


app.post('/signup', async (req, res) => {
    const { username, password, confirmPassword } = req.body;

    // Validate the inputs
    if (!username || !password || !confirmPassword) {
        return res.render('signup', { errorMessage: 'All fields are required.' });
    }

    if (password !== confirmPassword) {
        return res.render('signup', { errorMessage: 'Passwords do not match.' });
    }

    try {
        // Check if the username already exists
        const checkUserSql = 'SELECT * FROM users WHERE username = ?';
        const [results] = await db.promise().query(checkUserSql, [username]);

        if (results.length > 0) {
            return res.render('signup', { errorMessage: 'Username already exists.' });
        }

        // Hash the password
        const hashedPassword = await bcrypt.hash(password, 10);

        // Insert the user into the database
        const insertUserSql = 'INSERT INTO users (username, password) VALUES (?, ?)';
        await db.promise().query(insertUserSql, [username, hashedPassword]);

        // Redirect to login or dashboard
        res.render('login', { successMessage: 'Signup successful! Please log in.' });
    } catch (err) {
        console.error(err);
        res.render('signup', { errorMessage: 'Database error. Please try again later.' });
    }
});

app.get('/api/activity-logs', (req, res) => {
    const sql = 'SELECT timestamp, user_id, action, text_content, scroll_speed, scroll_direction FROM system_changes ORDER BY timestamp DESC LIMIT 10';
    db.query(sql, (err, results) => {
      if (err) {
        res.status(500).send({ error: 'Error fetching activity logs' });
      } else {
        res.json(results); // Send logs as JSON
      }
    });
  });


wss.on('connection', (ws) => {
    console.log('Client connected');

    ws.on('message', (message) => {
        const decodedMessage = message.toString();
        console.log('Received from client:', decodedMessage);

        wss.clients.forEach((client) => {
            if (client !== ws && client.readyState === WebSocket.OPEN) {
                client.send(decodedMessage);
            }
        })

        try {
            // Parse the received JSON
            const parsedData = JSON.parse(message);
    
            // Handle based on the 'type' field
            switch (parsedData.type) {
                case 'display_settings':
                    handleDisplaySettings(parsedData);
                    
                    break;
                default:
                    console.error('Unknown data type:', parsedData.type);
            }
        } catch (err) {
            console.error('Failed to parse message:', message, err);
        }
        
    });
    ws.on('close', () => {
        console.log('Client disconnected');
    });

    ws.send('Hello from Node.js Server');
});



function broadcast() {
    wss.clients.forEach(client => {
        if (client.readyState === WebSocket.OPEN) {
            client.send();
        }
    });
}


function handleDisplaySettings(data) {
    const { message, direction, speed } = data;

    const sql = `
        INSERT INTO system_changes (user_id, text_content, scroll_speed, scroll_direction, action)
        VALUES (?, ?, ?, ?, ?)
    `;
    const userId = 3; // Replace with actual user ID
    const action = 'display_update';

    db.query(sql, [userId, message, speed, direction, action], (err, result) => {
        if (err) {
            console.error('Failed to insert display settings into database:', err);
        } else {        
            // Broadcast the new activity to all connected clients
            broadcast();
        }
    });
}


const PORT = 3000;
server.listen(PORT, () => {
    console.log(`Server is running on http://localhost:${PORT}`);
});
