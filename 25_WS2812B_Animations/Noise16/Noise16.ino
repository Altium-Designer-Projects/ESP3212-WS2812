#include "FastLED.h"                              
#define DATA_PIN   8
#define NUM_LEDS   64 

CRGB leds[NUM_LEDS];

CRGBPalette16 currentPalette;
CRGBPalette16 targetPalette;
TBlendType    currentBlending= LINEARBLEND;

void setup() {
 Serial.begin(115200);                               
 FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
 FastLED.setBrightness(255);
 FastLED.setMaxPowerInVoltsAndMilliamps(5, 500);               // FastLED Power management set at 5V, 500mA.

} 

void loop(){

  Noise16();

} 
 
void Noise16(){   // "lavalamp effect"
  
  currentPalette = PartyColors_p;
  
  EVERY_N_MILLISECONDS(50){
    uint8_t maxChanges = 24;      // Number of tweens between palettes.
    nblendPaletteTowardPalette(currentPalette, targetPalette, maxChanges);  // Blend towards the target palette
  }

  EVERY_N_SECONDS(5) {             // Change the target palette to a random one every 5 seconds.
    targetPalette = CRGBPalette16(CHSV(random8(), 255, random8(128,255)), CHSV(random8(), 255, random8(128,255)), CHSV(random8(), 192, random8(128,255)), CHSV(random8(), 255, random8(128,255)));
  }
  
  uint8_t scale = 1000;                                       // the "zoom factor" for the noise

  for(uint16_t i = 0; i < NUM_LEDS; i++){

    uint16_t shift_x = millis() / 10;                         // x as a function of time
    uint16_t shift_y = 0;
    uint32_t real_x = (i + shift_x) * scale;                  // calculate the coordinates within the noise field
    uint32_t real_y = (i + shift_y) * scale;                  // based on the precalculated positions
    uint32_t real_z = 4223;
    uint8_t noise = inoise16(real_x, real_y, real_z) >> 8;    // get the noise data and scale it down
    uint8_t index = sin8(noise*3);                            // map led color based on noise data
    uint8_t bri   = noise;

    leds[i] = ColorFromPalette(currentPalette, index, bri, LINEARBLEND);   // With that value, look up the 8 bit colour palette value and assign it to the current LED.

  }
  FastLED.show();
} 
