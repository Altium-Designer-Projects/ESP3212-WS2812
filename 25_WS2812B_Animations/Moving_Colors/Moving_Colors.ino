#include "FastLED.h"                              
#define DATA_PIN   8
#define NUM_LEDS   64 

CRGB leds[NUM_LEDS];

int16_t position1 = NUM_LEDS * 2 / 3; 
int16_t position2 = NUM_LEDS / 3; 
int16_t position3 = 0;             

long colorPalletMoving[] = {CRGB::Red,CRGB::Green,CRGB::Blue,CRGB::Teal,CRGB::DarkViolet,CRGB::Gold,CRGB::DeepPink,CRGB::Teal,CRGB::Yellow};  

byte Second = 0;
byte Minute = 0;
byte lastSecond;

char rx_byte;

bool timerStart = false;

void setup() {
 Serial.begin(115200);                               
 FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);

 FastLED.setBrightness(64);
 
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
  
  if(Second >= 0 && Second <= 20){
     Moving_Colors(0,1,2);
  }else if(Second >= 20 && Second <= 40){
     Moving_Colors(3,4,5);
  }else if(Second >= 40 && Second <= 60){
     Moving_Colors(6,7,8);
  }

}


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


void Moving_Colors(int color0,int color1,int color2){
  EVERY_N_MILLISECONDS(120){
    leds[(position1 + 1 + NUM_LEDS) % NUM_LEDS] = colorPalletMoving[color0];
    leds[(position2 + 1 + NUM_LEDS) % NUM_LEDS] = colorPalletMoving[color1];
    leds[(position3 + 1 + NUM_LEDS) % NUM_LEDS] = colorPalletMoving[color2];
    FastLED.show();  
    position1 = ((position1 + 1 + NUM_LEDS) % NUM_LEDS);
    position2 = ((position2 + 1 + NUM_LEDS) % NUM_LEDS);
    position3 = ((position3 + 1 + NUM_LEDS) % NUM_LEDS);
  }
}
