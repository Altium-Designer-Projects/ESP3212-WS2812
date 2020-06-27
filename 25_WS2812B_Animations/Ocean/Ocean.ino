#include "FastLED.h"                              
#define DATA_PIN   8
#define NUM_LEDS   64 

uint8_t max_bright = 128; 
 
struct CRGB leds[NUM_LEDS];

CRGBPalette16 paletteBlack(CRGB::Black);
CRGBPalette16 paletteOcean(OceanColors_p);
TBlendType    currentBlending;                                // NOBLEND or LINEARBLEND 
 
static int16_t dist;                                          // A random number for our noise generator.
uint16_t xscale = 30;                                         // Wouldn't recommend changing this on the fly, or the animation will be really blocky.
uint16_t yscale = 30;                                         // Wouldn't recommend changing this on the fly, or the animation will be really blocky.
uint8_t maxChanges = 24;                                      // Value for blending between palettes.

void setup() {
 Serial.begin(115200);                               
 FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
 
 FastLED.setBrightness(max_bright);
 FastLED.setMaxPowerInVoltsAndMilliamps(5, 500);               // FastLED Power management set at 5V, 500mA.
  
 dist = random16(12345);   // A semi-random number for our noise generator
} 

 
void loop(){

  Ocean();
  
} 
 
void Ocean(){
  EVERY_N_MILLISECONDS(10){
    uint8_t maxChanges = 24; 
    nblendPaletteTowardPalette(paletteBlack, paletteOcean, maxChanges);   // AWESOME palette blending capability.
    for(int i = 0; i < NUM_LEDS; i++) {                                      // Just ONE loop to fill up the LED array as all of the pixels change.
      uint8_t index = inoise8(i*xscale, dist+i*yscale) % 255;                // Get a value from the noise function. I'm using both x and y axis.
      leds[i] = ColorFromPalette(paletteBlack, index, 255, LINEARBLEND);   // With that value, look up the 8 bit colour palette value and assign it to the current LED.
    }
    dist += beatsin8(10,1,4);                                                              // Update the LED array with noise at the new location
  }
  EVERY_N_SECONDS(5){                                        // Change the target palette to a random one every 5 seconds.
    paletteOcean = CRGBPalette16(CHSV(random8(), 255, random8(128,255)), CHSV(random8(), 255, random8(128,255)), CHSV(random8(), 192, random8(128,255)), CHSV(random8(), 255, random8(128,255)));
  }
  FastLED.show();
} 
