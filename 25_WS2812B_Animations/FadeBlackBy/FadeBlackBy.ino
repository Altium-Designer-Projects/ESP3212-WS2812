#include <FastLED.h>

#define NUM_LEDS  1
#define DATA_PIN  8

CRGB leds[NUM_LEDS];

void setup(){ 
 Serial.begin(9600);
 FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
 FastLED.setBrightness(255);
 FastLED.setMaxPowerInVoltsAndMilliamps(5,500);    // FastLED Power management set at 5V, 500mA.  
 FastLED.clear();
}

void loop(){ 
/*
  fadeInOutBlackBy(CRGB::Red);
  fadeInOutBlackBy(CRGB::DeepSkyBlue);
  fadeInOutBlackBy(CRGB::Yellow);
  fadeInOutBlackBy(CRGB::Cyan);
*/

/*
  fadeInOutBlackBy(CRGB::DeepPink);
  fadeInOutBlackBy(CRGB::Teal);
  fadeInOutBlackBy(CRGB::HotPink);
  fadeInOutBlackBy(CRGB::DarkViolet);
*/

  fadeInOutBlackBy(CRGB::MediumVioletRed);
  fadeInOutBlackBy(CRGB::Gold);
  fadeInOutBlackBy(CRGB::MediumSpringGreen);
  fadeInOutBlackBy(CRGB::MediumOrchid);

 
} // loop

void fadeOutBlackBy(CRGB predefinedColors){ 
  for(int i = 0; i < NUM_LEDS; i++ ){
      fadeToBlackBy(leds, NUM_LEDS, predefinedColors);  
      FastLED.show();
      FastLED.delay(5);
  }
}

void fadeInBlackBy(CRGB predefinedColors){ 
  for(int i = 0; i < NUM_LEDS; i++ ){
      fadeToBlackBy(leds, NUM_LEDS, predefinedColors);  
      FastLED.show();
      FastLED.delay(5);
  }
}

void fadeInOutBlackBy(CRGB predefinedColors){
 fadeInBlackBy(predefinedColors);
 fadeOutBlackBy(predefinedColors);
}

void fadeOutInBlackBy(CRGB predefinedColors){
 fadeOutBlackBy(predefinedColors); 
 fadeInBlackBy(predefinedColors);
}
