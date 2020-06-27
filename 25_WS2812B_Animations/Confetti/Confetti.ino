#include "FastLED.h"
#define DATA_PIN   8
#define NUM_LEDS   64  

CRGB leds[NUM_LEDS];

uint8_t gHueDots = 0; // rotating "base color" used by many of the patterns
#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

void setup() {
 Serial.begin(115200);                               
 FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
 FastLED.setBrightness(128);
 FastLED.clear();
}

void loop(){

  //Confetti_Random();
  
  //Confetti_Green_Blue();
  
  Confetti_Speckles();
  


}


void Confetti_Random(){
  fadeToBlackBy( leds, NUM_LEDS, 20);
  int pos = random16(NUM_LEDS);
  leds[pos] += CHSV( gHueDots + random8(64), random8(128,200), random8(48,255));
  FastLED.show();  
  FastLED.delay(8); 
  EVERY_N_MILLISECONDS( 20 ) { gHueDots++; } // slowly cycle the "base color" through the rainbow  
}

void Confetti_Green_Blue(){
  uint8_t p = 25;  // What percentage of the time to make speckles.  [Range 0-100]
  fadeToBlackBy( leds, NUM_LEDS, 10);
  if (random8(100) < p) {
    int pos = random16(NUM_LEDS);
    uint8_t hue = random8(2);  // randomly chooses a 0 or 1
    if (hue == 0) {
      hue = random8(92,111);  // pick a hue somewhere in the green zone
    } else {
      hue = random8(156,165);  // pick a hue somewhere in the blue zone
    }
    leds[pos] += CHSV( hue, random8(200,240), 255);
  }
  FastLED.show();  
  FastLED.delay(8); 
  EVERY_N_MILLISECONDS( 20 ) { gHueDots++; } // slowly cycle the "base color" through the rainbow  
}

void Confetti_Speckles(){   // Random colored speckles that blink in and fade smoothly.
  fadeToBlackBy(leds, NUM_LEDS, 10);
  int pos = random16(NUM_LEDS);
  leds[pos] += CHSV(gHueDots + random8(64), 200, 255);
  FastLED.show();  
  FastLED.delay(8); 
  EVERY_N_MILLISECONDS( 20 ) { gHueDots++; } // slowly cycle the "base color" through the rainbow  
}
