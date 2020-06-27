#include "FastLED.h"                              
#define DATA_PIN   8
#define NUM_LEDS   64 

CRGB leds[NUM_LEDS];               

uint8_t fadevalBeat = 224; // Trail behind the LED's. Lower => faster fade.
uint8_t bpm = 30;

void setup(){
 Serial.begin(115200);                               
 FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
 FastLED.setBrightness(128);
 FastLED.setMaxPowerInVoltsAndMilliamps(5,500);

} 

void loop(){

  Dot_Beat();

} 

void Dot_Beat(){
  uint8_t inner = beatsin8(bpm, NUM_LEDS/4, NUM_LEDS/4*3);    // Move 1/4 to 3/4
  uint8_t outer = beatsin8(bpm, 0, NUM_LEDS-1);               // Move entire length
  uint8_t middle = beatsin8(bpm, NUM_LEDS/3, NUM_LEDS/3*2);   // Move 1/3 to 2/3
  leds[middle] = CRGB::Purple;
  leds[inner] = CRGB::Blue;
  leds[outer] = CRGB::Yellow;
  nscale8(leds,NUM_LEDS,fadevalBeat);                         // Fade the entire array. Or for just a few LED's, use  nscale8(&leds[2], 5, fadevalBeat);
  FastLED.show();
} 
