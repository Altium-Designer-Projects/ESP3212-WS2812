#include "SPIFFS.h"
 
void setup(){
  Serial.begin(115200);
 
  if (!SPIFFS.begin(true)) {
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }
 
  Serial.print("Used bytes before write:");
  Serial.println(SPIFFS.usedBytes());
 
  File file = SPIFFS.open("/test.txt", FILE_WRITE);
 
  if (!file) {
    Serial.println("There was an error opening the file for writing");
    return;
  }
  if (file.print("<!DOCTYPE html><html><title>HTML Tutorial</title><body><h1>This is a heading</h1><p>This is a paragraph.</p></body></html>")){
    Serial.println("File was written");
  } else {
    Serial.println("File write failed");
  }
 
  file.close();
 
  Serial.print("Used bytes after write:");
  Serial.println(SPIFFS.usedBytes());
}
 

void loop(){
  // put your main code here, to run repeatedly:

}
