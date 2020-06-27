#include <FastLED.h>

#define NUM_LEDS  64
#define DATA_PIN  8

CRGB leds[NUM_LEDS];

void setup(){
 FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
 FastLED.setBrightness(255);
 FastLED.setMaxPowerInVoltsAndMilliamps(5,500);    // FastLED Power management set at 5V, 500mA.  
 FastLED.clear();
}

void loop(){
 
 Twinkle(CRGB::Cyan,32,40);
 
}

void Colors(CRGB predefinedColors){
 fill_solid( leds, NUM_LEDS, predefinedColors);
 FastLED.show();
}

void ColorsPixel(int Pixel,CRGB predefinedColors){
 leds[Pixel] = predefinedColors;
 FastLED.show();
}

void Twinkle(CRGB predefinedColors, int Count, int SpeedDelay){
  Colors(CRGB::Black);
  for (int i=0; i<Count; i++){
     ColorsPixel(random(NUM_LEDS),predefinedColors);
     FastLED.show();
     FastLED.delay(SpeedDelay);
   }
  FastLED.delay(SpeedDelay);
}
