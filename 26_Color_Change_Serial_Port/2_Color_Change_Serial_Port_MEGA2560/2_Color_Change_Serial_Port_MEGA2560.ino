#include "FastLED.h"
#include "EEPROM.h"
/*
 Hello 
 I design developer tools for embedded electronic systems. You can buy my projects design files.
 https://www.altiumdesignerprojects.com
*/
#define DATA_PIN  8
#define NUM_LEDS  64 

CRGB leds[NUM_LEDS];

byte animationSequence = 0;

String serialBuffer = "";         // a String to hold incoming data
bool bufferComplete = false;      // whether the string is complete
bool currentLineIsBlank = true;

bool eepromControl = true;

void setup(){
 Serial.begin(9600); 
 serialBuffer.reserve(64);  
 FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
 FastLED.setBrightness(255);
 FastLED.setMaxPowerInVoltsAndMilliamps(5,500);    // FastLED Power management set at 5V, 500mA.  
 FastLED.clear();
}

void loop(){
 
   if(eepromControl == true){    
      if(EEPROM.read(1) == 160){  
         animationSequence = EEPROM.read(0);
      }else{
         for(word i = 0; i < 16; i++){ EEPROM.update(i,'\0'); }    
         EEPROM.update(0,animationSequence);    // default value
      }
     eepromControl = false;
   }
 
   if(Serial.available() > 0){
      char inChar = (char)Serial.read();  // get the new byte:
      //Serial.println(inChar);
      serialBuffer += inChar;    // add it to the serialBuffer:
      if(inChar == '\n'){  // every line of text received from the client ends with \r\n
         currentLineIsBlank = true;
      }else if (inChar != '\r'){
         currentLineIsBlank = false;
      }
      if(inChar == '\n' && currentLineIsBlank){
         bufferComplete = true;
      }
   }


  if(bufferComplete == true){ 
     //Serial.println(serialBuffer);
     if(serialBuffer.indexOf("0") > -1){ 
        animationSequence = 0; 
        EEPROM.update(0,animationSequence); 
        EEPROM.update(1,160);          
     }  
     if(serialBuffer.indexOf("1") > -1){ 
        animationSequence = 1; 
        EEPROM.update(0,animationSequence); 
        EEPROM.update(1,160);   
     }
     if(serialBuffer.indexOf("2") > -1){ 
        animationSequence = 2; 
        EEPROM.update(0,animationSequence); 
        EEPROM.update(1,160);   
     }
     if(serialBuffer.indexOf("3") > -1){ 
        animationSequence = 3; 
        EEPROM.update(0,animationSequence); 
        EEPROM.update(1,160);        
     }  
     if(serialBuffer.indexOf("4") > -1){ 
        animationSequence = 4; 
        EEPROM.update(0,animationSequence); 
        EEPROM.update(1,160);      
     }
     if(serialBuffer.indexOf("5") > -1){ 
        animationSequence = 5; 
        EEPROM.update(0,animationSequence); 
        EEPROM.update(1,160);     
     }
     if(serialBuffer.indexOf("6") > -1){ 
        animationSequence = 6; 
        EEPROM.update(0,animationSequence); 
        EEPROM.update(1,160);       
     }
     if(serialBuffer.indexOf("7") > -1){ 
        animationSequence = 7; 
        EEPROM.update(0,animationSequence); 
        EEPROM.update(1,160);   
     }
     if(serialBuffer.indexOf("8") > -1){ 
        animationSequence = 8; 
        EEPROM.update(0,animationSequence); 
        EEPROM.update(1,160);   
     }  
     if(serialBuffer.indexOf("9") > -1){ 
        animationSequence = 9; 
        EEPROM.update(0,animationSequence); 
        EEPROM.update(1,160);            
     }
     if(serialBuffer.indexOf("10") > -1){ 
        animationSequence = 10; 
        EEPROM.update(0,animationSequence); 
        EEPROM.update(1,160);             
     }   
     if(serialBuffer.indexOf("11") > -1){ 
        animationSequence = 11; 
        EEPROM.update(0,animationSequence); 
        EEPROM.update(1,160);            
     }
     if(serialBuffer.indexOf("12") > -1){ 
        animationSequence = 12; 
        EEPROM.update(0,animationSequence); 
        EEPROM.update(1,160);            
     }
     if(serialBuffer.indexOf("13") > -1){ 
        animationSequence = 13; 
        EEPROM.update(0,animationSequence); 
        EEPROM.update(1,160);           
     }  
     if(serialBuffer.indexOf("14") > -1){ 
        animationSequence = 14; 
        EEPROM.update(0,animationSequence); 
        EEPROM.update(1,160);                    
     }
     if(serialBuffer.indexOf("15") > -1){ 
        animationSequence = 15; 
        EEPROM.update(0,animationSequence); 
        EEPROM.update(1,160);                     
     }     
     if(serialBuffer.indexOf("16") > -1){ 
        animationSequence = 16; 
        EEPROM.update(0,animationSequence); 
        EEPROM.update(1,160);                      
     }
     if(serialBuffer.indexOf("17") > -1){ 
        animationSequence = 17; 
        EEPROM.update(0,animationSequence);  
        EEPROM.update(1,160);                   
     }  
     if(serialBuffer.indexOf("18") > -1){ 
        animationSequence = 18; 
        EEPROM.update(0,animationSequence); 
        EEPROM.update(1,160);                     
     }
     if(serialBuffer.indexOf("19") > -1){ 
        animationSequence = 19; 
        EEPROM.update(0,animationSequence); 
        EEPROM.update(1,160);                     
     }  
     if(serialBuffer.indexOf("20") > -1){ 
        animationSequence = 20; 
        EEPROM.update(0,animationSequence);   
        EEPROM.update(1,160);                   
     }  
     
     serialBuffer = "";   // clear the string
     bufferComplete = false;
  } // buffer



 switch(animationSequence){
 case 0:
  Colors(CRGB::Black);
 break;
 case 1:
  Colors(CRGB::Red);
 break;
 case 2:
  Colors(CRGB::Green);
 break;
 case 3:
  Colors(CRGB::Blue); 
 break;
 case 4:
  Colors(CRGB::Cyan);
 break;
 case 5:
  Colors(CRGB::Plum);   
 break;
 case 6:
  Colors(CRGB::Coral);  
 break;
 case 7:
  Colors(CRGB::Purple);  
 break;
 case 8:
  Colors(CRGB::Yellow); 
 break;
 case 9:
  Colors(CRGB::Sienna); 
 break;
 case 10:
  Colors(CRGB::Maroon);   
 break;
 case 11:
  Colors(CRGB::Amethyst);    
 break;
 case 12:
  Colors(CRGB::BlueViolet);
 break;
 case 13:
  Colors(CRGB::DarkViolet);
 break;
 case 14:
  Colors(CRGB::DarkOrange);  
 break;
 case 15:
  Colors(CRGB::DeepSkyBlue);    
 break;
 case 16:
  Colors(CRGB::SpringGreen);   
 break;
 case 17:
  Colors(CRGB::MediumOrchid);    
 break;
 case 18:
  Colors(CRGB::MidnightBlue);  
 break;
 case 19:
  Colors(CRGB::PaleVioletRed);  
 break;
 case 20:
  Colors(CRGB::MediumVioletRed);
 break;
 }


} // loop

//   ****************************** FUNCTIONS **********************

void Colors(CRGB predefinedColors){
 fill_solid( leds, NUM_LEDS, predefinedColors);
 FastLED.show();
}
