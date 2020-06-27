
byte Second = 0;
byte Minute = 0;
byte lastSecond;
bool tStart = false;
/*
 Hello 
 I design developer tools for embedded electronic systems. You can buy my projects design files.
 https://www.altiumdesignerprojects.com
*/
char rx_byte;

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
    }
    
    if(rx_byte == '2'){
       tStart = false;
    }
    
    if(rx_byte == '3'){
       Second = 0;
    }
 }

}
