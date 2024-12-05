#include <ESP8266WiFi.h>
#include <WebSocketsClient.h>
#include <SoftwareSerial.h>
#include <ArduinoJson.h>


#define RX 4 //D1
#define TX 5 //D2

// const char* ssid = "HALAMDEPZAIDANGKEPHAI";
// const char* password = "halam232003";
// const char* ssid = "iPhone11";
// const char* password = "99999999";
const char* ssid = "Minh Nhi";
const char* password = "tumotdentam";
const char* server_host = "192.168.1.140";
const uint16_t port = 3001;   
WebSocketsClient webSocket;

SoftwareSerial mySerial(RX, TX);


void sendIdentity() {
    StaticJsonDocument<100> doc;

    doc["type"] = "esp_device";
    doc["message"] = "hello from esp";

    String jsonString;
    serializeJson(doc, jsonString);

    webSocket.sendTXT(jsonString);
}

void webSocketEvent(WStype_t type, uint8_t * payload, size_t length) {
    switch (type) {
        case WStype_DISCONNECTED:
            Serial.println("Disconnected");
            break;
        case WStype_CONNECTED:
            Serial.println("WebSocket connected");
            sendIdentity();
            break;
        case WStype_TEXT: {

            Serial.printf("Received from WebSocket and Forward to Arduino: %s\n", payload); // Log the raw payload

            StaticJsonDocument<200> doc;
            DeserializationError error = deserializeJson(doc, payload);
            if (!error) {
                const char* commandType = doc["type"];
                const char* command = doc["command"];
                const char* message = doc["message"];
                const char* direction = doc["direction"];
                int speed = doc["speed"];

                if (strcmp(commandType, "command") == 0 && strcmp(command, "update_display") == 0) {
                    Serial.printf("Command received: %s\n", message);
                    Serial.printf("Direction: %s, Speed: %d\n", direction, speed);

                    // Forward message to Arduino or handle internally
                    mySerial.printf("Message: %s, Direction: %s, Speed: %d\n", message, direction, speed);
                }
              } else {
                  Serial.println("Failed to parse command JSON");
              }
            // mySerial.printf((char*)payload); // Forward message to Arduino via hardware serial
            break;
        }
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
