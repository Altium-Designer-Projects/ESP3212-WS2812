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

 Chessboard();

}

void Chessboard(){
  static int16_t pos = 0;   // position along strip
  static int8_t delta = 3;  // delta (can be negative, and/or odd numbers)
  static uint8_t hue = 0;   // hue to display
  EVERY_N_MILLISECONDS(50){
    leds[pos] = CHSV(hue,255,255);
    pos = (pos + delta + NUM_LEDS) % NUM_LEDS;
    if (delta >= 0 && pos == 0) {  //going forward
      hue = hue + random8(42,128);
    }
    if (delta < 0 && pos == NUM_LEDS-1) {  //going backward
      hue = hue + random8(42,128);
    }
  }
  FastLED.show();  
  FastLED.delay(8); 
  EVERY_N_MILLISECONDS( 20 ) { gHueDots++; }   
}
