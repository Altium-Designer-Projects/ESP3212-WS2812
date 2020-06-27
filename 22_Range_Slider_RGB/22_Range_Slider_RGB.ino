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

word redVal,greenVal,blueVal;

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

void loop() {
 webSocket.loop();
}

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length){

    switch(type) {
        case WStype_DISCONNECTED:
          Serial.printf("[%u] Disconnected!\n", num);
          break;
        case WStype_CONNECTED:
        {
          IPAddress ip = webSocket.remoteIP(num);
          Serial.printf("[%u] Connection from ", num);
          Serial.println(ip.toString());
            
          //Serial.printf("[%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);
        }
          break;
        case WStype_TEXT:
          //Serial.printf("[%u] get Text: %s\n", num, payload);

          if(payload[0] == '#'){
             uint32_t rgb = (uint32_t) strtol((const char *) &payload[1], NULL, 16);

             redVal   = (rgb >> 16) & 0xFF;
             greenVal = (rgb >>  8) & 0xFF;
             blueVal  = (rgb >>  0) & 0xFF;
             Serial.println(String(redVal)+"  "+String(greenVal)+"  "+String(blueVal));
        }
        break;
    }
}
