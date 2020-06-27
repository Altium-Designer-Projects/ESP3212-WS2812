#include <FastLED.h>

#define LED_PIN      8
#define NUM_LEDS     64

#define LED_TYPE     WS2812B
#define COLOR_ORDER  GRB
CRGB leds[NUM_LEDS];

void setup(){
 FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
 FastLED.setBrightness(255);

}


void loop(){
    
 static uint8_t startIndex = 0;
 startIndex = startIndex + 1; 
 RainbowColors( startIndex);
 FastLED.show(); FastLED.delay(10);
 
}

void RainbowColors( uint8_t colorIndex){
 for(int i = 0; i < NUM_LEDS; i++){
     leds[i] = ColorFromPalette( RainbowColors_p, colorIndex, 255, LINEARBLEND);
     colorIndex += 3;
 }
}
