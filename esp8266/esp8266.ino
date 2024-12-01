#include <ESP8266WiFi.h>
#include <WebSocketsClient.h>
#include <SoftwareSerial.h>


#define RX 4 //D1
#define TX 5 //D2

// const char* ssid = "HALAMDEPZAIDANGKEPHAI";
// const char* password = "halam232003";
const char* ssid = "iPhone11";
const char* password = "99999999";
// const char* ssid = "Minh Nhi";
// const char* password = "tumotdentam";
const char* server_host = "172.20.10.2";
const uint16_t port = 3000;   
WebSocketsClient webSocket;

SoftwareSerial mySerial(RX, TX);

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
            Serial.printf("Received from WebSocket and Forward to Arduino: %s\n", payload); // Log the raw payload
            mySerial.printf((char*)payload); // Forward message to Arduino via hardware serial
            break;
        case WStype_ERROR: 
            Serial.printf("WebSocket Error: %s\n", payload);
            break;
    }
}

void setup() {
    Serial.begin(115200);   // For ESP8266 debugging  
    mySerial.begin(9600);  

    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("WiFi connected");
    Serial.print("ESP8266 IP Address: ");
    Serial.println(WiFi.localIP()); 

    webSocket.begin(server_host, port);
    webSocket.onEvent(webSocketEvent);
}

void loop() {
    webSocket.loop();
}
