const express = require('express');
const http = require('http');
const WebSocket = require('ws');
const path = require('path');

const app = express();
const server = http.createServer(app);
const wss = new WebSocket.Server({ server });

const esp8266Ws = new WebSocket('ws://192.168.1.65:8080');

// Serve a basic webpage
app.get('/', (req, res) => {
    res.sendFile(path.join(__dirname, 'index.html'));
});

// Handle WebSocket connections
wss.on('connection', (ws) => {
    console.log('Web client connected'); // Log when the web client connects

    // Handle incoming messages from the web client
    ws.on('message', (message) => {
        console.log('Received from web client:', message);

        // Check if ESP8266 WebSocket is connected
        if (esp8266Ws.readyState === WebSocket.OPEN) {
            // Send the received message to the ESP8266
            esp8266Ws.send(message);
            console.log('Message forwarded to ESP8266');
        } else {
            console.error('ESP8266 WebSocket is not connected');
        }
    });

    // Send a message to the client (optional)
    ws.send('Hello from Node.js Server');
});

esp8266Ws.on('open', () => {
    console.log('Connected to ESP8266');
});

esp8266Ws.on('message', (message) => {
    console.log('Message from ESP8266:', message.toString());
});

// Start the server
const PORT = 8080;
server.listen(PORT, () => {
    console.log(`Server is running on http://localhost:${PORT}`);
});
