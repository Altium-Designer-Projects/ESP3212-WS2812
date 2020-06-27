#include "FastLED.h"                              
#define DATA_PIN   8
#define NUM_LEDS   64                                      
/*
 Hello 
 I design developer tools for embedded electronic systems. You can buy my projects design files.
 https://www.altiumdesignerprojects.com
*/
CRGB leds[NUM_LEDS];                           

CRGBPalette16 currentPalette;
CRGBPalette16 targetPalette;
TBlendType    currentBlending = LINEARBLEND;

void setup() {
 Serial.begin(115200);                               
 FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);

 //currentBlending = LINEARBLEND;
  
 FastLED.setBrightness(128);
 FastLED.setMaxPowerInVoltsAndMilliamps(5, 500);    // FastLED Power management set at 5V, 500mA.  
} 

void loop(){
  
  Beatwave();
  
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

