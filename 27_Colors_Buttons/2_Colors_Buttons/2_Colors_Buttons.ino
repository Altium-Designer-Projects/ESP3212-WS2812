#include "WiFi.h"
#include "ESPAsyncWebServer.h"
#include "SPIFFS.h"
#include "EEPROM.h"
/*
 Hello 
 I design developer tools for embedded electronic systems. You can buy my projects design files.
 https://www.altiumdesignerprojects.com
*/
#include <WebSocketsServer.h>

#include <FastLED.h>
#define NUM_LEDS    64 
#define DATA_PIN    27   

CRGB leds[NUM_LEDS];

const char* ssid     = "TP-LINK-MCU";     
const char* password = "15253545"; 

AsyncWebServer server(80);
WebSocketsServer webSocket = WebSocketsServer(81);

byte animationSequence = 0; 

#define EEPROM_SIZE 64
bool eepromControl = true;

void setup(){
 Serial.begin(115200);
 FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
 FastLED.setBrightness(255);
 FastLED.setMaxPowerInVoltsAndMilliamps(5,500);    // FastLED Power management set at 5V, 500mA.  
 FastLED.clear();

 if(!EEPROM.begin(EEPROM_SIZE)){
   Serial.println("failed to initialise EEPROM"); 
   delay(1000);
 }

  if(!SPIFFS.begin(true)){   // Initialize SPIFFS
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  Serial.println(WiFi.localIP());  // Print ESP32 Local IP Address

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/index.html", "text/html");
  });
  
  // Route to load style.css file
  server.on("/css/bulma.min.css", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/css/bulma.min.css", "text/css");
  });

  // Route to load style.css file
  server.on("/css/colors.css", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/css/colors.css", "text/css");
  });

  server.on("/js/jquery-3.4.1.min.js", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/js/jquery-3.4.1.min.js", "application/javascript");
  });
 
  server.begin();
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);
}
 
void loop(){
 webSocket.loop();

 if(eepromControl == true){    
    
   if(EEPROM.read(1) == 160){   
      animationSequence = EEPROM.read(0);
   }else{
      for(word i = 0; i < 16; i++){ EEPROM.write(i,'\0'); }   // clear
      EEPROM.write(0,animationSequence);    // default value
   }
   eepromControl = false;
 }
   
 
 switch(animationSequence){
 case 0:
  Colors(CRGB::Black);
 break;
 case 1:
  Colors(CRGB::Red);   
 break;
 case 2:
  Colors(CRGB::Green);
 break;
 case 3:
  Colors(CRGB::Blue);
 break;
 case 4:
  Colors(CRGB::Cyan);
 break;
 case 5:
  Colors(CRGB::Plum);
 break;
 case 6:
  Colors(CRGB::Coral);
 break;
 case 7:
  Colors(CRGB::Purple);
 break;
 case 8:
  Colors(CRGB::Yellow);
 break;
 case 9:
  Colors(CRGB::Sienna);
 break;
 case 10:
  Colors(CRGB::Maroon);
 break;
 case 11:
  Colors(CRGB::Amethyst);
 break;
 case 12:
  Colors(CRGB::BlueViolet);
 break;
 case 13:
  Colors(CRGB::DarkViolet);
 break;
 case 14:
  Colors(CRGB::DarkOrange);
 break;
 case 15:
  Colors(CRGB::DeepSkyBlue);
 break;
 case 16:
  Colors(CRGB::SpringGreen);
 break;
 case 17:
  Colors(CRGB::MediumOrchid);
 break;
 case 18:
  Colors(CRGB::MidnightBlue);
 break;
 case 19:
  Colors(CRGB::PaleVioletRed);
 break;
 case 20:
  Colors(CRGB::MediumVioletRed);
 break; 
 }
 
} // ***********  loop *******************

void responseBroadcast(String strInput){
 String strJson = "{\"responseKey\":\""+strInput+"\"}";
 webSocket.broadcastTXT(strJson.c_str(), strJson.length());
}


