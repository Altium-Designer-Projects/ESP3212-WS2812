#include "WiFi.h"
#include "ESPAsyncWebServer.h"
#include "SPIFFS.h"
#include <WebSocketsServer.h>
/*
 Hello 
 I design developer tools for embedded electronic systems. You can buy my projects design files.
 https://www.altiumdesignerprojects.com
*/
const char* ssid = "TP-LINK-MCU";
const char* password = "15253545";

AsyncWebServer server(80);
WebSocketsServer webSocket = WebSocketsServer(81);

char payloadBuffer[64]; // for input data

void setup() {
 Serial.begin(115200);

 if(!SPIFFS.begin(true)){   // Initialize SPIFFS
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
 }
 
  WiFi.begin(ssid,password);
  while(WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(500);
  }
  Serial.println("");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // Print ESP32 Local IP Address
  Serial.println(WiFi.localIP());

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/index.html", "text/html");
  });

  // Route to load style.css file
  server.on("/css/bulma.min.css", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/css/bulma.min.css", "text/css");
  });

  server.begin();
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);
 
}
 
void loop() {
  webSocket.loop();
  
  if(Serial.available() > 0){
    char c[] = {(char)Serial.read()};
    webSocket.broadcastTXT(c, sizeof(c));
  }
  
}

void webSocketEvent(uint8_t client_num, WStype_t type, uint8_t * payload, size_t length){
  
  switch(type){  // Figure out the type of WebSocket event
    
    case WStype_DISCONNECTED:                // Client has disconnected
      Serial.printf("[%u] Disconnected!\n", client_num);
      break;

    case WStype_CONNECTED:                   // New client has connected
      {
        IPAddress ip = webSocket.remoteIP(client_num);
        Serial.printf("[%u] Connection from ", client_num);
        Serial.println(ip.toString());
      }
      break;
   
    case WStype_TEXT:                    // Handle text messages from client
     // Serial.printf("[%u] Received text: %s\n", client_num, payload);   // Print out raw message
      if(payload[0] == '?'){
         for(byte i = 0; i < length; i++){
             payloadBuffer[i] = (char) payload[i+1];
         }
         Serial.println(payloadBuffer); 
      }

      break;
 
    // For everything else: do nothing
    case WStype_BIN:
    case WStype_ERROR:
    case WStype_FRAGMENT_TEXT_START:
    case WStype_FRAGMENT_BIN_START:
    case WStype_FRAGMENT:
    case WStype_FRAGMENT_FIN:
    default:
      break;
  }
}
