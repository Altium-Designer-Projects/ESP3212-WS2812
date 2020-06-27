#include "SPIFFS.h"

File SPIFile;
 
void setup() {
  Serial.begin(115200);
  
  if(!SPIFFS.begin(true)){
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }
  
  SPIFile = SPIFFS.open("/text.txt");   //  jquery-3.4.1.js
  if(!SPIFile){
    Serial.println("Failed to open file for reading");
    return;
  }
  
  Serial.println("File Content:\r\n");
  while(SPIFile.available()){
    Serial.write(SPIFile.read());
  }
  SPIFile.close();
}
 
void loop() {

}
