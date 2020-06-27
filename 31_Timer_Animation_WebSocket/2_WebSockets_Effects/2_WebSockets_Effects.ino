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
#define NUM_LEDS   64 
#define DATA_PIN   27   

CRGB leds[NUM_LEDS];

uint8_t gHueDots = 0; // rotating "base color" used by many of the patterns
#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

CRGBPalette16 currentPalette;
CRGBPalette16 targetPalette;
TBlendType    currentBlending = LINEARBLEND;


const char* ssid     = "TP-LINK-MCU";     
const char* password = "15253545"; 

AsyncWebServer server(80);
WebSocketsServer webSocket = WebSocketsServer(81);

byte animationSequence = 0;

#define EEPROM_SIZE 64
bool eepromControl = true;

byte Second = 0;
byte Minute = 0;
byte lastSecond;
bool tStart = false;

static uint8_t hue = 0;

hw_timer_t * timer1 = NULL;
portMUX_TYPE timerMux1 = portMUX_INITIALIZER_UNLOCKED;
void IRAM_ATTR onTimer1(){

 if(tStart == true){ 
    Second++;
    if(Second >= 60){
       Second = 0;
       Minute++;
       if(Minute >= 60){
          Minute = 0;
       }
    }

   if(Second != lastSecond){
      Serial.println("Time -- " + String(Minute) +":"+ String(Second));  
   }
   lastSecond = Second;    
 }

} // timer son

