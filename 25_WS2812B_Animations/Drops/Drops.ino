#include "FastLED.h"                              
#define DATA_PIN   8
#define NUM_LEDS   64                                      

CRGB leds[NUM_LEDS];
CHSV leds_vu[NUM_LEDS];  // FastLED array using HSV.

int target[NUM_LEDS];  // Array to store target brightness values.
int count;
int delta;
int temp;
int highCutoff = 300;
int lowCutoff = 30;

void setup() { 
 Serial.begin(115200);                               
 FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
 FastLED.setBrightness(255);

}

void loop(){

 //Drops(160,0);

 //Drops(240,128);

 Drops(160,32);
 
 //Drops(240,64);
 
/*
Red (0..) "HUE_RED"
Orange (32..) "HUE_ORANGE"
Yellow (64..) "HUE_YELLOW"
Green (96..) "HUE_GREEN"
Aqua (128..) "HUE_AQUA"
Blue (160..) "HUE_BLUE"
Purple (192..) "HUE_PURPLE"
Pink(224..) "HUE_PINK"
*/
}


void Drops(int hue_low,int hue_high){
  if(count == 0){
    for(int i=0; i < NUM_LEDS; i++){
      target[random8(NUM_LEDS)] = random8();  // Pick random pixels, assign random values.
      if (target[i] < lowCutoff){
        target[i] = 0;  // Force low values to clamp to zero.
      }
    }
    count = random8(25,110);  // Pick a new count value.
  }

  for(int i=0; i < NUM_LEDS; i++){
    if(leds_vu[i].value < target[i]){  // less then the target, so fade up.
      delta = (leds_vu[i].value - target[i]) / 5;
      if (leds_vu[i].value + delta >= target[i]){
        leds_vu[i].value = target[i];  // limit to target.
      }else{
        leds_vu[i].value += delta;
      }
    }else{  // greater then the target, so fade down.
      delta = ((leds_vu[i].value - target[i])/8) + 1;  // +1 so delta is always at least 1.
      if (leds_vu[i].value - delta <= target[i]){
        leds_vu[i].value = target[i];  // limit to target.
      }else{
        leds_vu[i].value -= delta;
      }
    }
    if(i == 0){ temp = delta; }  // Save first pixel's delta number to print below.
    // Tweak hue color based on brightness.
    int c_hue = constrain(leds_vu[i].value,lowCutoff,highCutoff);
    leds_vu[i].hue = map(c_hue, lowCutoff, highCutoff, hue_low, hue_high);
                  // map(valueIn, fromLow,   fromHigh,   toLow,   toHigh)
    leds_vu[i].saturation = 255;  // Set saturation to full.
    leds[i] = leds_vu[i];
    if (target[i] > lowCutoff){
      target[i] -= 2;  // Fade by this ammount.
    }else{
      target[i] = 0;  // If target less then lowCutoff, clamp to zero.
    }
  }
  FastLED.show();  // Display the leds[] array.
  count--;  
  delay(10);
}
