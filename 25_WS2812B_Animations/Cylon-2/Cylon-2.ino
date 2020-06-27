#include "FastLED.h"                              
#define DATA_PIN    8
#define NUM_LEDS    64 

CRGB leds[NUM_LEDS];

void setup(){
 Serial.begin(9600);                               
 FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);

 FastLED.setBrightness(255);
}

void loop(){

/*
 Cylon_Colors(CRGB::Purple);  
 Cylon_Colors(CRGB::Green);  
 Cylon_Colors(CRGB::Gold);  
*/

/*

 Cylon_Colors(CRGB::Cyan);  
 Cylon_Colors(CRGB::DarkViolet);  
 Cylon_Colors(CRGB::SpringGreen);  
*/
/*
 Cylon_Colors(CRGB::DarkCyan);
 Cylon_Colors(CRGB::Yellow); 
 Cylon_Colors(CRGB::HotPink);
*/


 Cylon_Colors(CRGB::HotPink);
 Cylon_Colors(CRGB::Gold); 
 Cylon_Colors(CRGB::DodgerBlue);

  
}

void Cylon_Colors(CRGB predefinedColors){
 for(int i = 0; i < NUM_LEDS; i++){
  leds[i] = predefinedColors;
  FastLED.show();
  for(int i = 0; i < NUM_LEDS; i++){
     leds[i].nscale8(250);
  }
  FastLED.delay(15);
 }
 for(int i = (NUM_LEDS) - 1; i >= 0; i--){
  leds[i] = predefinedColors;
  FastLED.show();
  for(int i = 0; i < NUM_LEDS; i++){
     leds[i].nscale8(250);
  }
  FastLED.delay(15);
 }
}
