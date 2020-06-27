#include <FastLED.h>
/*
 Hello 
 I design developer tools for embedded electronic systems. You can buy my projects design files.
 https://www.altiumdesignerprojects.com
*/
#define DATA_PIN    27 // ESP3212 27
#define NUM_LEDS    64 

CRGB leds[NUM_LEDS];

void setup(){
 Serial.begin(115200);
 FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
 FastLED.setBrightness(255);
 FastLED.setMaxPowerInVoltsAndMilliamps(5,500);    // FastLED Power management set at 5V, 500mA.  
 FastLED.clear();
}

void loop(){ 

Colors(CRGB::MediumOrchid);
  
/*
Black

Red  
Green              
Blue               
Cyan
Plum

Coral
Purple              
Yellow 
Sienna
Maroon

Amethyst
BlueViolet 
DarkViolet       
DarkOrange
DeepSkyBlue 

SpringGreen
MediumOrchid
MidnightBlue
PaleVioletRed   
MediumVioletRed  

*/

//Colors(CRGB::DarkOrange);
//Colors(CRGB::DarkViolet);
//Colors(CRGB::MediumOrchid);
//Colors(CRGB::Amethyst);
//Colors(CRGB::Cyan);
//Colors(CRGB::Coral);
//Colors(CRGB::Sienna);
//Colors(CRGB::Aqua);
//Colors(CRGB::Maroon);
// Colors(CRGB::MidnightBlue); 





// colorHex(0x008080);
 

} 

void Colors(CRGB predefinedColors){
 fill_solid( leds, NUM_LEDS, predefinedColors);
 FastLED.show();
}

void ColorsPixel(int Pixel,CRGB predefinedColors){
 leds[Pixel] = predefinedColors;
 FastLED.show();
}

void setPixel(int Pixel, byte red, byte green, byte blue){
 leds[Pixel].r = red;
 leds[Pixel].g = green;
 leds[Pixel].b = blue;
}

void setAll(byte red, byte green, byte blue){
  for(int i = 0; i < NUM_LEDS; i++ ){
    setPixel(i, red, green, blue); 
  }
  FastLED.show();
}

void colorHex(uint32_t colorValue){
  for(int i = 0; i < NUM_LEDS; i++ ){
    leds[i] = colorValue;
  }
  FastLED.show();
}




  
