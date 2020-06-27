#include "FastLED.h"                                          // FastLED library.

#define DATA_PIN  8
#define NUM_LEDS 64  

#define qsubd(x, b) ((x>b)?b:0)                               // Clip. . . . A digital unsigned subtraction macro. if result <0, then x=0. Otherwise, x=b.
#define qsuba(x, b) ((x>b)?x-b:0)                             // Level shift. . . Unsigned subtraction macro. if result <0, then x=0. Otherwise x=x-b.

struct CRGB leds[NUM_LEDS];                                   // Initialize our LED array.

// Palette definitions
CRGBPalette16 currentPalette;
CRGBPalette16 targetPalette;
TBlendType    currentBlending = LINEARBLEND;                                // NOBLEND or LINEARBLEND

unsigned int ledLoc = 0;                                     // Current active LED.

void setup(){
 Serial.begin(115200);                                        // Initialize serial port for debugging.
 FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
 FastLED.setBrightness(255);
 FastLED.setMaxPowerInVoltsAndMilliamps(5, 500);               // FastLED Power management set at 5V, 500mA.

} 

void loop(){
 
 flash();
 
} 


void flash(){
  currentPalette = PartyColors_p;
  EVERY_N_MILLIS_I(thisTimer,100) {                           // This only sets the Initial timer delay. To change this value, you need to use thisTimer.setPeriod(); You could also call it thatTimer and so on.
    uint8_t timeval = beatsin8(10,5,100);                     // Create/modify a variable based on the beastsin8() function.
    thisTimer.setPeriod(timeval);                             // Use that as our update timer value.
    ledLoc = (ledLoc+1) % (NUM_LEDS-1);                       // A simple routine to just move the active LED UP the strip.
    leds[ledLoc] = ColorFromPalette(currentPalette, ledLoc, 255, currentBlending);     // Pick a slightly rotating colour from the Palette
  }
  fadeToBlackBy(leds, NUM_LEDS, 8);                           // Leave a nice comet trail behind.
  FastLED.show();
} 
