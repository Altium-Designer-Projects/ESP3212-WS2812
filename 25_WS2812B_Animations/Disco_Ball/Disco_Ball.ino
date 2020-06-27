#include "FastLED.h"                              
#define DATA_PIN   8
#define NUM_LEDS   64 

CRGB leds[NUM_LEDS];

int wave1=0;                    // Current phase is calculated.
int wave2=0;
int wave3=0;
 
CRGBPalette16 currentPaletteBlack(CRGB::Black);
CRGBPalette16 targetPalette;

void setup() {
 Serial.begin(115200);                               
 FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
 FastLED.setBrightness(128);
 FastLED.setMaxPowerInVoltsAndMilliamps(5, 500);               // FastLED Power management set at 5V, 500mA

// random16_set_seed(4832);                                    // Awesome randomizer (which we don't yet need here.)
// random16_add_entropy(1023);
  
}

void loop(){

 Disco_Ball();
 
}

void Disco_Ball(){
  uint8_t secondHand = (millis() / 1000) % 60;
  static uint8_t lastSecond = 99;

  if( lastSecond != secondHand){
    lastSecond = secondHand;
    CRGB p = CHSV( HUE_PURPLE, 255, 255);
    CRGB g = CHSV( HUE_GREEN, 255, 255);
    CRGB u = CHSV( HUE_BLUE, 255, 255);
    CRGB b = CRGB::Black;
    CRGB w = CRGB::White;

    switch(secondHand){
      case  0: targetPalette = RainbowColors_p; break;
      case  5: targetPalette = CRGBPalette16( u,u,b,b, p,p,b,b, u,u,b,b, p,p,b,b); break;
      case 10: targetPalette = OceanColors_p; break;
      case 15: targetPalette = CloudColors_p; break;
      case 20: targetPalette = LavaColors_p; break;
      case 25: targetPalette = ForestColors_p; break;
      case 30: targetPalette = PartyColors_p; break;
      case 35: targetPalette = CRGBPalette16( b,b,b,w, b,b,b,w, b,b,b,w, b,b,b,w); break;
      case 40: targetPalette = CRGBPalette16( u,u,u,w, u,u,u,w, u,u,u,w, u,u,u,w); break;
      case 45: targetPalette = CRGBPalette16( u,p,u,w, p,u,u,w, u,g,u,w, u,p,u,w); break;
      case 50: targetPalette = CloudColors_p; break;
      case 55: targetPalette = CRGBPalette16( u,u,u,w, u,u,p,p, u,p,p,p, u,p,p,w); break;
      case 60: break;
    }
  }

  EVERY_N_MILLISECONDS(20){                           // FastLED based non-blocking delay to update/display the sequence.
    nblendPaletteTowardPalette( currentPaletteBlack, targetPalette, 24);
    wave1 += beatsin8(10,-4,4);
    wave2 += beatsin8(15,-2,2);
    wave3 += beatsin8(12,-3, 3);

    for(int k=0; k<NUM_LEDS; k++){
       uint8_t tmp = sin8(7*k + wave1) + sin8(7*k + wave2) + sin8(7*k + wave3);
       leds[k] = ColorFromPalette(currentPaletteBlack, tmp, 255);
    }                          
  }
  FastLED.show();
}
