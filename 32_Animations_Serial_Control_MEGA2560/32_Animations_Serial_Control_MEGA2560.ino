#include "FastLED.h"
#include "EEPROM.h"
/*
 Hello 
 I design developer tools for embedded electronic systems. You can buy my projects design files.
 https://www.altiumdesignerprojects.com
*/
#define DATA_PIN   8
#define NUM_LEDS  64 
CRGB leds[NUM_LEDS];
CHSV leds_vu[NUM_LEDS];   // FastLED array using HSV.
CHSV hsv[NUM_LEDS];
CRGB base[NUM_LEDS];      // base pattern that's always running
CRGB overlay[NUM_LEDS];   // this is added to base


CRGBPalette16 currentPalette;
CRGBPalette16 targetPalette;
CRGBPalette16 currentPaletteBlack  = CRGBPalette16(CRGB::Black);    // Use palettes instead of direct CHSV or CRGB assignments
CRGBPalette16 targetPaletteRainbow = RainbowColors_p;   // Also support smooth palette transitioning
CRGBPalette16 currentPaletteMover = RainbowColors_p;
CRGBPalette16 targetPaletteMover = OceanColors_p;
CRGBPalette16 paletteBlack(CRGB::Black);
CRGBPalette16 paletteOcean(OceanColors_p);

TBlendType    currentBlending = LINEARBLEND;

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))
#define qsubd(x, b) ((x>b)?b:0)                               // Clip. . . . A digital unsigned subtraction macro. if result <0, then x=0. Otherwise, x=b.
#define qsuba(x, b) ((x>b)?x-b:0)                             // Level shift. . . Unsigned subtraction macro. if result <0, then x=0. Otherwise x=x-b.


uint8_t  confettiFade = 8;              // How quickly does it fade? Lower = slower fade rate.
int       confettiHue = 50;             // Starting hue.
uint8_t   confettiInc = 3;              // Incremental value for rotating hues
uint8_t   confettiSat = 100;            // The saturation, where 255 = brilliant colours.
uint8_t   confettiBri = 255;            // Brightness of a sequence. Remember, max_bright is the overall limiter.
int       confettihuediff = 256;        // Range of random #'s to use for hue

uint8_t gHueDots = 0; // rotating "base color" used by many of the patterns
static uint8_t hueCylon = 0;

int wave1=0;                    // Current phase is calculated.
int wave2=0;
int wave3=0;

uint8_t fadevalBeat = 224; // Trail behind the LED's. Lower => faster fade.
uint8_t bpm = 30;

int target[NUM_LEDS];  // Array to store target brightness values.
int count;
int delta;
int temp;
int highCutoff = 300;
int lowCutoff = 30;

unsigned int ledLoc = 0; 

uint8_t juggleDots   = 4;          // Number of dots in use.
uint8_t juggleFade   = 2;          // How long should the trails be. Very low value = longer trails.
uint8_t juggleDiff   = 16;         // Incremental change in hue between each dot.
uint8_t juggleHue    = 0;          // Starting hue.
uint8_t juggleCurHue = 0;          // The current hue
uint8_t juggleSat    = 255;        // Saturation of the colour.
uint8_t juggleBright = 255;        // How bright should the LED/display be.
uint8_t juggleBeat   = 5;          // Higher = faster movement.

uint8_t startIndexPalette;
uint8_t colorIndexPalette;

int16_t position1 = NUM_LEDS * 2 / 3; 
int16_t position2 = NUM_LEDS / 3; 
int16_t position3 = 0;             

long colorPalletMoving[] = {CRGB::Red,CRGB::Green,CRGB::Blue,CRGB::Teal,CRGB::DarkViolet,CRGB::Gold,CRGB::DeepPink,CRGB::Teal,CRGB::Yellow};  

static int16_t dist;                                          // A random number for our noise generator.
uint16_t xscale = 30;                                         // Wouldn't recommend changing this on the fly, or the animation will be really blocky.
uint16_t yscale = 30;                                         // Wouldn't recommend changing this on the fly, or the animation will be really blocky.
uint8_t maxChanges = 24;   


uint8_t thisbeatSnowfall =  23;                                       // Beats per minute for first part of dot.
uint8_t thatbeatSnowfall =  28;                                       // Combined the above with this one.
uint8_t thisfadeSnowfall =  32;                                       // How quickly does it fade? Lower = slower fade rate.
uint8_t  thissatSnowfall = 255;                                       // The saturation, where 255 = brilliant colours.
uint8_t  thisbriSnowfall = 255;                                       // Brightness of a sequence.
int        myhueSnowfall =   0;

uint16_t frame = 0;  

int targetChase[NUM_LEDS];  // Place to save some targetChase values.
int countChase;
int deltaChase;
int tempChase;
int lowCutoffChase = 50;

byte VolcanoAnim = 0;

// ******************
byte Second = 0;
byte Minute = 0;
byte lastSecond;
bool timerStart = false;

byte animationSequence = 0;

String serialBuffer = "";         // a String to hold incoming data
bool bufferComplete = false;  // whether the string is complete
bool currentLineIsBlank = true;
bool eepromControl = true;

