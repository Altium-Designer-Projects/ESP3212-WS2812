#include "FastLED.h"                              

#define DATA_PIN  8
#define NUM_LEDS 64   

CRGB leds[NUM_LEDS];                  

CRGBPalette16 currentPalette;
CRGBPalette16 targetPalette;
TBlendType    currentBlending;                                // NOBLEND or LINEARBLEND

uint8_t thisbeatSnowfall =  23;                                       // Beats per minute for first part of dot.
uint8_t thatbeatSnowfall =  28;                                       // Combined the above with this one.
uint8_t thisfadeSnowfall =  32;                                       // How quickly does it fade? Lower = slower fade rate.
uint8_t  thissatSnowfall = 255;                                       // The saturation, where 255 = brilliant colours.
uint8_t  thisbriSnowfall = 255;                                       // Brightness of a sequence.
int        myhueSnowfall =   0;

byte Second = 0;
byte Minute = 0;
byte lastSecond;

char rx_byte;
bool timerStart = false;

void setup(){
 Serial.begin(115200);
 FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);

 currentBlending = LINEARBLEND;
  
 FastLED.setBrightness(255);
 FastLED.setMaxPowerInVoltsAndMilliamps(5, 500);               // FastLED Power management set at 5V, 500mA.
 
 noInterrupts();      // disable all interrupts  // Timer1 kesmesi ayarlanıyor
 TCNT1  = 0;   // initialize counter value to 0
 TCCR1A = 0;   // same for TCCR1B
 TCCR1B = 0;   // set entire TCCR1A register to 0
 OCR1A = 15624;  // set compare match register for 1hz increments // = (16*10^6) / (1*1024) - 1 ( must be < 65536 )
 TCCR1B |= (1 << WGM12);               // turn on CTC mode   
 TCCR1B |= (1 << CS12) | (1 << CS10);  // Set CS10 and CS12 bits for 1024 prescaler
 TIMSK1 |= (1 << OCIE1A);              // enable timer compare interrupt
 interrupts();        // enable 
}

void loop(){
  timerStart = true;
  
  if(Second >= 0 && Second <= 15){
     targetPalette = OceanColors_p;
  }else if(Second >= 15 && Second <= 30){
     targetPalette = LavaColors_p;
  }else if(Second >= 30 && Second <= 45){
     targetPalette = ForestColors_p;
  }else if(Second >= 45 && Second <= 60){
    static uint8_t baseC = random8();                         // You can use this as a baseline colour if you want similar hues in the next line.
    targetPalette = CRGBPalette16(CHSV(random8(), 255, random8(128,255)), CHSV(random8(), 255, random8(128,255)), CHSV(random8(), 192, random8(128,255)), CHSV(random8(), 255, random8(128,255)));
  }
  
  EVERY_N_MILLISECONDS(100){
    uint8_t maxChanges = 24; 
    nblendPaletteTowardPalette(currentPalette, targetPalette, maxChanges);   // AWESOME palette blending capability.
  }
  EVERY_N_MILLISECONDS(50){                           // FastLED based non-blocking delay to update/display the sequence.
    Snowfall();                               
  }
  FastLED.show();

} // loop

ISR(TIMER1_COMPA_vect){   
 
 if(timerStart == true){ 
    Second++;
    if(Second >= 60){
       Second = 0;
       Minute++;
       if(Minute >= 60){
          Minute = 0;
       }
    }

   if(Second != lastSecond){
      Serial.println("Time -- " + String(Minute) +":"+ String(Second));  
   }
   lastSecond = Second;    
 }
 
}

void Snowfall(){                                              // a colored dot sweeping back and forth, with fading trails
  
  fadeToBlackBy( leds, NUM_LEDS, thisfadeSnowfall);
  int pos1 = beatsin16(thisbeatSnowfall,0,NUM_LEDS);
  int pos2 = beatsin16(thatbeatSnowfall,0,NUM_LEDS);

  leds[(pos1+pos2)/2] += ColorFromPalette(currentPalette, myhueSnowfall++, thisbriSnowfall, currentBlending);

}
