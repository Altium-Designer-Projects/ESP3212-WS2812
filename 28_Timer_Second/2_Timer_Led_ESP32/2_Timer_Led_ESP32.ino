/*
 Hello 
 I design developer tools for embedded electronic systems. You can buy my projects design files.
 https://www.altiumdesignerprojects.com
*/

hw_timer_t * timer = NULL;
volatile uint8_t ledstat = 0; 

void IRAM_ATTR onTimer(){
  
  ledstat = 1-ledstat;
  digitalWrite(2, ledstat);   // turn the LED on or off
  
}

void setup() {
  Serial.begin(115200);

  pinMode(2, OUTPUT);
  digitalWrite(2, LOW);    // turn the LED off by making the voltage LOW

  timer = timerBegin(1, 80, true);  // timer 0, MWDT clock period = 12.5 ns * TIMGn_Tx_WDT_CLK_PRESCALE -> 12.5 ns * 80 -> 1000 ns = 1 us, countUp
  timerAttachInterrupt(timer, &onTimer, true); // edge (not level) triggered 
  timerAlarmWrite(timer, 1000000, true); // 1000000 * 1 us = 1 second, autoreload true 
  timerAlarmEnable(timer); // enable
}

void loop(){



}
