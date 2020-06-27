#include <FastLED.h>
#include <DueTimer.h>
#define DATA_PIN   8   // 23
#define NUM_LEDS   64 

CRGB leds[NUM_LEDS];

void setup(){
 Serial.begin(115200);
 FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
 FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
 FastLED.setBrightness(255);
 FastLED.setMaxPowerInVoltsAndMilliamps(5,500);    // FastLED Power management set at 5V, 500mA.  
 FastLED.clear(); 
}

void loop(){
/* 
 ColorsPixel(1,CRGB::Cyan);
 ColorsPixel(5,CRGB::Cyan);
 ColorsPixel(10,CRGB::Red);
*/

 //Sparkle(CRGB::Cyan,50);

 //Sparkle(CRGB::Red,50);

 Sparkle(CRGB::Red,75);
}
 
 
void Colors(CRGB predefinedColors){
 fill_solid( leds, NUM_LEDS, predefinedColors);
 FastLED.show();
}

void ColorsPixel(int Pixel,CRGB predefinedColors){
 leds[Pixel] = predefinedColors;
 FastLED.show();
}

void Sparkle(CRGB predefinedColors,int SpeedDelay) {
  int Pixel = random(NUM_LEDS);
  ColorsPixel(Pixel,predefinedColors);
  FastLED.show();
  FastLED.delay(SpeedDelay);
  ColorsPixel(Pixel,CRGB::Black);
}
