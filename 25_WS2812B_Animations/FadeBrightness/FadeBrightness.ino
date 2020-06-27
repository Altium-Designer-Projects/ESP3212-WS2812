#include "FastLED.h"                              
#define DATA_PIN   8
#define NUM_LEDS   1  

CRGB leds[NUM_LEDS];

uint8_t hueFade;

void setup(){
 Serial.begin(115200);                               
 FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
 FastLED.setBrightness(255);
 FastLED.clear();
}

void loop(){

/*
  fadeInOutBrightness(CRGB::Red);
  fadeInOutBrightness(CRGB::DeepSkyBlue);
  fadeInOutBrightness(CRGB::Yellow);
  fadeInOutBrightness(CRGB::Cyan);
*/

/*
  fadeInOutBrightness(CRGB::DeepPink);
  fadeInOutBrightness(CRGB::Teal);
  fadeInOutBrightness(CRGB::HotPink);
  fadeInOutBrightness(CRGB::DarkViolet);
*/

fadeInOutBrightness(CRGB::Lime);
fadeInOutBrightness(CRGB::DarkBlue);
fadeInOutBrightness(CRGB::Gold);
fadeInOutBrightness(CRGB::Red);

}

void Colors(CRGB predefinedColors){
 fill_solid( leds, NUM_LEDS, predefinedColors);
 FastLED.show();
}

void inBrightness(){
 for(int i = 0; i < 255; i++){ 
     FastLED.setBrightness(i);
     FastLED.delay(5);
 }
}

void outBrightness(){
 for(int i = 255; i > 0; i--){   
     FastLED.setBrightness(i);
     FastLED.delay(5);
 }
}

void fadeInBrightness(CRGB predefinedColors){
 Colors(predefinedColors);
 EVERY_N_MILLISECONDS(50){  
   inBrightness();
 }
}

void fadeOutBrightness(CRGB predefinedColors){
 Colors(predefinedColors);
 EVERY_N_MILLISECONDS(50){  
   outBrightness();
 }
}

void fadeInOutBrightness(CRGB predefinedColors){
  fadeInBrightness(predefinedColors);
  fadeOutBrightness(predefinedColors);  
}

void fadeOutInBrightness(CRGB predefinedColors){
  fadeOutBrightness(predefinedColors);    
  fadeInBrightness(predefinedColors);
}
