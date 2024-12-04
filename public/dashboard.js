const serverIp = window.location.hostname;
const ws = new WebSocket(`ws://${serverIp}:3001`);

ws.onopen = () => {
    document.getElementById('ws-status').innerText = 'Connected to WebSocket!';
    console.log('Connected to WebSocket server');
};

ws.onmessage = (event) => {
    console.log('Message from server:', event.data);
};

ws.onerror = (error) => {
    document.getElementById('ws-status').innerText = 'WebSocket Error!';
    console.error('WebSocket error:', error);
};

ws.onclose = () => {
    document.getElementById('ws-status').innerText = 'Disconnected from WebSocket';
    console.log('Client disconnected');
};

function sendMessage() {
    const message = document.getElementById('messageBox').value;
    const direction = document.getElementById('scrollDirection').value;
    const speed = document.getElementById('scrollSpeed').value;

    const payload = JSON.stringify({
        type: "display_settings",
        message: message,
        direction: direction,
        speed: parseInt(speed, 10)
    });

    if (ws.readyState === WebSocket.OPEN) {
        ws.send(payload);
        console.log('Sent:', payload);
    } else {
        console.error('WebSocket is not open');
    }
}

document.getElementById('sendButton').addEventListener('click', sendMessage);