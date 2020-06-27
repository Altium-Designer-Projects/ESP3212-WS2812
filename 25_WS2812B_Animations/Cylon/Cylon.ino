#include <FastLED.h>
#define NUM_LEDS   64 
#define DATA_PIN   8   

CRGB leds[NUM_LEDS];

uint8_t hue = 0;

void setup() { 
	Serial.begin(115200);
	FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
	FastLED.setBrightness(128);
}

void loop(){ 

 Cylon();
 
}

void Cylon(){ 
  for(int i = 0; i < NUM_LEDS; i++){
    leds[i] = CHSV(hue++, 255, 255);
    FastLED.show(); 
    for(int i = 0; i < NUM_LEDS; i++){
     leds[i].nscale8(250);
    }
    FastLED.delay(10);
  }

  for(int i = (NUM_LEDS)-1; i >= 0; i--){
    leds[i] = CHSV(hue++, 255, 255);
    FastLED.show();
    for(int i = 0; i < NUM_LEDS; i++){
     leds[i].nscale8(250);
    }
    FastLED.delay(10);
  }
}