void setup(){
 Serial.begin(9600);
 serialBuffer.reserve(64);   

 FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
 FastLED.setBrightness(255);
 FastLED.setMaxPowerInVoltsAndMilliamps(5,500);    // FastLED Power management set at 5V, 500mA.  
 FastLED.clear();

 dist = random16(12345);   // A semi-random number for our noise generator

 noInterrupts();      // disable all interrupts  // Timer1 kesmesi ayarlanıyor
 TCNT1  = 0;          // initialize counter value to 0
 TCCR1A = 0;          // same for TCCR1B
 TCCR1B = 0;          // set entire TCCR1A register to 0
 OCR1A = 15624;       // set compare match register for 1hz increments // = (16*10^6) / (1*1024) - 1 ( must be < 65536 )
 TCCR1B |= (1 << WGM12);               // turn on CTC mode   
 TCCR1B |= (1 << CS12) | (1 << CS10);  // Set CS10 and CS12 bits for 1024 prescaler
 TIMSK1 |= (1 << OCIE1A);              // enable timer compare interrupt
 interrupts();        // enable
 
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
      }else if (inChar != '\r') {
         currentLineIsBlank = false;
      }
      if(inChar == '\n' && currentLineIsBlank){
         bufferComplete = true;
      }
   }

  if(bufferComplete == true){ 
     //Serial.println(serialBuffer);
      if(serialBuffer.indexOf("0") > -1){ 
         Second=0; Minute=0;
         timerStart = false;
         animationSequence = 0; 
         EEPROM.update(0,animationSequence);   
         EEPROM.update(1,160); 
      }     
      if(serialBuffer.indexOf("1") > -1){ 
         Second=0; Minute=0;
         timerStart = false;
         animationSequence = 1; 
         EEPROM.update(0,animationSequence);   
         EEPROM.update(1,160);         
      }
      if(serialBuffer.indexOf("2") > -1){ 
         Second=0; Minute=0;
         timerStart = false;
         animationSequence = 2;
         EEPROM.update(0,animationSequence);   
         EEPROM.update(1,160);        
      }
      if(serialBuffer.indexOf("3") > -1){ 
         Second=0; Minute=0;
         timerStart = false;
         animationSequence = 3;
         EEPROM.update(0,animationSequence);   
         EEPROM.update(1,160);         
      }  
      
      /*
       other colors codes 
      */
          
      if(serialBuffer.indexOf("21") > -1){ 
         Colors(CRGB::Black);
         Second=0; Minute=0;         
         timerStart = false;
         animationSequence = 21;
         EEPROM.update(0,animationSequence);   
         EEPROM.update(1,160);          
         
      }       
      if(serialBuffer.indexOf("22") > -1){ 
         Colors(CRGB::Black);
         Second=0; Minute=0;
         timerStart = false;
         animationSequence = 22;
         EEPROM.update(0,animationSequence);   
         EEPROM.update(1,160); 
      } 
      if(serialBuffer.indexOf("23") > -1){ 
         Colors(CRGB::Black);
         Second=0; Minute=0;
         timerStart = false;
         animationSequence = 23;
         EEPROM.update(0,animationSequence);   
         EEPROM.update(1,160); 
      } 
      if(serialBuffer.indexOf("24") > -1){ 
         Colors(CRGB::Black);
         Second=0; Minute=0;
         timerStart = false;
         animationSequence = 24;
         EEPROM.update(0,animationSequence);   
         EEPROM.update(1,160); 
      } 

      if(serialBuffer.indexOf("25") > -1){ 
         Colors(CRGB::Black);
         Second=0; Minute=0;
         timerStart = true;
         animationSequence = 25;
         EEPROM.update(0,animationSequence);   
         EEPROM.update(1,160); 
      } 
      
      if(serialBuffer.indexOf("26") > -1){ 
         Colors(CRGB::Black);
         Second=0; Minute=0;
         timerStart = true;
         animationSequence = 26;
         EEPROM.update(0,animationSequence);   
         EEPROM.update(1,160);         
      } 
      
      if(serialBuffer.indexOf("27") > -1){ 
         Colors(CRGB::Black);
         Second=0; Minute=0;
         timerStart = false;
         animationSequence = 27;
         EEPROM.update(0,animationSequence);   
         EEPROM.update(1,160); 
      } 
      
      if(serialBuffer.indexOf("28") > -1){ 
         Colors(CRGB::Black);
         Second=0; Minute=0;
         timerStart = false;
         animationSequence = 28;
         EEPROM.update(0,animationSequence);   
         EEPROM.update(1,160); 
      } 
      
      if(serialBuffer.indexOf("29") > -1){ 
         Colors(CRGB::Black);
         Second=0; Minute=0;
         timerStart = true;
         animationSequence = 29;
         EEPROM.update(0,animationSequence);   
         EEPROM.update(1,160); 
      } 

      if(serialBuffer.indexOf("30") > -1){ 
         Colors(CRGB::Black);
         Second=0; Minute=0;
         timerStart = false;
         animationSequence = 30;
         EEPROM.update(0,animationSequence);   
         EEPROM.update(1,160); 
      } 

      if(serialBuffer.indexOf("31") > -1){ 
         Colors(CRGB::Black);
         Second=0; Minute=0;
         timerStart = false;
         animationSequence = 31;
         EEPROM.update(0,animationSequence);   
         EEPROM.update(1,160); 
      } 
      
      if(serialBuffer.indexOf("32") > -1){ 
         Colors(CRGB::Black);
         Second=0; Minute=0;
         timerStart = false;
         animationSequence = 32;
         EEPROM.update(0,animationSequence);   
         EEPROM.update(1,160); 
      } 

      if(serialBuffer.indexOf("33") > -1){ 
         Colors(CRGB::Black);
         Second=0; Minute=0;
         timerStart = false;
         animationSequence = 33;
         EEPROM.update(0,animationSequence);   
         EEPROM.update(1,160); 
      } 

      if(serialBuffer.indexOf("34") > -1){ 
         Colors(CRGB::Black);
         Second=0; Minute=0;
         timerStart = false;
         animationSequence = 34;
         EEPROM.update(0,animationSequence);   
         EEPROM.update(1,160); 
      } 

      if(serialBuffer.indexOf("35") > -1){ 
         Colors(CRGB::Black);
         Second=0; Minute=0;
         timerStart = false;
         animationSequence = 35;
         EEPROM.update(0,animationSequence);   
         EEPROM.update(1,160); 
      } 
      
      if(serialBuffer.indexOf("36") > -1){ 
         Colors(CRGB::Black);
         Second=0; Minute=0;
         timerStart = false;
         animationSequence = 36;
         EEPROM.update(0,animationSequence);   
         EEPROM.update(1,160); 
      } 

      if(serialBuffer.indexOf("37") > -1){ 
         Colors(CRGB::Black);
         Second=0; Minute=0;
         timerStart = false;
         animationSequence = 37;
         EEPROM.update(0,animationSequence);   
         EEPROM.update(1,160); 
      }    

      if(serialBuffer.indexOf("38") > -1){ 
         Colors(CRGB::Black);
         Second=0; Minute=0;
         timerStart = false;
         animationSequence = 38;
         EEPROM.update(0,animationSequence);   
         EEPROM.update(1,160); 
      } 

      if(serialBuffer.indexOf("39") > -1){ 
         Colors(CRGB::Black);
         Second=0; Minute=0;
         timerStart = true;
         animationSequence = 39;
         EEPROM.update(0,animationSequence);   
         EEPROM.update(1,160); 
      }

      if(serialBuffer.indexOf("40") > -1){ 
         Colors(CRGB::Black);
         Second=0; Minute=0;
         timerStart = false;
         animationSequence = 40;
         EEPROM.update(0,animationSequence);   
         EEPROM.update(1,160); 
      } 

      if(serialBuffer.indexOf("41") > -1){ 
         Colors(CRGB::Black);
         Second=0; Minute=0;
         timerStart = true;
         animationSequence = 41;
         EEPROM.update(0,animationSequence);   
         EEPROM.update(1,160); 
      }
      
      if(serialBuffer.indexOf("42") > -1){ 
         Colors(CRGB::Black);
         Second=0; Minute=0;
         timerStart = false;
         animationSequence = 42;
         EEPROM.update(0,animationSequence);   
         EEPROM.update(1,160); 
      } 

      if(serialBuffer.indexOf("43") > -1){ 
         Colors(CRGB::Black);
         Second=0; Minute=0;
         timerStart = false;
         animationSequence = 43;
         EEPROM.update(0,animationSequence);   
         EEPROM.update(1,160);    
      }
      
      if(serialBuffer.indexOf("44") > -1){ 
         Colors(CRGB::Black);
         Second=0; Minute=0;
         timerStart = false;
         animationSequence = 44;
         EEPROM.update(0,animationSequence);   
         EEPROM.update(1,160);   
      }

      if(serialBuffer.indexOf("45") > -1){ 
         Colors(CRGB::Black);
         Second=0; Minute=0;
         timerStart = true;
         animationSequence = 45;
         EEPROM.update(0,animationSequence);   
         EEPROM.update(1,160);   
      }
      
      if(serialBuffer.indexOf("46") > -1){ 
         Colors(CRGB::Black);
         Second=0; Minute=0;
         timerStart = false;
         animationSequence = 46;
         EEPROM.update(0,animationSequence);   
         EEPROM.update(1,160);   
      }

      if(serialBuffer.indexOf("47") > -1){ 
         Colors(CRGB::Black);
         Second=0; Minute=0;
         timerStart = true;
         animationSequence = 47;
         EEPROM.update(0,animationSequence);   
         EEPROM.update(1,160);   
      }

      if(serialBuffer.indexOf("48") > -1){ 
         Colors(CRGB::Black);
         Second=0; Minute=0;
         timerStart = true;
         animationSequence = 48;
         EEPROM.update(0,animationSequence);   
         EEPROM.update(1,160);   
      }
      
      if(serialBuffer.indexOf("49") > -1){ 
         Colors(CRGB::Black);
         Second=0; Minute=0;
         timerStart = true;
         animationSequence = 49;
         EEPROM.update(0,animationSequence);   
         EEPROM.update(1,160);   
      }

      if(serialBuffer.indexOf("50") > -1){ 
         Colors(CRGB::Black);
         Second=0; Minute=0;
         timerStart = true;
         animationSequence = 50;
         EEPROM.update(0,animationSequence);   
         EEPROM.update(1,160);   
      }

      serialBuffer = "";   // clear the string
      bufferComplete = false;
  }


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

 break;
 case 5:

 break;
 case 6:

 break;
 case 7:

 break;
 case 8:

 break;
 case 9:

 break;
 case 10:

 break;
 case 11: 

 break;
 case 12:

 break;
 case 13:

 break;
 case 14:

 break;
 case 15: 

 break;
 case 16:

 break;
 case 17:

 break;
 case 18:

 break;
 case 19: 

 break;
 case 20:
 // Colors(CRGB::MediumVioletRed);
 break;
 case 21:  // APPLAUSE
  Applause();
 break;
 case 22:  // BEADS
  Beads();
 break; 
 case 23:  // 
  Beatwave();
 break;
 case 24:  //  CHESSBOARD 
  Chessboard();
 break; 
 case 25:  // CONFETTI
   if(Second >= 0 && Second <= 15){
      Confetti_Pink();
   }else if(Second >= 15 && Second <= 30){
      Confetti_Random();
   }else if(Second >= 30 && Second <= 45){
      Confetti_Green_Blue();
   }else if(Second >= 45 && Second <= 60){
      Confetti_Speckles();
   }
 break;
 case 26:  // CYLON   
   Cylon();  
 break;
 case 27: // DISCO-BALL
  Disco_Ball();
 break;
 case 28: // DOT-BEAT
  Dot_Beat();
 break;
 case 29:  // DROPS
  if(Second >= 0 && Second <= 15){
     Drops(160,0); 
  }else if(Second >= 15 && Second <= 30){
     Drops(240,128);
  }else if(Second >= 30 && Second <= 45){
     Drops(160,32);
  }else if(Second >= 45 && Second <= 60){
     Drops(240,64); 
  }
 break;
 case 30: // FADE-PALETTE
  Fade_Palette();
 break;
 case 31: // FLASH
  flash();
 break;
 case 32: // GRADIENT
  Gradient();
 break;
 case 33: // HEART-BEAT
  Heart_Beat();
 break;
 case 34: // HSV-COLORS
  HSV_Colors(); 
 break;
 case 35: // JUGGLE
  Juggle();
 break;
 case 36:  // LAV-WAVE
  Lava_Wave();
 break;
 case 37: // METEOR
  Meteor();
 break;
 case 38: // MOVER
  Mover(); 
 break;
 case 39: // MOVING-COLORS
  if(Second >= 0 && Second <= 20){
     Moving_Colors(0,1,2);
  }else if(Second >= 20 && Second <= 40){
     Moving_Colors(3,4,5);
  }else if(Second >= 40 && Second <= 60){
     Moving_Colors(6,7,8);
  }
 break;
 case 40: // NOISE
  Noise16();
 break;
 case 41: //  OCEAN
  if(Second >= 0 && Second <= 20){
     Ocean();
  }else if(Second >= 20 && Second <= 40){
     Ocean_Wave();
  }else if(Second >= 40 && Second <= 60){
     Ocean();
  }
 break;
 case 42: // OVERLAY
  Overlay();
 break;
 case 43: // CROSS-FADE
  PaletteCrossFade();
 break;
 case 44: // PARTY-WAVE
  Party_Wave();
 break;
 case 45: // RAINBOW
   if(Second >= 0 && Second <= 20){
      Rainbow_March(200, 10); FastLED.show();
   }else if(Second >= 20 && Second <= 40){
     static uint8_t startIndex = 0;
     startIndex = startIndex + 1; 
     RainbowColors( startIndex);
     FastLED.show(); FastLED.delay(10);
   }else if(Second >= 40 && Second <= 60){
     Rainbow_Wave();
   }  
 break;
 case 46: // SINELON
   Sinelon();
 break;
 case 47: // SNOWFALL
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

 Snowfall();
 break;
 case 48: // SPARKLE
   if(Second >= 0 && Second <= 10){
      Sparkle(CRGB::Red,50);
   }else if(Second >= 10 && Second <= 20){
      Sparkle(CRGB::Gold,50);
   }else if(Second >= 20 && Second <= 30){
      Sparkle(CRGB::HotPink,50);
   }else if(Second >= 30 && Second <= 40){
      Sparkle(CRGB::Cyan,50);  
   }else if(Second >= 40 && Second <= 50){
      Sparkle(CRGB::Yellow,50);
   }else if(Second >= 50 && Second <= 60){
      Sparkle(CRGB::DarkBlue,50); 
   }
 break;
 case 49: // VOLCANO  
   if(Second >= 0 && Second <= 20){
      if(VolcanoAnim == 0){
         Colors(CRGB::Black);
         VolcanoAnim = 1;
      }    
      Volcano_Red();
   }else if(Second >= 20 && Second <= 40){
      if(VolcanoAnim == 1){
         Colors(CRGB::Black);
         VolcanoAnim = 2;
      }       
      Volcano_Blue();
   }else if(Second >= 40 && Second <= 60){
      if(VolcanoAnim == 2){
         Colors(CRGB::Black);
         VolcanoAnim = 0;
      }       
      Volcano_Green();
   }
 break;
 case 50: // WIZARD
   if(Second >= 0 && Second <= 20){
      Wizard_A();
   }else if(Second >= 20 && Second <= 40){
      Wizard_B();
   }else if(Second >= 40 && Second <= 60){
      Wizard_C();
   }
 break;
   
} // switch()

  
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
// ----------  Functions ------------------------------------------
void Colors(CRGB predefinedColors){
 fill_solid( leds, NUM_LEDS, predefinedColors);
 FastLED.show();
}

