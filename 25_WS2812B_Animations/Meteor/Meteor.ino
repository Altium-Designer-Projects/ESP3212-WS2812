#include "FastLED.h"
#define DATA_PIN   8
#define NUM_LEDS   64  

CRGB leds[NUM_LEDS];

uint8_t gHueDots = 0; // rotating "base color" used by many of the patterns
#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

void setup() {
 Serial.begin(115200);                               
 FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
 FastLED.setBrightness(255);
 FastLED.clear();
}

void loop(){

  Meteor();

}

void Meteor(){
  static uint8_t pos;  //used to keep track of position
  EVERY_N_MILLISECONDS(70){
    fadeToBlackBy( leds, NUM_LEDS, 200);  //fade all the pixels some
    leds[pos] = CHSV(gHueDots, random8(170,230), 255);
    leds[(pos+5) % NUM_LEDS] = CHSV(gHueDots+64, random8(170,230), 255);
    pos = pos + 1;  //advance position
    
    //This following check is very important.  Do not go past the last pixel!
    if (pos == NUM_LEDS) { pos = 0; }  //reset to beginning
    //Trying to write data to non-existent pixels causes bad things.
  }
  FastLED.show();  
  FastLED.delay(5); 
  EVERY_N_MILLISECONDS( 15 ) { gHueDots++; } 
}
