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

void setup() {
 Serial.begin(115200);                               
 FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
 FastLED.setBrightness(128);
 FastLED.clear();
}

void loop(){

  Applause();

}

void Applause(){   //originally by Mark K.
//Number of loops is increased based on number of pixels.
//Fade ammount drops as pixels increase.
static uint8_t appAmmount = (NUM_LEDS/32);
  for (uint8_t i=0; i<appAmmount; i++){
    static uint16_t lastPixel = 0;
    fadeToBlackBy( leds, NUM_LEDS, 32/appAmmount );  //was 32
    leds[lastPixel] = CHSV(random8(HUE_BLUE,HUE_PURPLE),255,255);
    lastPixel = random16(NUM_LEDS);
    leds[lastPixel] = CRGB::White;
    FastLED.delay(5);
  }
  FastLED.show();  
  FastLED.delay(50); 
  EVERY_N_MILLISECONDS( 80 ) { gHueDots++; } // slowly cycle the "base color" through the rainbow
}