void ColorsPixel(int Pixel,CRGB predefinedColors){
 leds[Pixel] = predefinedColors;
 FastLED.show();
}

void Applause(){   //originally by Mark K.
//Number of loops is increased based on number of pixels.
//Fade ammount drops as pixels increase.
static uint8_t appAmmount = (NUM_LEDS/32);
  for (uint8_t i=0; i<appAmmount; i++){
    static uint16_t lastPixel = 0;
    fadeToBlackBy( leds, NUM_LEDS, 32/appAmmount );  //was 32
    leds[lastPixel] = CHSV(random8(HUE_BLUE,HUE_PURPLE),255,255);
    lastPixel = random16(NUM_LEDS);
    leds[lastPixel] = CRGB::White;
    FastLED.delay(5);
  }
  FastLED.show();  
  FastLED.delay(50); 
  EVERY_N_MILLISECONDS( 80 ) { gHueDots++; } // slowly cycle the "base color" through the rainbow
}

void Beads(){
  const uint16_t spewSpeed = 100;  // rate of advance
  static boolean spewing = 0;      // pixels are On(1) or Off(0)
  static uint8_t count = 1;        // how many to light (or not light)
  static uint8_t temp = count;
  static uint8_t hue = random8();
  EVERY_N_MILLISECONDS(spewSpeed){
    if (count == 0) {
      spewing = !spewing;
      if (spewing == 1) { count = random8(2,5); }  // random number for On pixels
      else { count = random8(1,8); }  // random number for Off pixels
      temp = count;
      //gHueDots = gHueDots - 30;
      hue = random8();
    }
    for (uint8_t i = NUM_LEDS-1; i > 0; i--) {
      leds[i] = leds[i-1];  // shift data down the line by one pixel
    }
    if (spewing == 1) {  // new pixels are On
      if (temp == count) {
        leds[0] = CHSV(hue-5, 215, 255);  // for first dot
        //leds[0] = CHSV(gHueDots-5, 215, 255);  // for first dot
      } else {
        leds[0] = CHSV(hue, 255, 255/(1+((temp-count)*2)) );  // for following dots
        //leds[0] = CHSV(gHueDots, 255, 255/(1+((temp-count)*2)) );  // for following dots
      }
    } else {  // new pixels are Off
      leds[0] = CHSV(0,0,0);  // set pixel 0 to black
    }
    count = count - 1;  // reduce count by one.
  }//end every_n
  FastLED.show();  
  FastLED.delay(8); 
  EVERY_N_MILLISECONDS( 20 ) { gHueDots++; } // slowly cycle the "base color" through the rainbow
}

