const express = require('express');
const http = require('http');
const WebSocket = require('ws');
const path = require('path');

const app = express();
const server = http.createServer(app);
const wss = new WebSocket.Server({ server });


app.get('/', (req, res) => {
    res.sendFile(path.join(__dirname, 'index.html'));
});

wss.on('connection', (ws) => {
    console.log('Client connected');

    ws.on('message', (message) => {
        const decodedMessage = message.toString();
        console.log('Received from client:', decodedMessage);

        wss.clients.forEach((client) => {
            if (client !== ws && client.readyState === WebSocket.OPEN) {
                console.log(`Forwarding message to client: ${decodedMessage}`);
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
