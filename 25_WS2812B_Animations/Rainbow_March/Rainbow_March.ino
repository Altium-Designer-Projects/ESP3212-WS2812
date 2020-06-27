#include "FastLED.h"                              
#define DATA_PIN   8
#define NUM_LEDS   64 

CRGB leds[NUM_LEDS];

CRGBPalette16 currentPalette;
CRGBPalette16 targetPalette;
TBlendType    currentBlending = LINEARBLEND;                  // NOBLEND or LINEARBLEND

void setup(){
 Serial.begin(115200);                               
 FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);

 FastLED.setBrightness(255);
 FastLED.setMaxPowerInVoltsAndMilliamps(5,500);              // FastLED 2.1 Power management set at 5V, 500mA
}

void loop(){

  Rainbow_March(200, 10);
  FastLED.show();
  
} 

void Rainbow_March(uint8_t thisdelay, uint8_t deltahue){     // The fill_rainbow call doesn't support brightness levels.
 uint8_t thishue = millis() * (255-thisdelay)/255;             // To change the rate, add a beat or something to the result. 'thisdelay' must be a fixed value.
 thishue = beat8(50);                                       // This uses a FastLED sawtooth generator. Again, the '50' should not change on the fly.
 //thishue = beatsin8(50,0,255);                              // This can change speeds on the fly. You can also add these to each other.
  fill_rainbow(leds, NUM_LEDS, thishue, deltahue);            // Use FastLED's fill_rainbow routine.
}
