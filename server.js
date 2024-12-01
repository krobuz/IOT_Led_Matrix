require('dotenv').config();
const express = require('express');
const http = require('http');
const mysql = require('mysql2');
const bcrypt = require('bcrypt');

const app = express();
const server = http.createServer(app);
const WebSocket = require('ws');;
const wss = new WebSocket.Server({ port: 3001 });

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


wss.on('connection', (ws) => {
    console.log('Client connected');

    ws.on('message', (message) => {
        const decodedMessage = message.toString();
        console.log('Received from client:', decodedMessage);

        wss.clients.forEach((client) => {
            if (client !== ws && client.readyState === WebSocket.OPEN) {
                // console.log(`Forwarding message to client: ${decodedMessage}`);
                client.send(decodedMessage);
            }
        })
        
    });
    ws.on('close', () => {
        console.log('Client disconnected');
    });

    ws.send('Hello from Node.js Server');
});


const PORT = 3000;
server.listen(PORT, () => {
    console.log(`Server is running on http://localhost:${PORT}`);
});
