#include "FastLED.h"                              
#define DATA_PIN   8
#define NUM_LEDS   64  

CRGB leds[NUM_LEDS];

int targetChase[NUM_LEDS];  // Place to save some targetChase values.
int countChase;
int deltaChase;
int tempChase;
int lowCutoffChase = 50;

void setup(){ 
 Serial.begin(115200);                               
 FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
 FastLED.setBrightness(255);
}

void loop(){ 
 
 Volcano_Blue();
 
}


void Volcano_Red(){ 
  if(countChase == 0){
    for(int i=0; i < NUM_LEDS; i++){
      targetChase[random8(NUM_LEDS)] = random8();
      if(targetChase[i] < lowCutoffChase){
        targetChase[i] = 0;
      }
    }
    countChase = random8(35,70);
  }

  for(int i=0; i < NUM_LEDS; i++){
    if(leds[i].r < targetChase[i]){
      deltaChase = (targetChase[i] - leds[i].r) / 5;
      if(leds[i].r + deltaChase >= targetChase[i]){
         leds[i].r = targetChase[i];
      }else{
         leds[i].r += deltaChase;
      }
    }else{
      deltaChase = ((leds[i].r - targetChase[i])/8) + 1;
      if(leds[i].r - deltaChase <= targetChase[i]){
         leds[i].r = targetChase[i];
      }else{
         leds[i].r -= deltaChase;
      }
    }
    if (i == 0){ tempChase = deltaChase; }

    if(targetChase[i] > lowCutoffChase){
       targetChase[i] -= 2;
    }else{
       targetChase[i] = 0;
    }  
  }
  FastLED.show();
  countChase--; 
  delay(10);
}

void Volcano_Green(){ 
  if(countChase == 0){
    for(int i=0; i < NUM_LEDS; i++){
      targetChase[random8(NUM_LEDS)] = random8();
      if(targetChase[i] < lowCutoffChase){
        targetChase[i] = 0;
      }
    }
    countChase = random8(35,70);
  }

  for(int i=0; i < NUM_LEDS; i++){
    if(leds[i].g < targetChase[i]){
      deltaChase = (targetChase[i] - leds[i].g) / 5;
      if(leds[i].g + deltaChase >= targetChase[i]){
         leds[i].g = targetChase[i];
      }else{
         leds[i].g += deltaChase;
      }
    }else{
      deltaChase = ((leds[i].g - targetChase[i])/8) + 1;
      if(leds[i].g - deltaChase <= targetChase[i]){
         leds[i].g = targetChase[i];
      }else{
         leds[i].g -= deltaChase;
      }
    }
    if (i == 0){ tempChase = deltaChase; }

    if(targetChase[i] > lowCutoffChase){
       targetChase[i] -= 2;
    }else{
       targetChase[i] = 0;
    }  
  }
  FastLED.show();
  countChase--; 
  delay(10);
}

void Volcano_Blue(){ 
  if(countChase == 0){
    for(int i=0; i < NUM_LEDS; i++){
      targetChase[random8(NUM_LEDS)] = random8();
      if(targetChase[i] < lowCutoffChase){
        targetChase[i] = 0;
      }
    }
    countChase = random8(35,70);
  }

  for(int i=0; i < NUM_LEDS; i++){
    if(leds[i].b < targetChase[i]){
      deltaChase = (targetChase[i] - leds[i].b) / 5;
      if(leds[i].b + deltaChase >= targetChase[i]){
         leds[i].b = targetChase[i];
      }else{
         leds[i].b += deltaChase;
      }
    }else{
      deltaChase = ((leds[i].b - targetChase[i])/8) + 1;
      if(leds[i].b - deltaChase <= targetChase[i]){
         leds[i].b = targetChase[i];
      }else{
         leds[i].b -= deltaChase;
      }
    }
    if (i == 0){ tempChase = deltaChase; }

    if(targetChase[i] > lowCutoffChase){
       targetChase[i] -= 2;
    }else{
       targetChase[i] = 0;
    }  
  }
  FastLED.show();
  countChase--; 
  delay(10);
}