void setup(){
 Serial.begin(115200);
 FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
 FastLED.setBrightness(255);
 FastLED.setMaxPowerInVoltsAndMilliamps(5,500);    // FastLED Power management set at 5V, 500mA.  
 FastLED.clear();

 timer1 = timerBegin(1, 80, true);  // timer 1, MWDT clock period = 12.5 ns * TIMGn_Tx_WDT_CLK_PRESCALE -> 12.5 ns * 80 -> 1000 ns = 1 us, countUp
 timerAttachInterrupt(timer1, &onTimer1, true); // edge (not level) triggered 
 timerAlarmWrite(timer1, 1000000, true); // 1000000 * 1 us = 1 second, autoreload true 
 timerAlarmEnable(timer1); // enable // at least enable the timer alarms 
 
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

 break;
 case 5:

 break;
 case 6:

 break;
 case 7:

 break;
 case 8:

 break;
 case 9:

 break;
 case 10:

 break;
 case 11:

 break;
 case 12:

 break;
 case 13:

 break;
 case 14:

 break;
 case 15:

 break;
 case 16:

 break;
 case 17:

 break;
 case 18:

 break;
 case 19:

 break;
 case 20:
 
 break; 
 case 21: //  APPLAUSE
  Applause();
 break;
 case 22: //  BEADS
  Beads();
 break;
 case 23: //  BEATWAVE
  Beatwave();
 break;
 }
 
} // ***********  loop ******************* EFF1

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

       break;
       case 5:

       break;
       case 6:

       break;
       case 7:

       break;
       case 8:

       break;
       case 9:

       break;
       case 10:

       break;
       case 11:

       break;
       case 12:

       break;
       case 13:

       break;
       case 14:

       break;
       case 15:

       break;
       case 16:

       break;
       case 17:

       break;
       case 18:

       break;
       case 19:

       break;
       case 20:

       break;
       case 21:
        responseBroadcast("APPLAUSE");  // 21
       break;
       case 22:
        responseBroadcast("BEADS");    // 
       break;
       case 23:
        responseBroadcast("BEATWAVE");  //
       break;	   
       }
  
    }
    break;
   
    case WStype_TEXT:                    // Handle text messages from client   // gelen data

      //Serial.printf("..[%u] Received text: %s\n", client_num, payload);     // Print out raw message
      
      if(strcmp((char *)payload, "CLOSE") == 0 ){
         tStart = false;   //       ekliyoruz
         Second=0; Minute=0;
         animationSequence = 0;
         EEPROM.write(0,animationSequence); EEPROM.commit();
         responseBroadcast("CLOSE");
      }
      
      if(strcmp((char *)payload, "RED") == 0 ){
         Second=0; Minute=0;
         tStart = false;          
         animationSequence = 1;
         responseBroadcast("RED");
      }
      
      if(strcmp((char *)payload, "GREEN") == 0 ){
         Second=0; Minute=0;
         tStart = false;          
         animationSequence = 2;
         responseBroadcast("GREEN");
      }
      
      if(strcmp((char *)payload, "BLUE") == 0 ){
         Second=0; Minute=0;
         tStart = false;          
         animationSequence = 3;
         responseBroadcast("BLUE");
      }
      /*
       other colors codes 
      */
      
      if(strcmp((char *)payload, "APPLAUSE") == 0 ){ //21
         Colors(CRGB::Black);
         Second=0; Minute=0;
         tStart = false;         
         animationSequence = 21;
         EEPROM.write(0,animationSequence);  EEPROM.commit();          
         responseBroadcast("APPLAUSE");
      }
      
      if(strcmp((char *)payload, "BEADS") == 0 ){
         Colors(CRGB::Black);
         Second=0; Minute=0;
         tStart = false;       
         animationSequence = 22;
         EEPROM.write(0,animationSequence);  EEPROM.commit();        
         responseBroadcast("BEADS");
      }
      
      if(strcmp((char *)payload, "BEATWAVE") == 0 ){
         Colors(CRGB::Black);
         Second=0; Minute=0;
         tStart = false;         
         animationSequence = 23;
         EEPROM.write(0,animationSequence);  EEPROM.commit();          
         responseBroadcast("BEATWAVE");
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

void Applause(){   //originally by Mark K.
//Number of loops is increased based on number of pixels.
//Fade ammount drops as pixels increase.
static uint8_t appAmmount = (NUM_LEDS/32);
  for (uint8_t i=0; i<appAmmount; i++){
    static uint16_t lastPixel = 0;
    fadeToBlackBy( leds, NUM_LEDS, 32/appAmmount );  //was 32
    leds[lastPixel] = CHSV(random8(HUE_BLUE,HUE_PURPLE),255,255);
    lastPixel = random16(NUM_LEDS);
    leds[lastPixel] = CRGB::White;
    FastLED.delay(5);
  }
  FastLED.show();  
  FastLED.delay(50); 
  EVERY_N_MILLISECONDS( 80 ) { gHueDots++; } // slowly cycle the "base color" through the rainbow
}

void Beads(){
  const uint16_t spewSpeed = 100;  // rate of advance
  static boolean spewing = 0;      // pixels are On(1) or Off(0)
  static uint8_t count = 1;        // how many to light (or not light)
  static uint8_t temp = count;
  static uint8_t hue = random8();
  EVERY_N_MILLISECONDS(spewSpeed){
    if (count == 0) {
      spewing = !spewing;
      if (spewing == 1) { count = random8(2,5); }  // random number for On pixels
      else { count = random8(1,8); }  // random number for Off pixels
      temp = count;
      //gHueDots = gHueDots - 30;
      hue = random8();
    }
    for (uint8_t i = NUM_LEDS-1; i > 0; i--) {
      leds[i] = leds[i-1];  // shift data down the line by one pixel
    }
    if (spewing == 1) {  // new pixels are On
      if (temp == count) {
        leds[0] = CHSV(hue-5, 215, 255);  // for first dot
        //leds[0] = CHSV(gHueDots-5, 215, 255);  // for first dot
      } else {
        leds[0] = CHSV(hue, 255, 255/(1+((temp-count)*2)) );  // for following dots
        //leds[0] = CHSV(gHueDots, 255, 255/(1+((temp-count)*2)) );  // for following dots
      }
    } else {  // new pixels are Off
      leds[0] = CHSV(0,0,0);  // set pixel 0 to black
    }
    count = count - 1;  // reduce count by one.
  }//end every_n
  FastLED.show();  
  FastLED.delay(8); 
  EVERY_N_MILLISECONDS( 20 ) { gHueDots++; } // slowly cycle the "base color" through the rainbow
}

void Beatwave(){
  currentPalette = RainbowColors_p;
  uint8_t wave1 = beatsin8(9, 0, 255);  // That's the same as beatsin8(9);
  uint8_t wave2 = beatsin8(8, 0, 255);
  uint8_t wave3 = beatsin8(7, 0, 255);
  uint8_t wave4 = beatsin8(6, 0, 255);

  for (int i=0; i<NUM_LEDS; i++) {
    leds[i] = ColorFromPalette( currentPalette, i+wave1+wave2+wave3+wave4, 255, currentBlending); 
  }
  FastLED.show();
}
