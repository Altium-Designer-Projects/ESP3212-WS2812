#include <WiFi.h>
#include "ESPAsyncWebServer.h"
#include "SPIFFS.h"
/*
 Hello 
 I design developer tools for embedded electronic systems. You can buy my projects design files.
 https://www.altiumdesignerprojects.com
*/
const char* ssid     = "TurkTelekom_T44C6";     
const char* password = "vedat12345"; 

AsyncWebServer server(80);

void setup(){
  // Serial port for debugging purposes
  Serial.begin(115200);

  // Initialize SPIFFS
  if(!SPIFFS.begin(true)){
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  // Print ESP32 Local IP Address
  Serial.println(WiFi.localIP());

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/index.html", "text/html");
  });
  
  // Route to load style.css file
  server.on("/css/style.css", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/css/style.css", "text/css");
  });

  // Route to load style.css file
  server.on("/images/streetart.jpg", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/images/streetart.jpg", "image/jpeg");
  });

  // Route to load style.css file
  server.on("/images/streetart2.jpg", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/images/streetart2.jpg", "image/jpeg");
  });

  // Route to load style.css file
  server.on("/images/streetart5.jpg", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/images/streetart5.jpg", "image/jpeg");
  });

  // Route to load style.css file
  server.on("/images/streetart3.jpg", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/images/streetart3.jpg", "image/jpeg");
  });

  // Route to load style.css file
  server.on("/images/streetart4.jpg", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/images/streetart4.jpg", "image/jpeg");
  });
    
  // Start server
  server.begin();
}
 
void loop(){
  
}

/*
  .html    text/html
  .htm     text/html
  .css     text/css
  .js      application/javascript
  .png     image/png
  .gif     image/gif
  .jpg     image/jpeg
  .ico     image/x-icon
  .xml     text/xml
  .pdf     application/pdf
  .zip     application/zip
  
*/







  
