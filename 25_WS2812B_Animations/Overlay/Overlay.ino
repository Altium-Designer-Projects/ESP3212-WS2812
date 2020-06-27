#include "FastLED.h"                              

#define DATA_PIN   8
#define NUM_LEDS   64  

CRGB base[NUM_LEDS];      //base pattern that's always running
CRGB overlay[NUM_LEDS];   //this is added to base
CRGB leds[NUM_LEDS];      //what is sent to pixels and displayed

void setup() {
 Serial.begin(115200);                               
 FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
 
 FastLED.setBrightness(255);
 FastLED.clear();

}

void loop(){
 
 Overlay();

}


void Overlay(){
  static uint8_t gHue = 0; //slowly rotating hue color
  EVERY_N_MILLISECONDS( 30 ) { gHue++; }  //slowly cycle through the rainbow
  fill_rainbow( base, NUM_LEDS, gHue, 255/NUM_LEDS/2);
  fadeToBlackBy( base, NUM_LEDS, 220);  //dim the rainbow down
  
  EVERY_N_MILLISECONDS(60){
   static uint8_t pos = 0;
   overlay[pos] = overlay[NUM_LEDS-1-pos] = CRGB::Pink;
   fadeToBlackBy( overlay, NUM_LEDS, 120);  //creates fading tail effect
   pos++;
   if(pos == NUM_LEDS/2) {  //check against desired range
      pos = 0;  //reset for next round
   }
  }
  for(uint8_t i=0; i<NUM_LEDS; i++){
    leds[i] = base[i];      //copy base to leds
    leds[i] += overlay[i];  //add overlay to leds
  }
  FastLED.show();     //show the pixels
}