void Beatwave(){
  currentPalette = RainbowColors_p;
  uint8_t wave1 = beatsin8(9, 0, 255);  // That's the same as beatsin8(9);
  uint8_t wave2 = beatsin8(8, 0, 255);
  uint8_t wave3 = beatsin8(7, 0, 255);
  uint8_t wave4 = beatsin8(6, 0, 255);

  for (int i=0; i<NUM_LEDS; i++) {
    leds[i] = ColorFromPalette( currentPalette, i+wave1+wave2+wave3+wave4, 255, currentBlending); 
  }
  FastLED.show();
}


void Confetti_Pink(){                                         // random colored speckles that blink in and fade smoothly
  uint8_t secondHand = (millis() / 1000) % 15;                // IMPORTANT!!! Change '15' to a different value to change duration of the loop.
  static uint8_t lastSecond = 99;                             // Static variable, means it's only defined once. This is our 'debounce' variable.
  if (lastSecond != secondHand) {                             // Debounce to make sure we're not repeating an assignment.
    lastSecond = secondHand;
    switch(secondHand) {
      case  0: confettiInc=1; confettiHue=192; confettiSat=255; confettiFade=2; confettihuediff=256; break;  // You can change values here, one at a time , or altogether.
      case  5: confettiInc=2; confettiHue=128; confettiFade=8; confettihuediff=64; break;
      case 10: confettiInc=1; confettiHue=random16(255); confettiFade=1; confettihuediff=16; break;      // Only gets called once, and not continuously for the next several seconds. Therefore, no rainbows.
      case 15: break;                                                                // Here's the matching 15 for the other one.
    }
  }                                               // Check the demo loop for changes to the variables.

  EVERY_N_MILLISECONDS(5){       // FastLED based non-blocking delay to update/display the sequence.
    fadeToBlackBy(leds, NUM_LEDS, confettiFade);                    // Low values = slower fade.
    int pos = random16(NUM_LEDS);                               // Pick an LED at random.
    leds[pos] += CHSV((confettiHue + random16(confettihuediff))/4 , confettiSat, confettiBri);  // I use 12 bits for hue so that the hue increment isn't too quick.
    confettiHue = confettiHue + confettiInc;  
  }
  FastLED.show();
}

