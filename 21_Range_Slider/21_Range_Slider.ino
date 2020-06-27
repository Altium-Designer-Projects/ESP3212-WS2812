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
        IPAddress ip = webSocket.remoteIP(client_num);       // bağlanan ip yi öğrendik
        Serial.printf("[%u] Connection from ", client_num);  // [0] Connection from 192.168.2.103
        Serial.println(ip.toString());
      }
      break;
   
    case WStype_TEXT:                    // Handle text messages from client

      //Serial.printf("[%u] Received text: %s\n", client_num, payload);   // Print out raw message

     if(payload[0] == '#'){
      uint16_t brightness = (uint16_t) strtol((const char *) &payload[1], NULL, 10);
      Serial.print("Brightness = ");
      Serial.println(brightness);
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

/*

uint16_t brightness = (uint16_t) strtol((const char *) &payload[1], NULL, 10);
//  long result     = strtol(char input_string, char *next_token, int base);

burada ucuncu parametre olan base disinda iki parametre vardir, bu ne icin kullanilir, sunun icin;
eger "12 3443 994" seklinde bosluk karakterinin separator oldugu bir string'iniz varsa, bunlari parse 
edip adim adim long ya da int degiskenlere atmak icin, her parse edilen ifadeden sonra, parse edilmemis
kismin adresinin tutulacagi bir char pointer'a ihtiyac duyulmaktadir, ikinci parametre, eger "123" gibi 
basit bir ifadnin long'a donusturulmesi mevzu bahisse null'lanabilir.

*/
