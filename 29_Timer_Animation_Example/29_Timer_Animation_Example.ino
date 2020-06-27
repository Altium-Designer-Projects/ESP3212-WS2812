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
bool tStart = false; // timerStart

char rx_byte;

bool animationStart = false;

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

}

void setup(){
 Serial.begin(115200);  
 FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
 setAll(0,0,0);

 timer1 = timerBegin(1, 80, true);  // timer 1, MWDT clock period = 12.5 ns * TIMGn_Tx_WDT_CLK_PRESCALE -> 12.5 ns * 80 -> 1000 ns = 1 us, countUp
 timerAttachInterrupt(timer1, &onTimer1, true); // edge (not level) triggered 
 timerAlarmWrite(timer1, 1000000, true); // 1000000 * 1 us = 1 second, autoreload true 
 timerAlarmEnable(timer1); // enable // at least enable the timer alarms 
}

void loop(){

 if(Serial.available() > 0){       
    rx_byte = Serial.read();       
    if(rx_byte == '1'){
       tStart = true;
       animationStart = true; 
    }
    
    if(rx_byte == '2'){
       tStart = false;
       animationStart = false;
    }
    
    if(rx_byte == '3'){
       Second = 0;
    }
 }
 
 if(animationStart == true){
   
   if(Second >= 0 && Second <= 10){
      Sparkle(50,0,50,50);
   }else if(Second >= 10 && Second <= 20){
      Sparkle(0,0,255,50);
   }else if(Second >= 20 && Second <= 30){
      Sparkle(0,255,0,50);
   }else if(Second >= 30 && Second <= 40){
      Sparkle(255,0,0,50); 
   }else if(Second >= 40 && Second <= 50){
      Sparkle(0,250,155,50); 
   }else if(Second >= 50 && Second <= 60){
      Sparkle(219,112,147,50); 
   }
      
 }

}

void setPixel(int Pixel, byte red, byte green, byte blue) {  // değiştirelim
   leds[Pixel].r = red;
   leds[Pixel].g = green;
   leds[Pixel].b = blue;
}

void setAll(byte red, byte green, byte blue) {
  for(int i = 0; i < NUM_LEDS; i++ ) {
    setPixel(i, red, green, blue); 
  }
  FastLED.show();
}

void Sparkle(byte red, byte green, byte blue, int SpeedDelay) {
  int Pixel = random(NUM_LEDS);
  setPixel(Pixel,red,green,blue);
  FastLED.show();
  FastLED.delay(SpeedDelay);
  setPixel(Pixel,0,0,0);
}
