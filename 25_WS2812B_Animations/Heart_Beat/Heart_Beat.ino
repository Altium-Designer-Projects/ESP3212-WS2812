#include "FastLED.h"                              
#define DATA_PIN   8
#define NUM_LEDS   64 

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))  // Definition for the array of routines to display.

CRGB leds[NUM_LEDS];            

uint8_t gHueDots = 0;     // rotating "base color" used by many of the patterns

void setup() {
 Serial.begin(115200);                               
 FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
 FastLED.setBrightness(128);
 FastLED.setMaxPowerInVoltsAndMilliamps(5,500);               // FastLED Power management set at 5V, 500mA.
} 

void loop(){

  Heart_Beat();

} 

void Heart_Beat(){  // Colored stripes pulsing at a defined Beats-Per-Minute.
  uint8_t BeatsPerMinute = 62;
  CRGBPalette16 palette = PartyColors_p;
  uint8_t beat = beatsin8(BeatsPerMinute, 64, 255);
  
  for(int i = 0; i < NUM_LEDS; i++) { //9948
    leds[i] = ColorFromPalette(palette, gHueDots+(i*2), beat-gHueDots+(i*10));
  }
  EVERY_N_MILLISECONDS(20) { gHueDots++; }
  FastLED.show();
}
