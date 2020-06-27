#include "FastLED.h"                              
#define DATA_PIN   8
#define NUM_LEDS   64 

#define BRIGHTNESS 100
CRGB leds[NUM_LEDS];

uint8_t startIndexPalette;
uint8_t colorIndexPalette;

void setup() {
 Serial.begin(115200);                               
 FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
 FastLED.setBrightness(255);
}


void loop(){
  
 Party_Wave();
  
}

void Party_Wave(){
  FastLED.setBrightness(100);
  EVERY_N_MILLISECONDS(10){
    colorIndexPalette = startIndexPalette;
    for( int i = 0; i < NUM_LEDS; i++){
              //ColorFromPalette( paletteName, colorIndexPalette[0-255], brightness[0-255], blendType[NOBLEND or LINEARBLEND])
      leds[i] = ColorFromPalette( PartyColors_p, colorIndexPalette, 255, LINEARBLEND);
      colorIndexPalette = colorIndexPalette + 10;  //how fast to advance through palette
    }
    FastLED.show();
    startIndexPalette = startIndexPalette + 1;  //how fast to move the palette down the strip
  }
}
