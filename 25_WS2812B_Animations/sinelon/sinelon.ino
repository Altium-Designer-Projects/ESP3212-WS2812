#include "FastLED.h"
#define DATA_PIN   8
#define NUM_LEDS   64  

CRGB leds[NUM_LEDS];

uint8_t gHueDots = 0;         // rotating "base color" used by many of the patterns
#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

void setup() {
 Serial.begin(115200);                               
 FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
 FastLED.setBrightness(128);
 FastLED.clear();
}

void loop(){

  Sinelon();

}

void Sinelon(){
  // a colored dot sweeping back and forth, with fading trails
  fadeToBlackBy( leds, NUM_LEDS, 12);
  int pos = beatsin16( 13, 0, NUM_LEDS-1 );
  leds[pos] += CHSV( gHueDots, 255, 192);
  FastLED.show();  
  FastLED.delay(8); 
  EVERY_N_MILLISECONDS( 20 ) { gHueDots++; } 
}
