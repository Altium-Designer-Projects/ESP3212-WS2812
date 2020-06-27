#include <FastLED.h>
#define NUM_LEDS   64 
#define DATA_PIN   27   
/*
 Hello 
 I design developer tools for embedded electronic systems. You can buy my projects design files.
 https://www.altiumdesignerprojects.com
*/
CRGB leds[NUM_LEDS];

byte Second = 0;
byte Minute = 0;
byte lastSecond;
bool tStart = false;

char rx_byte;
byte animationSequence = 0;

static uint8_t hue = 0;

hw_timer_t * timer1 = NULL;
portMUX_TYPE timerMux1 = portMUX_INITIALIZER_UNLOCKED;
void IRAM_ATTR onTimer1(){

 if(tStart == true){ 
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

} // timer son

void setup(){
 Serial.begin(115200);   
 FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
 setAll(0,0,0);

 timer1 = timerBegin(1, 80, true);                // timer 1, MWDT clock period = 12.5 ns * TIMGn_Tx_WDT_CLK_PRESCALE -> 12.5 ns * 80 -> 1000 ns = 1 us, countUp
 timerAttachInterrupt(timer1, &onTimer1, true);   // edge (not level) triggered 
 timerAlarmWrite(timer1, 1000000, true);          // 1000000 * 1 us = 1 second, autoreload true 
 timerAlarmEnable(timer1);                        // enable // at least enable the timer alarms 
}

void loop(){
  
 if(Serial.available() > 0){       
    rx_byte = Serial.read();  
    
    if(rx_byte == '0'){
       setAll(0,0,0);
       tStart = false;
       Second = 0;
       animationSequence = 0;
    }
    
    if(rx_byte == '1'){
       setAll(0,0,0);
       Second = 0;
       tStart = true;
       animationSequence = 1;
    }
    
    if(rx_byte == '2'){
       setAll(0,0,0);
       Second = 0;      
       tStart = true;
       animationSequence = 2;
    }
    
    if(rx_byte == '3'){
       setAll(0,0,0);
       Second = 0;      
       tStart = true;
       animationSequence = 3;
    }
 }

 switch(animationSequence){
 case 0:
  setAll(0,0,0);
 break;
 case 1:
   if(Second >= 0 && Second <= 10){  
    
   }else if(Second >= 10 && Second <= 20){
   
   }else if(Second >= 20 && Second <= 30){
   
   }else if(Second >= 30 && Second <= 40){
     
   }else if(Second >= 40 && Second <= 50){
    
   }else if(Second >= 50 && Second <= 60){
    
   }
 break;
 case 2:
   if(Second >= 0 && Second <= 20){
     
   }else if(Second >= 20 && Second <= 40){
     
   }else if(Second >= 40 && Second <= 60){
    
   } 
 break;
 case 3:
   Sparkle(255,255,0,80); // yellow
 break;
 case 4:
 
 break;
 }
 

  
} // loop

// ***  anim
