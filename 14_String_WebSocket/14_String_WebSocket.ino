#include "WiFi.h"
#include "ESPAsyncWebServer.h"
#include "index.h"
#include <WebSocketsServer.h>
#include <Button.h>
/*
 Hello 
 I design developer tools for embedded electronic systems. You can buy my projects design files.
 https://www.altiumdesignerprojects.com
*/
AsyncWebServer server(80);
WebSocketsServer webSocket = WebSocketsServer(81);

const char* ssid = "TP-LINK-MCU";
const char* password = "15253545";

Button buttonPin0 = Button(16,BUTTON_PULLDOWN); 
Button buttonPin1 = Button(17,BUTTON_PULLDOWN); 
Button buttonPin2 = Button(21,BUTTON_PULLDOWN); 
Button buttonPin3 = Button(22,BUTTON_PULLDOWN); 

char payloadConvert[64]; 
char payloadBuffer[64]; 

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


  if(buttonPin0.uniquePress()){  
     char city[] = "istanbul\r\n"; 
     webSocket.broadcastTXT(city, sizeof(city)-1);
  } 

  if(buttonPin1.uniquePress()){  
     String strTest = "vedat\r\n";
     strTest.toCharArray(payloadConvert, strTest.length()+1);
     webSocket.broadcastTXT(payloadConvert, strTest.length()-1);
  } 

  if(buttonPin2.uniquePress()){  
     String strName = "vedat\r\n";
     webSocket.broadcastTXT(strName.c_str(), strName.length());
  } 
  
 
} // loop

void webSocketEvent(uint8_t client_num, WStype_t type, uint8_t * payload, size_t length){

  switch(type){  // Figure out the type of WebSocket event
    
    case WStype_DISCONNECTED:                // Client has disconnected
      Serial.printf("..[%u] Disconnected!\n", client_num);     // [0] Disconnected!  sayfayı kapattığımız zaman
    break;

    case WStype_CONNECTED:                   // New client has connected
      {
        IPAddress ip = webSocket.remoteIP(client_num);         // bağlanan ip yi öğrendik
        Serial.printf("..[%u] Connection from ", client_num);  // [0] Connection from 192.168.2.103
        Serial.println(ip.toString());
      }
    break;
   
    case WStype_TEXT:                    // Handle text messages from client   // gelen data

      //Serial.printf("..[%u] Received text: %s\n", client_num, payload);     // Print out raw message

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
