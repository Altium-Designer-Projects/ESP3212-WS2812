#include "FastLED.h"                              
#define DATA_PIN   8
#define NUM_LEDS   64 
   
CRGB leds[NUM_LEDS];
CRGB leds2[NUM_LEDS];

CRGBPalette16 currentPaletteMover = RainbowColors_p;
CRGBPalette16 targetPaletteMover = OceanColors_p;
TBlendType    currentBlending;                                // NOBLEND or LINEARBLEND

void setup() {
 Serial.begin(115200);                               
 FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
 FastLED.setMaxPowerInVoltsAndMilliamps(5,500);               // FastLED Power management set at 5V, 500mA.

 currentBlending = LINEARBLEND;  
  
}

void loop(){

 Mover();

} 

void Mover(){
 FastLED.setBrightness(128);
 EVERY_N_MILLISECONDS(100){
    uint8_t maxChanges = 24; 
    nblendPaletteTowardPalette(currentPaletteMover, targetPaletteMover, maxChanges);   // AWESOME palette blending capability.
 }

 EVERY_N_SECONDS(5){                      // Change the target palette to a random one every 5 seconds.
    static uint8_t baseC = random8();     // You can use this as a baseline colour if you want similar hues in the next line.
    targetPaletteMover = CRGBPalette16(CHSV(random8(), 255, random8(128,255)), CHSV(random8(), 255, random8(128,255)), CHSV(random8(), 192, random8(128,255)), CHSV(random8(), 255, random8(128,255)));
 }

 for(int i = 0; i < NUM_LEDS; i++){
     uint8_t red = (millis() / 5) + (i * 12);   // speed, length
     if (red > 128) red = 0;
     leds[i] = ColorFromPalette(currentPaletteMover, red, red, currentBlending);
 }                                              // render the first animation into leds2   
 FastLED.show();
} 