void webSocketEvent(uint8_t client_num, WStype_t type, uint8_t * payload, size_t length){

  switch(type){  // Figure out the type of WebSocket event
    
    case WStype_DISCONNECTED:                                  // Client has disconnected
      Serial.printf("..[%u] Disconnected!\n", client_num);     // [0] Disconnected!  sayfayı kapattığımız zaman
    break;

    case WStype_CONNECTED:                   // New client has connected
    {
      IPAddress ip = webSocket.remoteIP(client_num);         // bağlanan ip yi öğrendik
      Serial.printf("..[%u] Connection from ", client_num);  // [0] Connection from 192.168.2.103
      Serial.println(ip.toString());

 
       switch(animationSequence){  // reports
       case 0:
        responseBroadcast("CLOSE");
       break;
       case 1:
        responseBroadcast("RED"); // RED
       break;
       case 2:
        responseBroadcast("GREEN"); // GREEN 
       break;
       case 3:
        responseBroadcast("BLUE"); // BLUE
       break;
       case 4:
        responseBroadcast("CYAN"); // Cyan 
       break;
       case 5:
        responseBroadcast("PLUM"); // Plum
       break;
       case 6:
        responseBroadcast("CORAL"); // Coral  
       break;
       case 7:
        responseBroadcast("PURPLE"); // Purple 
       break;
       case 8:
        responseBroadcast("YELLOW"); // Yellow 
       break;
       case 9:
        responseBroadcast("SIENNA");  // Sienna 
       break;
       case 10:
        responseBroadcast("MAROON");  // Maroon
       break;
       case 11:
        responseBroadcast("AMETHYST"); // Amethyst
       break;
       case 12:
        responseBroadcast("BLUE-VIOLET"); // BlueViolet
       break;
       case 13:
        responseBroadcast("DARK-VIOLET");  
       break;
       case 14:
        responseBroadcast("DARK-ORANGE"); 
       break;
       case 15:
        responseBroadcast("DEEP-SKY-BLUE"); 
       break;
       case 16:
        responseBroadcast("SPRING-GREEN"); 
       break;
       case 17:
        responseBroadcast("MEDIUM-ORCHID");  
       break;
       case 18:
        responseBroadcast("MIDNIGHT-BLUE"); 
       break;
       case 19:
        responseBroadcast("PALE-VIOLET-RED"); 
       break; 
       case 20:
        responseBroadcast("MEDIUM-VIOLET-RED");  
       break;        
       }
  
    }
    break;
   
    case WStype_TEXT:                    // Handle text messages from client   // gelen data

      //Serial.printf("..[%u] Received text: %s\n", client_num, payload);     // Print out raw message
      
      if(strcmp((char *)payload, "CLOSE") == 0 ){
         animationSequence = 0;
         EEPROM.write(0,animationSequence); 
         EEPROM.write(1,160); EEPROM.commit();
         responseBroadcast("CLOSE");
      }
 
      if(strcmp((char *)payload, "RED") == 0 ){
         animationSequence = 1;
         EEPROM.write(0,animationSequence); 
         EEPROM.write(1,160); EEPROM.commit();         
         responseBroadcast("RED");
      }
      
      if(strcmp((char *)payload, "GREEN") == 0 ){
         animationSequence = 2;
         EEPROM.write(0,animationSequence); 
         EEPROM.write(1,160); EEPROM.commit();         
         responseBroadcast("GREEN");
      }
      
      if(strcmp((char *)payload, "BLUE") == 0 ){
         animationSequence = 3;
         EEPROM.write(0,animationSequence); 
         EEPROM.write(1,160); EEPROM.commit();         
         responseBroadcast("BLUE");
      }

      if(strcmp((char *)payload, "CYAN") == 0 ){
         animationSequence = 4;
         EEPROM.write(0,animationSequence); 
         EEPROM.write(1,160); EEPROM.commit();         
         responseBroadcast("CYAN");
      }

      if(strcmp((char *)payload, "PLUM") == 0 ){
         animationSequence = 5;
         EEPROM.write(0,animationSequence); 
         EEPROM.write(1,160); EEPROM.commit();         
         responseBroadcast("PLUM");
      }

      if(strcmp((char *)payload, "CORAL") == 0 ){
         animationSequence = 6;
         EEPROM.write(0,animationSequence); 
         EEPROM.write(1,160); EEPROM.commit();         
         responseBroadcast("CORAL");
      }

      if(strcmp((char *)payload, "PURPLE") == 0 ){
         animationSequence = 7;
         EEPROM.write(0,animationSequence); 
         EEPROM.write(1,160); EEPROM.commit();         
         responseBroadcast("PURPLE");
      }
      
      if(strcmp((char *)payload, "YELLOW") == 0 ){
         animationSequence = 8;
         EEPROM.write(0,animationSequence); 
         EEPROM.write(1,160); EEPROM.commit();         
         responseBroadcast("YELLOW");  
      }

      if(strcmp((char *)payload, "SIENNA") == 0 ){
         animationSequence = 9;
         EEPROM.write(0,animationSequence); 
         EEPROM.write(1,160); EEPROM.commit();         
         responseBroadcast("SIENNA");
      }

      if(strcmp((char *)payload, "MAROON") == 0 ){
         animationSequence = 10;
         EEPROM.write(0,animationSequence); 
         EEPROM.write(1,160); EEPROM.commit();         
         responseBroadcast("MAROON");
      }

      if(strcmp((char *)payload, "AMETHYST") == 0 ){
         animationSequence = 11;
         EEPROM.write(0,animationSequence); 
         EEPROM.write(1,160); EEPROM.commit();         
         responseBroadcast("AMETHYST");
      }
      
      if(strcmp((char *)payload, "BLUE-VIOLET") == 0 ){
         animationSequence = 12;
         EEPROM.write(0,animationSequence); 
         EEPROM.write(1,160); EEPROM.commit();           
         responseBroadcast("BLUE-VIOLET");
      }

      if(strcmp((char *)payload, "DARK-VIOLET") == 0 ){
         animationSequence = 13;
         EEPROM.write(0,animationSequence); 
         EEPROM.write(1,160); EEPROM.commit();           
         responseBroadcast("DARK-VIOLET");
      }

      if(strcmp((char *)payload, "DARK-ORANGE") == 0 ){
         animationSequence = 14;
         EEPROM.write(0,animationSequence); 
         EEPROM.write(1,160); EEPROM.commit();           
         responseBroadcast("DARK-ORANGE");
      }

      if(strcmp((char *)payload, "DEEP-SKY-BLUE") == 0 ){
         animationSequence = 15;
         EEPROM.write(0,animationSequence); 
         EEPROM.write(1,160); EEPROM.commit();         
         responseBroadcast("DEEP-SKY-BLUE");
      }
      
      if(strcmp((char *)payload, "SPRING-GREEN") == 0 ){
         animationSequence = 16;
         EEPROM.write(0,animationSequence); 
         EEPROM.write(1,160); EEPROM.commit();         
         responseBroadcast("SPRING-GREEN");
      }

      if(strcmp((char *)payload, "MEDIUM-ORCHID") == 0 ){
         animationSequence = 17;
         EEPROM.write(0,animationSequence); 
         EEPROM.write(1,160); EEPROM.commit();         
         responseBroadcast("MEDIUM-ORCHID");
      }

      if(strcmp((char *)payload, "MIDNIGHT-BLUE") == 0 ){
         animationSequence = 18;
         EEPROM.write(0,animationSequence); 
         EEPROM.write(1,160); EEPROM.commit();         
         responseBroadcast("MIDNIGHT-BLUE");
      }

      
      if(strcmp((char *)payload, "PALE-VIOLET-RED") == 0 ){
         animationSequence = 19;
         EEPROM.write(0,animationSequence); 
         EEPROM.write(1,160); EEPROM.commit();         
         responseBroadcast("PALE-VIOLET-RED");
      }

      if(strcmp((char *)payload, "MEDIUM-VIOLET-RED") == 0 ){
         animationSequence = 20;
         EEPROM.write(0,animationSequence); 
         EEPROM.write(1,160); EEPROM.commit();         
         responseBroadcast("MEDIUM-VIOLET-RED");
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

//   ****************************** FUNCTIONS **********************

void Colors(CRGB predefinedColors){
 fill_solid( leds, NUM_LEDS, predefinedColors);
 FastLED.show();
}

void ColorsPixel(int Pixel,CRGB predefinedColors){
 leds[Pixel] = predefinedColors;
 FastLED.show();
}






  
