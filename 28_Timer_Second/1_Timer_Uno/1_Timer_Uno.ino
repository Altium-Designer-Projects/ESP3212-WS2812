/*
 Hello 
 I design developer tools for embedded electronic systems. You can buy my projects design files.
 https://www.altiumdesignerprojects.com
*/
byte Second = 0;
byte Minute = 0;
byte lastSecond;
bool timerStart = false;

char rx_byte;

void setup(){
 Serial.begin(9600);

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
 
  if(Serial.available() > 0){       
    rx_byte = Serial.read();       
    if(rx_byte == '1'){
       timerStart = true; 
    }
    
    if(rx_byte == '2'){
       timerStart = false;
    }
    
    if(rx_byte == '3'){
       Second = 0;
    }
 }

  
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
