#include "WiFi.h"
#include "ESPAsyncWebServer.h"
#include "index.h"
#include <WebSocketsServer.h>
/*
 Hello 
 I design developer tools for embedded electronic systems. You can buy my projects design files.
 https://www.altiumdesignerprojects.com
*/
AsyncWebServer server(80);
WebSocketsServer webSocket = WebSocketsServer(81);

const char* ssid = "TP-LINK-MCU";
const char* password = "15253545";

char payloadBuffer[64]; 

word number;

void setup(){
 Serial.begin(115200);
 
 WiFi.begin(ssid,password);

  while(WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(500);
  }
  Serial.println("");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", indexPage);
  });

  server.begin();
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);
}

void loop(){
  webSocket.loop();
}

void webSocketEvent(uint8_t client_num, WStype_t type, uint8_t * payload, size_t length){

  switch(type){  // Figure out the type of WebSocket event
    
    case WStype_DISCONNECTED:                // Client has disconnected
      Serial.printf("[%u] Disconnected!\n", client_num);     // [0] Disconnected!  sayfayı kapattığımız zaman
    break;

    case WStype_CONNECTED:                   // New client has connected
    {
     IPAddress ip = webSocket.remoteIP(client_num);          // bağlanan ip yi öğrendik
     Serial.printf("[%u] Connection from ", client_num);     // [0] Connection from 192.168.2.103
     //Serial.printf("[%u] Connected from %d.%d.%d.%d url: %s\n", client_num, ip[0], ip[1], ip[2], ip[3], payload);
     Serial.println(ip.toString());
    }
    break;
   
    case WStype_TEXT:                    // Handle text messages from client   // gelen data

      Serial.printf("[%u] Received text: %s\n", client_num, payload);   // Print out raw message

      number = (word) strtol((const char *) &payload[1], NULL, 10);
      Serial.println(number); 
      
      if(number == 82540){
         Serial.println("true"); 
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