void Confetti_Random(){
  fadeToBlackBy( leds, NUM_LEDS, 20);
  int pos = random16(NUM_LEDS);
  leds[pos] += CHSV( gHueDots + random8(64), random8(128,200), random8(48,255));
  FastLED.show();  
  FastLED.delay(8); 
  EVERY_N_MILLISECONDS( 20 ) { gHueDots++; } // slowly cycle the "base color" through the rainbow  
}

void Confetti_Green_Blue(){
  uint8_t p = 25;  // What percentage of the time to make speckles.  [Range 0-100]
  fadeToBlackBy( leds, NUM_LEDS, 10);
  if (random8(100) < p) {
    int pos = random16(NUM_LEDS);
    uint8_t hue = random8(2);  // randomly chooses a 0 or 1
    if (hue == 0) {
      hue = random8(92,111);  // pick a hue somewhere in the green zone
    } else {
      hue = random8(156,165);  // pick a hue somewhere in the blue zone
    }
    leds[pos] += CHSV( hue, random8(200,240), 255);
  }
  FastLED.show();  
  FastLED.delay(8); 
  EVERY_N_MILLISECONDS( 20 ) { gHueDots++; } // slowly cycle the "base color" through the rainbow  
}

void Confetti_Speckles(){   // Random colored speckles that blink in and fade smoothly.
  fadeToBlackBy(leds, NUM_LEDS, 10);
  int pos = random16(NUM_LEDS);
  leds[pos] += CHSV(gHueDots + random8(64), 200, 255);
  FastLED.show();  
  FastLED.delay(8); 
  EVERY_N_MILLISECONDS( 20 ) { gHueDots++; } // slowly cycle the "base color" through the rainbow  
}

void Cylon(){ 
  for(int i = 0; i < NUM_LEDS; i++){
    leds[i] = CHSV(hueCylon++, 255, 255);
    FastLED.show(); 
    for(int i = 0; i < NUM_LEDS; i++){
     leds[i].nscale8(250);
    }
    FastLED.delay(5);
  }

  for(int i = (NUM_LEDS)-1; i >= 0; i--){
    leds[i] = CHSV(hueCylon++, 255, 255);
    FastLED.show();
    for(int i = 0; i < NUM_LEDS; i++){
     leds[i].nscale8(250);
    }
    FastLED.delay(5);
  }
}

void Cylon_Colors(CRGB streakcolor){
 for(int i = 0; i < NUM_LEDS; i++){
  leds[i] = streakcolor;
  FastLED.show();
  for(int i = 0; i < NUM_LEDS; i++){
     leds[i].nscale8(250);
  }
  FastLED.delay(5);
 }
 for(int i = (NUM_LEDS) - 1; i >= 0; i--){
  leds[i] = streakcolor;
  FastLED.show();
  for(int i = 0; i < NUM_LEDS; i++){
     leds[i].nscale8(250);
  }
  FastLED.delay(5);
 }
}



