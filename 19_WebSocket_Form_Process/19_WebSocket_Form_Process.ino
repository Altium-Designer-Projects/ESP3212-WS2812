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
 
String payloadString = "";

bool bufferComplete = false;   // whether the string is complete

String strName = "";
String strLastname = "";
String strCountry = "";

word firstChar  = 0;
word secondChar = 0;

void setup(){
 Serial.begin(115200);
 
 WiFi.begin(ssid,password);

  while(WiFi.status()!=WL_CONNECTED)
  {
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

 if(bufferComplete == true){ 

    //payloadString = String(payloadBuffer);   
    //Serial.println(payloadString);

    if(payloadString.indexOf("firstname") > -1){   

       firstChar  = payloadString.indexOf("firstname&");
       secondChar = payloadString.indexOf("lastname&");
       strName   = payloadString.substring(firstChar + 10, secondChar);
       Serial.println("strName ... " + strName);
    }

    if(payloadString.indexOf("lastname") > -1){ 

       firstChar  = payloadString.indexOf("lastname&");
       secondChar = payloadString.indexOf("country&");
       strLastname   = payloadString.substring(firstChar + 9, secondChar);
       Serial.println("strLastname ... " + strLastname);
    }

    if(payloadString.indexOf("country") > -1){ 

       firstChar  = payloadString.indexOf("country&");
       secondChar = payloadString.indexOf("#");
       strCountry = payloadString.substring(firstChar + 8, secondChar);
       Serial.println("strCountry ... " + strCountry);
    }
    
    payloadString = "";       // clear the string: 
    bufferComplete = false;
 }

 if(strName == "vedat"){  // processing
    Serial.println("HELLO VEDAT");

    strName = ""; // clear
 }

 
}// loop

void webSocketEvent(uint8_t client_num, WStype_t type, uint8_t * payload, size_t length){

  switch(type){  // Figure out the type of WebSocket event
    
    case WStype_DISCONNECTED:                // Client has disconnected
      Serial.printf("--[%u] Disconnected!\n", client_num);     // [0] Disconnected!  sayfayı kapattığımız zaman
      break;

    case WStype_CONNECTED:                   // New client has connected
      {
        IPAddress ip = webSocket.remoteIP(client_num);       // bağlanan ip yi öğrendik
        Serial.printf("--[%u] Connection from ", client_num);  // [0] Connection from 192.168.2.103
        Serial.println(ip.toString());
      }
      break;
   
    case WStype_TEXT:                    // Handle text messages from client

     // Serial.printf("[%u] Received text: %s\n", client_num, payload);   // Print out raw message

    
      if(payload[0] == '?'){
         
         payloadString = (char *)payload;

         bufferComplete = true;
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
