#include "FastLED.h"                              
#define DATA_PIN   8
#define NUM_LEDS   64 

CRGB leds[NUM_LEDS];

CRGBPalette16 currentPaletteBlack  = CRGBPalette16(CRGB::Black);    // Use palettes instead of direct CHSV or CRGB assignments
CRGBPalette16 targetPaletteRainbow = RainbowColors_p;   // Also support smooth palette transitioning
TBlendType    currentBlending = LINEARBLEND; 
 
uint8_t juggleDots   = 4;          // Number of dots in use.
uint8_t juggleFade   = 2;          // How long should the trails be. Very low value = longer trails.
uint8_t juggleDiff   = 16;         // Incremental change in hue between each dot.
uint8_t juggleHue    = 0;          // Starting hue.
uint8_t juggleCurHue = 0;          // The current hue
uint8_t juggleSat    = 255;        // Saturation of the colour.
uint8_t juggleBright = 255;        // How bright should the LED/display be.
uint8_t juggleBeat   = 5;          // Higher = faster movement.

void setup(){
 Serial.begin(115200);                               
 FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
 FastLED.setBrightness(255);
 FastLED.setMaxPowerInVoltsAndMilliamps(5,500); 
  
}

void loop(){

 Juggle();
  
}

void Juggle(){   
 EVERY_N_MILLISECONDS(100){  
    uint8_t maxChanges = 24; 
    nblendPaletteTowardPalette(currentPaletteBlack, targetPaletteRainbow, maxChanges);   
 }
 uint8_t secondHand = (millis() / 1000) % 30;      // IMPORTANT!!! Change '30' to a different value to change duration of the loop.
 static uint8_t lastSecond = 99;                   // Static variable, means it's only defined once. This is our 'debounce' variable.
 if (lastSecond != secondHand){                    // Debounce to make sure we're not repeating an assignment.
    lastSecond = secondHand;
    switch(secondHand){
      case  0: juggleDots = 1; juggleBeat = 20; juggleDiff = 16; juggleFade = 2; juggleHue = 0; break;                  // You can change values here, one at a time , or altogether.
      case 10: juggleDots = 4; juggleBeat = 10; juggleDiff = 16; juggleFade = 8; juggleHue = 128; break;
      case 20: juggleDots = 8; juggleBeat =  3; juggleDiff =  0; juggleFade = 8; juggleHue=random8(); break;           // Only gets called once, and not continuously for the next several seconds. Therefore, no rainbows.
      case 30: break;
    }
 }
 juggleCurHue = juggleHue;                                           // Reset the hue values.
 fadeToBlackBy(leds, NUM_LEDS, juggleFade);
 for(int i = 0; i < juggleDots; i++){
     leds[beatsin16(juggleBeat+i+juggleDots,0,NUM_LEDS)] += ColorFromPalette(currentPaletteBlack, juggleCurHue , juggleBright, currentBlending);    // Munge the values and pick a colour from the palette
     juggleCurHue += juggleDiff;
 }
 FastLED.show(); 
}
