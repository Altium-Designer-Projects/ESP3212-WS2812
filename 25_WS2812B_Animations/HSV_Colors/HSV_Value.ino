#include "FastLED.h"                              
#define DATA_PIN   8
#define NUM_LEDS   64 

#define BRIGHTNESS  50
CRGB leds[NUM_LEDS];
CHSV hsv[NUM_LEDS];

void setup() {
 Serial.begin(115200);                               
 FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);


 FastLED.setBrightness(255);
 FastLED.clear();
}


void loop(){
  
 HSV_Value(); 

}



void HSV_Value(){
  
  EVERY_N_SECONDS(2){
    for (uint8_t i=0; i < NUM_LEDS; i++) {
      hsv[i].h = random8();
      hsv[i].s = random8(200,256);
      hsv[i].v = random8(64,85);
      leds[i] = CHSV(hsv[i].h, hsv[i].s, hsv[i].v);

     /* Serial.print("HSV["); Serial.print(i);
      Serial.print("]: "); Serial.print(hsv[i].h);
      Serial.print(" "); Serial.print(hsv[i].s);
      Serial.print(" "); Serial.println(hsv[i].v);*/
    }
    FastLED.show();  
  }

}
