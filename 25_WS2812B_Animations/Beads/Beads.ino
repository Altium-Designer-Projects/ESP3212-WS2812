#include "FastLED.h"
#define DATA_PIN   8
#define NUM_LEDS   64  
/*
 Hello 
 I design developer tools for embedded electronic systems. You can buy my projects design files.
 https://www.altiumdesignerprojects.com
*/
CRGB leds[NUM_LEDS];

uint8_t gHueDots = 0; // rotating "base color" used by many of the patterns
#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

void setup(){
 Serial.begin(115200);                               
 FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
 FastLED.setBrightness(128);
 FastLED.clear();
}

void loop(){

 Beads();

}


void Beads(){
  const uint16_t spewSpeed = 100;  // rate of advance
  static boolean spewing = 0;      // pixels are On(1) or Off(0)
  static uint8_t count = 1;        // how many to light (or not light)
  static uint8_t temp = count;
  static uint8_t hue = random8();
  EVERY_N_MILLISECONDS(spewSpeed){
    if (count == 0) {
      spewing = !spewing;
      if (spewing == 1) { count = random8(2,5); }  // random number for On pixels
      else { count = random8(1,8); }  // random number for Off pixels
      temp = count;
      //gHueDots = gHueDots - 30;
      hue = random8();
    }
    for (uint8_t i = NUM_LEDS-1; i > 0; i--) {
      leds[i] = leds[i-1];  // shift data down the line by one pixel
    }
    if (spewing == 1) {  // new pixels are On
      if (temp == count) {
        leds[0] = CHSV(hue-5, 215, 255);  // for first dot
        //leds[0] = CHSV(gHueDots-5, 215, 255);  // for first dot
      } else {
        leds[0] = CHSV(hue, 255, 255/(1+((temp-count)*2)) );  // for following dots
        //leds[0] = CHSV(gHueDots, 255, 255/(1+((temp-count)*2)) );  // for following dots
      }
    } else {  // new pixels are Off
      leds[0] = CHSV(0,0,0);  // set pixel 0 to black
    }
    count = count - 1;  // reduce count by one.
  }//end every_n
  FastLED.show();  
  FastLED.delay(8); 
  EVERY_N_MILLISECONDS( 20 ) { gHueDots++; } // slowly cycle the "base color" through the rainbow
}
