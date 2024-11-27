#include <ESP8266WiFi.h>
#include <WebSocketsClient.h>

const char* ssid = "Minh Nhi";
const char* password = "tumotdentam";
const char* ip_host = "192.168.1.78";
const uint16_t port = 3000;   
WebSocketsClient webSocket;

void webSocketEvent(WStype_t type, uint8_t * payload, size_t length) {
    switch (type) {
        case WStype_DISCONNECTED:
            Serial.println("Disconnected");
            break;
        case WStype_CONNECTED:
            Serial.println("WebSocket connected");
            webSocket.sendTXT("Hello from ESP8266");
            break;
        case WStype_TEXT:
            Serial.printf("Received: %s\n", payload);
            break;
        case WStype_ERROR:
            Serial.printf("WebSocket Error: %s\n", payload);
            break;
    }
}

void setup() {
    Serial.begin(115200);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("WiFi connected");
    Serial.print("ESP8266 IP Address: ");
    Serial.println(WiFi.localIP()); 

    webSocket.begin(ip_host, port);
    webSocket.onEvent(webSocketEvent);
}

void loop() {
    webSocket.loop();
}
