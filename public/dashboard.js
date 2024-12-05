const serverIp = window.location.hostname;
const ws = new WebSocket(`ws://${serverIp}:3001`);

ws.onopen = () => {
    document.getElementById('ws-status').innerText = 'Connected to WebSocket!';
    console.log('Connected to WebSocket server');
};

ws.onmessage = (event) => {
    fetch('/api/activity-logs')
        .then(response => response.json())
        .then(logs => {
            const logsContainer = document.getElementById('activityLogs');

            logsContainer.innerHTML = '';

            logs.forEach(log => {
                const row = document.createElement('tr');

                const timestampCell = document.createElement('td');
                timestampCell.textContent = new Date(log.timestamp).toLocaleString();
                row.appendChild(timestampCell);

                const userCell = document.createElement('td');
                userCell.textContent = `User ${log.user_id}`;
                row.appendChild(userCell);

                const actionCell = document.createElement('td');
                actionCell.textContent = log.action;
                row.appendChild(actionCell);

                const detailsCell = document.createElement('td');
                detailsCell.textContent = log.text_content + ', ' + log.scroll_speed + ', ' + log.scroll_direction;
                row.appendChild(detailsCell);

                logsContainer.appendChild(row);
            });
        })
        .catch(error => {
            console.error('Error fetching activity logs:', error);
        });
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