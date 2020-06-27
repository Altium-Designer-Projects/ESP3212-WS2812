#include "FastLED.h"                              
#define DATA_PIN   8
#define NUM_LEDS   64 

CRGB leds[NUM_LEDS];

CRGBPalette16 currentPaletteBlack = CRGBPalette16(CRGB::Black);
CRGBPalette16 targetPalette;
TBlendType    currentBlending = LINEARBLEND;           // NOBLEND or LINEARBLEND

void setup() {
 Serial.begin(115200);                               
 FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
 FastLED.setBrightness(255);
 FastLED.setMaxPowerInVoltsAndMilliamps(5,500);
}

void loop(){
  
 PaletteCrossFade();
  
}

void PaletteCrossFade(){
  uint8_t secondHand = (millis() / 1000) % 60;
  static uint8_t lastSecond = 99;
  
  if(lastSecond != secondHand){
    lastSecond = secondHand;
    CRGB p = CHSV(HUE_PURPLE, 255, 255);
    CRGB g = CHSV(HUE_GREEN, 255, 255);
    CRGB b = CRGB::Black;
    CRGB w = CRGB::White;
    if(secondHand ==  0)  { targetPalette = RainbowColors_p; }
    if(secondHand == 10)  { targetPalette = CRGBPalette16(g,g,b,b, p,p,b,b, g,g,b,b, p,p,b,b); }
    if(secondHand == 20)  { targetPalette = CRGBPalette16(b,b,b,w, b,b,b,w, b,b,b,w, b,b,b,w); }
    if(secondHand == 30)  { targetPalette = LavaColors_p; }
    if(secondHand == 40)  { targetPalette = CloudColors_p; }
    if(secondHand == 50)  { targetPalette = PartyColors_p; }
  }
  EVERY_N_MILLISECONDS(100){
   uint8_t maxChanges = 24; 
   nblendPaletteTowardPalette(currentPaletteBlack, targetPalette, maxChanges);
  }
  EVERY_N_MILLISECONDS(20){
   static uint8_t startIndex = 0;
   startIndex += 1;                                 // motion speed
   FillLEDsFromPaletteColors(startIndex);
  }
  FastLED.show();
}
void FillLEDsFromPaletteColors(uint8_t colorIndex){
  for(int i = 0; i < NUM_LEDS; i++) {
    leds[i] = ColorFromPalette(currentPaletteBlack, colorIndex + sin8(i*16), 255);
    colorIndex += 3;
  }
}
