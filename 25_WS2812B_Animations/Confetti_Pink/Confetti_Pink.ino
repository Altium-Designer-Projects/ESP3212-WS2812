#include "FastLED.h" 

#define DATA_PIN      8
#define NUM_LEDS     64                                      

CRGB leds[NUM_LEDS];                           

uint8_t  confettiFade = 8;              // How quickly does it fade? Lower = slower fade rate.
int       confettiHue = 50;             // Starting hue.
uint8_t   confettiInc = 3;              // Incremental value for rotating hues
uint8_t   confettiSat = 100;            // The saturation, where 255 = brilliant colours.
uint8_t   confettiBri = 255;            // Brightness of a sequence. Remember, max_bright is the overall limiter.
int       confettihuediff = 256;        // Range of random #'s to use for hue

void setup(){
 Serial.begin(115200);    
 FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
 FastLED.setBrightness(128);
 FastLED.setMaxPowerInVoltsAndMilliamps(5, 500);  
} 

void loop(){
  
 Confetti_Pink();
 
} 

void Confetti_Pink(){                                         // random colored speckles that blink in and fade smoothly
  uint8_t secondHand = (millis() / 1000) % 15;                // IMPORTANT!!! Change '15' to a different value to change duration of the loop.
  static uint8_t lastSecond = 99;                             // Static variable, means it's only defined once. This is our 'debounce' variable.
  if (lastSecond != secondHand) {                             // Debounce to make sure we're not repeating an assignment.
    lastSecond = secondHand;
    switch(secondHand) {
      case  0: confettiInc=1; confettiHue=192; confettiSat=255; confettiFade=2; confettihuediff=256; break;  // You can change values here, one at a time , or altogether.
      case  5: confettiInc=2; confettiHue=128; confettiFade=8; confettihuediff=64; break;
      case 10: confettiInc=1; confettiHue=random16(255); confettiFade=1; confettihuediff=16; break;      // Only gets called once, and not continuously for the next several seconds. Therefore, no rainbows.
      case 15: break;                                                                // Here's the matching 15 for the other one.
    }
  }                                               // Check the demo loop for changes to the variables.

  EVERY_N_MILLISECONDS(5){       // FastLED based non-blocking delay to update/display the sequence.
    fadeToBlackBy(leds, NUM_LEDS, confettiFade);                    // Low values = slower fade.
    int pos = random16(NUM_LEDS);                               // Pick an LED at random.
    leds[pos] += CHSV((confettiHue + random16(confettihuediff))/4 , confettiSat, confettiBri);  // I use 12 bits for hue so that the hue increment isn't too quick.
    confettiHue = confettiHue + confettiInc;  
  }
  FastLED.show();
} 
