#include <WiFi.h>
#include <WebServer.h>
#include <WebSocketsServer.h>
/*
 Hello 
 I design developer tools for embedded electronic systems. You can buy my projects design files.
 https://www.altiumdesignerprojects.com
*/
WebServer server;

const char* ssid = "TP-LINK-MCU";
const char* password = "15253545";

WebSocketsServer webSocket = WebSocketsServer(81);

word redVal,greenVal,blueVal;

void setup() {
  Serial.begin(115200);
  Serial.println("Connecting");
  WiFi.begin(ssid, password);
  while ( WiFi.status() != WL_CONNECTED ) {
    delay(500);
    Serial.print(".");
  }

  // Print our IP address
  Serial.println("Connected!");
  Serial.print("My IP address: ");
  Serial.println(WiFi.localIP());

  // Start WebSocket server and assign callback
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);
}

void loop(){
  webSocket.loop();
}

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length){
  switch(type){
    case WStype_DISCONNECTED:            // Client has disconnected
      Serial.printf("[%u] Disconnected!\n", num);
      break;
    case WStype_CONNECTED:    // New client has connected
      {
        IPAddress ip = webSocket.remoteIP(num);
        Serial.printf("[%u] Connection from ", num);
        Serial.println(ip.toString());
      }
      break;

    case WStype_TEXT:          // Echo text message back to client
      
      //Serial.printf("[%u] Text: %s\n", num, payload);

      if(payload[0] == '#'){
         uint32_t rgb = (uint32_t) strtol((const char *) &payload[1], NULL, 16);
 
         redVal   = (rgb >> 16) & 0xFF;
         greenVal = (rgb >>  8) & 0xFF;
         blueVal  = (rgb >>  0) & 0xFF;
         Serial.println(String(redVal)+"  "+String(greenVal)+"  "+String(blueVal));
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
