#include "FastLED.h"
#define DATA_PIN   8
#define NUM_LEDS   64  

CRGB leds[NUM_LEDS];

uint8_t gHueDots = 0;
#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

void setup(){
 Serial.begin(115200);                               
 FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
 FastLED.setBrightness(128);
 FastLED.clear();
}

void loop(){

  Wizard_A();
  
 // Wizard_B();
  
 // Wizard_C();
    

}

void Wizard_A(){
  // four colored dots, weaving in and out of sync with each other
  fadeToBlackBy( leds, NUM_LEDS, 20);
  byte dothue = 0;
  for( int i = 0; i < 4; i++) {
    leds[beatsin16( i+5, 0, NUM_LEDS-1 )] |= CHSV(dothue, 200, 255);
    dothue += 32;
  }
  FastLED.show();  
  FastLED.delay(8); 
  EVERY_N_MILLISECONDS( 20 ) { gHueDots++; }   
}

void Wizard_B(){
  // four colored dots, weaving in and out of sync with each other
  fadeToBlackBy( leds, NUM_LEDS, 20);
  byte dothue = 0;
  for( int i = 0; i < 8; i++) {
    leds[beatsin16( i+5, 0, NUM_LEDS-1 )] |= CHSV(dothue, 200, 255);
    dothue += 32;
  }
  FastLED.show();  
  FastLED.delay(8); 
  EVERY_N_MILLISECONDS( 20 ) { gHueDots++; }   
}


void Wizard_C(){
  // four colored dots, weaving in and out of sync with each other
  fadeToBlackBy( leds, NUM_LEDS, 20);
  byte dothue = 0;
  for( int i = 0; i < 12; i++) {
    leds[beatsin16( i+5, 0, NUM_LEDS-1 )] |= CHSV(dothue, 200, 255);
    dothue += 16;
  }
  FastLED.show();  
  FastLED.delay(8); 
  EVERY_N_MILLISECONDS( 20 ) { gHueDots++; }   
}
