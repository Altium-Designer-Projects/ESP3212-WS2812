#include "WiFi.h"
#include "ESPAsyncWebServer.h"
#include "SPIFFS.h"
#include <WebSocketsServer.h>
/*
 Hello 
 I design developer tools for embedded electronic systems. You can buy my projects design files.
 https://www.altiumdesignerprojects.com
*/
#include <FastLED.h>
#define NUM_LEDS   64 
#define DATA_PIN   27
CRGB leds[NUM_LEDS];

const char* ssid = "TP-LINK-MCU";
const char* password = "15253545";

AsyncWebServer server(80);
WebSocketsServer webSocket = WebSocketsServer(81);

char payloadBuffer[64]; // for input data

byte Second = 0;
byte Minute = 0;
byte lastSecond;
bool tStart = false;

char rx_byte;
byte animationSequence = 0;

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

}


void setup() {
 Serial.begin(115200);
 FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
 setAll(0,0,0);

 timer1 = timerBegin(1, 80, true);  // timer 1, MWDT clock period = 12.5 ns * TIMGn_Tx_WDT_CLK_PRESCALE -> 12.5 ns * 80 -> 1000 ns = 1 us, countUp
 timerAttachInterrupt(timer1, &onTimer1, true); // edge (not level) triggered 
 timerAlarmWrite(timer1, 1000000, true); // 1000000 * 1 us = 1 second, autoreload true 
 timerAlarmEnable(timer1); // enable // at least enable the timer alarms 
 
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
 
 if(tStart == true){  
   if(Second >= 0 && Second <= 10){
      Sparkle(50,0,50,50);
   }else if(Second >= 10 && Second <= 20){
      Sparkle(0,0,255,50);
   }else if(Second >= 20 && Second <= 30){
      Sparkle(0,255,0,50);
   }else if(Second >= 30 && Second <= 40){
      Sparkle(255,0,0,50); 
   }else if(Second >= 40 && Second <= 50){
      Sparkle(0,250,155,50); 
   }else if(Second >= 50 && Second <= 60){
      Sparkle(219,112,147,50); 
   }
  
 }

   
  if(Serial.available() > 0){  // response
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

      if(strcmp((char *)payload, "true") == 0 ){
         tStart = true;
      }

      if(strcmp((char *)payload, "false") == 0 ){
         tStart = false;
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
} // **********************


void setPixel(int Pixel, byte red, byte green, byte blue) {
   leds[Pixel].r = red;
   leds[Pixel].g = green;
   leds[Pixel].b = blue;
}

void setAll(byte red, byte green, byte blue) {
  for(int i = 0; i < NUM_LEDS; i++ ) {
    setPixel(i, red, green, blue); 
  }
  FastLED.show();
}

// ************************* Sparkle **************************** // 
void Sparkle(byte red, byte green, byte blue, int SpeedDelay) {
  int Pixel = random(NUM_LEDS);
  setPixel(Pixel,red,green,blue);
  FastLED.show();
  FastLED.delay(SpeedDelay);
  setPixel(Pixel,0,0,0);
}