void Chessboard(){
  static int16_t pos = 0;   // position along strip
  static int8_t delta = 3;  // delta (can be negative, and/or odd numbers)
  static uint8_t hue = 0;   // hue to display
  EVERY_N_MILLISECONDS(50){
    leds[pos] = CHSV(hue,255,255);
    pos = (pos + delta + NUM_LEDS) % NUM_LEDS;
    if (delta >= 0 && pos == 0) {  //going forward
      hue = hue + random8(42,128);
    }
    if (delta < 0 && pos == NUM_LEDS-1) {  //going backward
      hue = hue + random8(42,128);
    }
  }
  FastLED.show();  
  FastLED.delay(8); 
  EVERY_N_MILLISECONDS( 20 ) { gHueDots++; }   
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

void Dot_Beat(){
  uint8_t inner = beatsin8(bpm, NUM_LEDS/4, NUM_LEDS/4*3);    // Move 1/4 to 3/4
  uint8_t outer = beatsin8(bpm, 0, NUM_LEDS-1);               // Move entire length
  uint8_t middle = beatsin8(bpm, NUM_LEDS/3, NUM_LEDS/3*2);   // Move 1/3 to 2/3
  leds[middle] = CRGB::Purple;
  leds[inner] = CRGB::Blue;
  leds[outer] = CRGB::Yellow;
  nscale8(leds,NUM_LEDS,fadevalBeat);                         // Fade the entire array. Or for just a few LED's, use  nscale8(&leds[2], 5, fadevalBeat);
  FastLED.show();
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

void Fade_Palette(){
  currentPalette = PartyColors_p;
  EVERY_N_MILLISECONDS(100){                                                 // FastLED based non-blocking FIXED delay.
    uint8_t maxChanges = 24;
    nblendPaletteTowardPalette(currentPalette, targetPalette, maxChanges);    // AWESOME palette blending capability.
  }
  EVERY_N_SECONDS(5){                                                        // Change the target palette to a random one every 5 seconds.
    uint8_t baseC = random8(255);                                             // Use the built-in random number generator as we are re-initializing the FastLED one.
    targetPalette = CRGBPalette16(CHSV(baseC+random8(0,32), 255, random8(128, 255)), CHSV(baseC+random8(0,32), 255, random8(128, 255)), CHSV(baseC+random8(0,32), 192, random8(128, 255)), CHSV(baseC+random8(0,32), 255, random8(128, 255)));
  }
  random16_set_seed(535);                                                           // The randomizer needs to be re-set each time through the loop in order for the 'random' numbers to be the same each time through.
  for (int i = 0; i<NUM_LEDS; i++) {
    uint8_t fader = sin8(millis()/random8(10,20));                                  // The random number for each 'i' will be the same every time.
    leds[i] = ColorFromPalette(currentPalette, i*20, fader, currentBlending);       // Now, let's run it through the palette lookup.
  }
  random16_set_seed(millis()); 
  FastLED.show();
}

void inBrightness(){
 for(int i = 0; i < 255; i++){ 
     FastLED.setBrightness(i);
     FastLED.delay(5);
 }
}

void flash(){
  currentPalette = PartyColors_p;
  EVERY_N_MILLIS_I(thisTimer,100) {                           // This only sets the Initial timer delay. To change this value, you need to use thisTimer.setPeriod(); You could also call it thatTimer and so on.
    uint8_t timeval = beatsin8(10,5,100);                     // Create/modify a variable based on the beastsin8() function.
    thisTimer.setPeriod(timeval);                             // Use that as our update timer value.
    ledLoc = (ledLoc+1) % (NUM_LEDS-1);                       // A simple routine to just move the active LED UP the strip.
    leds[ledLoc] = ColorFromPalette(currentPalette, ledLoc, 255, currentBlending);     // Pick a slightly rotating colour from the Palette
  }
  fadeToBlackBy(leds, NUM_LEDS, 8);                           // Leave a nice comet trail behind.
  FastLED.show();
} 

void Gradient(){
 uint8_t starthue = beatsin8(5, 0, 255);
 uint8_t endhue = beatsin8(7, 0, 255);
 if(starthue < endhue){
    fill_gradient(leds, NUM_LEDS, CHSV(starthue,255,255), CHSV(endhue,255,255), FORWARD_HUES);    // If we don't have this, the colour fill will flip around. 
 }else{
    fill_gradient(leds, NUM_LEDS, CHSV(starthue,255,255), CHSV(endhue,255,255), BACKWARD_HUES);
 }
 FastLED.show();  
}

void Heart_Beat(){  
  uint8_t BeatsPerMinute = 62;
  CRGBPalette16 palette = PartyColors_p;
  uint8_t beat = beatsin8(BeatsPerMinute, 64, 255);
  
  for(int i = 0; i < NUM_LEDS; i++) { //9948
    leds[i] = ColorFromPalette(palette, gHueDots+(i*2), beat-gHueDots+(i*10));
  }
  EVERY_N_MILLISECONDS(20) { gHueDots++; }
  FastLED.show();
}

void HSV_Colors(){
  EVERY_N_SECONDS(2){
    for(uint8_t i=0; i < NUM_LEDS; i++){
      hsv[i].h = random8();
      hsv[i].s = random8(200,256);
      hsv[i].v = random8(64,85);
      leds[i]  = CHSV(hsv[i].h, hsv[i].s, hsv[i].v);
    }
    FastLED.show();  
  }
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

void Lava_Wave(){
  FastLED.setBrightness(100);
  EVERY_N_MILLISECONDS(10){
    colorIndexPalette = startIndexPalette;
    for( int i = 0; i < NUM_LEDS; i++){
              //ColorFromPalette( paletteName, colorIndexPalette[0-255], brightness[0-255], blendType[NOBLEND or LINEARBLEND])
      leds[i] = ColorFromPalette( LavaColors_p, colorIndexPalette, 255, LINEARBLEND);
      colorIndexPalette = colorIndexPalette + 10;  //how fast to advance through palette
    }
    FastLED.show();
    startIndexPalette = startIndexPalette + 1;  //how fast to move the palette down the strip
  }
}

void Meteor(){
  static uint8_t pos;  //used to keep track of position
  EVERY_N_MILLISECONDS(70){
    fadeToBlackBy( leds, NUM_LEDS, 200);  //fade all the pixels some
    leds[pos] = CHSV(gHueDots, random8(170,230), 255);
    leds[(pos+5) % NUM_LEDS] = CHSV(gHueDots+64, random8(170,230), 255);
    pos = pos + 1;  //advance position
    
    //This following check is very important.  Do not go past the last pixel!
    if (pos == NUM_LEDS) { pos = 0; }  //reset to beginning
    //Trying to write data to non-existent pixels causes bad things.
  }
  FastLED.show();  
  FastLED.delay(5); 
  EVERY_N_MILLISECONDS( 15 ) { gHueDots++; } 
}

void Mover(){
 FastLED.setBrightness(128);
 EVERY_N_MILLISECONDS(100){
    uint8_t maxChanges = 24; 
    nblendPaletteTowardPalette(currentPaletteMover, targetPaletteMover, maxChanges);   // AWESOME palette blending capability.
 }

 EVERY_N_SECONDS(5){                      // Change the target palette to a random one every 5 seconds.
    static uint8_t baseC = random8();     // You can use this as a baseline colour if you want similar hues in the next line.
    targetPaletteMover = CRGBPalette16(CHSV(random8(), 255, random8(128,255)), CHSV(random8(), 255, random8(128,255)), CHSV(random8(), 192, random8(128,255)), CHSV(random8(), 255, random8(128,255)));
 }

 for(int i = 0; i < NUM_LEDS; i++){
     uint8_t red = (millis() / 5) + (i * 12);   // speed, length
     if (red > 128) red = 0;
     leds[i] = ColorFromPalette(currentPaletteMover, red, red, currentBlending);
 }                                              // render the first animation into leds2   
 FastLED.show();
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


void Noise16(){   // "lavalamp effect"
  
  currentPalette = PartyColors_p;
  
  EVERY_N_MILLISECONDS(50){
    uint8_t maxChanges = 24;      // Number of tweens between palettes.
    nblendPaletteTowardPalette(currentPalette, targetPalette, maxChanges);  // Blend towards the target palette
  }

  EVERY_N_SECONDS(5) {             // Change the target palette to a random one every 5 seconds.
    targetPalette = CRGBPalette16(CHSV(random8(), 255, random8(128,255)), CHSV(random8(), 255, random8(128,255)), CHSV(random8(), 192, random8(128,255)), CHSV(random8(), 255, random8(128,255)));
  }
  
  uint8_t scale = 1000;                                       // the "zoom factor" for the noise

  for(uint16_t i = 0; i < NUM_LEDS; i++){

    uint16_t shift_x = millis() / 10;                         // x as a function of time
    uint16_t shift_y = 0;
    uint32_t real_x = (i + shift_x) * scale;                  // calculate the coordinates within the noise field
    uint32_t real_y = (i + shift_y) * scale;                  // based on the precalculated positions
    uint32_t real_z = 4223;
    uint8_t noise = inoise16(real_x, real_y, real_z) >> 8;    // get the noise data and scale it down
    uint8_t index = sin8(noise*3);                            // map led color based on noise data
    uint8_t bri   = noise;

    leds[i] = ColorFromPalette(currentPalette, index, bri, LINEARBLEND);   // With that value, look up the 8 bit colour palette value and assign it to the current LED.

  }
  FastLED.show();
} 

void Ocean_Wave(){
  FastLED.setBrightness(100);
  EVERY_N_MILLISECONDS(10){
    colorIndexPalette = startIndexPalette;
    for( int i = 0; i < NUM_LEDS; i++){
              //ColorFromPalette( paletteName, colorIndexPalette[0-255], brightness[0-255], blendType[NOBLEND or LINEARBLEND])
      leds[i] = ColorFromPalette( OceanColors_p, colorIndexPalette, 255, LINEARBLEND);
      colorIndexPalette = colorIndexPalette + 10;  //how fast to advance through palette
    }
    FastLED.show();
    startIndexPalette = startIndexPalette + 1;  //how fast to move the palette down the strip
  }
}

void Overlay(){
  static uint8_t gHue = 0; //slowly rotating hue color
  EVERY_N_MILLISECONDS( 30 ) { gHue++; }  //slowly cycle through the rainbow
  fill_rainbow( base, NUM_LEDS, gHue, 255/NUM_LEDS/2);
  fadeToBlackBy( base, NUM_LEDS, 220);  //dim the rainbow down
  
  EVERY_N_MILLISECONDS(60){
   static uint8_t pos = 0;
   overlay[pos] = overlay[NUM_LEDS-1-pos] = CRGB::Pink;
   fadeToBlackBy( overlay, NUM_LEDS, 120);  //creates fading tail effect
   pos++;
   if(pos == NUM_LEDS/2) {  //check against desired range
      pos = 0;  //reset for next round
   }
  }
  for(uint8_t i=0; i<NUM_LEDS; i++){
    leds[i] = base[i];      //copy base to leds
    leds[i] += overlay[i];  //add overlay to leds
  }
  FastLED.show();     //show the pixels
}

void Ocean(){
  EVERY_N_MILLISECONDS(10){
    uint8_t maxChanges = 24; 
    nblendPaletteTowardPalette(paletteBlack, paletteOcean, maxChanges);   // AWESOME palette blending capability.
    for(int i = 0; i < NUM_LEDS; i++) {                                      // Just ONE loop to fill up the LED array as all of the pixels change.
      uint8_t index = inoise8(i*xscale, dist+i*yscale) % 255;                // Get a value from the noise function. I'm using both x and y axis.
      leds[i] = ColorFromPalette(paletteBlack, index, 255, LINEARBLEND);   // With that value, look up the 8 bit colour palette value and assign it to the current LED.
    }
    dist += beatsin8(10,1,4);                                                              // Update the LED array with noise at the new location
  }
  EVERY_N_SECONDS(5){                                        // Change the target palette to a random one every 5 seconds.
    paletteOcean = CRGBPalette16(CHSV(random8(), 255, random8(128,255)), CHSV(random8(), 255, random8(128,255)), CHSV(random8(), 192, random8(128,255)), CHSV(random8(), 255, random8(128,255)));
  }
  FastLED.show();
} 


void PaletteCrossFade(){
  uint8_t secondHand = (millis() / 1000) % 60;
  static uint8_t lastSecond = 99;
  
  if(lastSecond != secondHand){
    lastSecond = secondHand;
    CRGB p = CHSV(HUE_PURPLE, 255, 255);
    CRGB g = CHSV(HUE_GREEN, 255, 255);
    CRGB b = CRGB::Black;
    CRGB w = CRGB::White;
    if(secondHand ==  0)  { targetPalette = RainbowColors_p; }
    if(secondHand == 10)  { targetPalette = CRGBPalette16(g,g,b,b, p,p,b,b, g,g,b,b, p,p,b,b); }
    if(secondHand == 20)  { targetPalette = CRGBPalette16(b,b,b,w, b,b,b,w, b,b,b,w, b,b,b,w); }
    if(secondHand == 30)  { targetPalette = LavaColors_p; }
    if(secondHand == 40)  { targetPalette = CloudColors_p; }
    if(secondHand == 50)  { targetPalette = PartyColors_p; }
  }
  EVERY_N_MILLISECONDS(100){
   uint8_t maxChanges = 24; 
   nblendPaletteTowardPalette(currentPaletteBlack, targetPalette, maxChanges);
  }
  EVERY_N_MILLISECONDS(20){
   static uint8_t startIndex = 0;
   startIndex += 1;                                 // motion speed
   FillLEDsFromPaletteColors(startIndex);
  }
  FastLED.show();
}

void FillLEDsFromPaletteColors(uint8_t colorIndex){
  for(int i = 0; i < NUM_LEDS; i++) {
    leds[i] = ColorFromPalette(currentPaletteBlack, colorIndex + sin8(i*16), 255);
    colorIndex += 3;
  }
}

void Party_Wave(){
  FastLED.setBrightness(100);
  EVERY_N_MILLISECONDS(10){
    colorIndexPalette = startIndexPalette;
    for( int i = 0; i < NUM_LEDS; i++){
              //ColorFromPalette( paletteName, colorIndexPalette[0-255], brightness[0-255], blendType[NOBLEND or LINEARBLEND])
      leds[i] = ColorFromPalette( PartyColors_p, colorIndexPalette, 255, LINEARBLEND);
      colorIndexPalette = colorIndexPalette + 10;  //how fast to advance through palette
    }
    FastLED.show();
    startIndexPalette = startIndexPalette + 1;  //how fast to move the palette down the strip
  }
}

void Rainbow_March(uint8_t thisdelay, uint8_t deltahue){     // The fill_rainbow call doesn't support brightness levels.
 uint8_t thishue = millis() * (255-thisdelay)/255;             // To change the rate, add a beat or something to the result. 'thisdelay' must be a fixed value.
 thishue = beat8(50);                                       // This uses a FastLED sawtooth generator. Again, the '50' should not change on the fly.
 //thishue = beatsin8(50,0,255);                              // This can change speeds on the fly. You can also add these to each other.
  fill_rainbow(leds, NUM_LEDS, thishue, deltahue);            // Use FastLED's fill_rainbow routine.
}

void RainbowColors( uint8_t colorIndex){
 for(int i = 0; i < NUM_LEDS; i++){
     leds[i] = ColorFromPalette( RainbowColors_p, colorIndex, 255, LINEARBLEND);
     colorIndex += 3;
 }
}

void Rainbow_Wave(){
  FastLED.setBrightness(100);
  EVERY_N_MILLISECONDS(10){
    colorIndexPalette = startIndexPalette;
    for( int i = 0; i < NUM_LEDS; i++){
              //ColorFromPalette( paletteName, colorIndexPalette[0-255], brightness[0-255], blendType[NOBLEND or LINEARBLEND])
      leds[i] = ColorFromPalette( RainbowColors_p, colorIndexPalette, 255, LINEARBLEND);
      colorIndexPalette = colorIndexPalette + 10;  //how fast to advance through palette
    }
    FastLED.show();
    startIndexPalette = startIndexPalette + 1;  //how fast to move the palette down the strip
  }
}

void Sinelon(){
  // a colored dot sweeping back and forth, with fading trails
  fadeToBlackBy( leds, NUM_LEDS, 12);
  int pos = beatsin16( 13, 0, NUM_LEDS-1 );
  leds[pos] += CHSV( gHueDots, 255, 192);
  FastLED.show();  
  FastLED.delay(8); 
  EVERY_N_MILLISECONDS( 20 ) { gHueDots++; } 
}

void Snowfall(){                                              // a colored dot sweeping back and forth, with fading trails
  EVERY_N_MILLISECONDS(100){
    uint8_t maxChanges = 24; 
    nblendPaletteTowardPalette(currentPalette, targetPalette, maxChanges);   // AWESOME palette blending capability.
  }
  EVERY_N_MILLISECONDS(50){                           // FastLED based non-blocking delay to update/display the sequence.
    fadeToBlackBy( leds, NUM_LEDS, thisfadeSnowfall);
    int pos1 = beatsin16(thisbeatSnowfall,0,NUM_LEDS);
    int pos2 = beatsin16(thatbeatSnowfall,0,NUM_LEDS);
    leds[(pos1+pos2)/2] += ColorFromPalette(currentPalette, myhueSnowfall++, thisbriSnowfall, currentBlending);
  }
  FastLED.show();
}

void Sparkle(CRGB predefinedColors,int SpeedDelay) {
  int Pixel = random(NUM_LEDS);
  ColorsPixel(Pixel,predefinedColors);
  FastLED.show();
  FastLED.delay(SpeedDelay);
  ColorsPixel(Pixel,CRGB::Black);
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


void Wizard_A(){
  // four colored dots, weaving in and out of sync with each other
  fadeToBlackBy( leds, NUM_LEDS, 20);
  byte dothue = 0;
  for( int i = 0; i < 4; i++) {
    leds[beatsin16( i+5, 0, NUM_LEDS-1 )] |= CHSV(dothue, 200, 255);
    dothue += 32;
  }
  FastLED.show();  
  FastLED.delay(8); 
  EVERY_N_MILLISECONDS( 20 ) { gHueDots++; }   
}

void Wizard_B(){
  // four colored dots, weaving in and out of sync with each other
  fadeToBlackBy( leds, NUM_LEDS, 20);
  byte dothue = 0;
  for( int i = 0; i < 8; i++) {
    leds[beatsin16( i+5, 0, NUM_LEDS-1 )] |= CHSV(dothue, 200, 255);
    dothue += 32;
  }
  FastLED.show();  
  FastLED.delay(8); 
  EVERY_N_MILLISECONDS( 20 ) { gHueDots++; }   
}


void Wizard_C(){
  // four colored dots, weaving in and out of sync with each other
  fadeToBlackBy( leds, NUM_LEDS, 20);
  byte dothue = 0;
  for( int i = 0; i < 12; i++) {
    leds[beatsin16( i+5, 0, NUM_LEDS-1 )] |= CHSV(dothue, 200, 255);
    dothue += 16;
  }
  FastLED.show();  
  FastLED.delay(8); 
  EVERY_N_MILLISECONDS( 20 ) { gHueDots++; }   
}
