/*
 Hello 
 I design developer tools for embedded electronic systems. You can buy my projects design files.
 https://www.altiumdesignerprojects.com
*/
word SecondInterval = 1000;
word secondPreviousMillis = 0;
word secondCurrentMillis;

byte Second;
byte lastSecond;

word counter = 0;

void setup() {
 Serial.begin(115200);
}
 
void loop() {
     
   if(Second != lastSecond){
      Serial.print("counter .... ");  Serial.println(counter);
   }
   lastSecond = Second; 

   secondCurrentMillis = millis();

   if(secondCurrentMillis - secondPreviousMillis >= SecondInterval){
      counter++;
      if(counter >= 150){
        Serial.println("break");
        counter = 0;
      }
      secondPreviousMillis = secondCurrentMillis;
   }
   

}
