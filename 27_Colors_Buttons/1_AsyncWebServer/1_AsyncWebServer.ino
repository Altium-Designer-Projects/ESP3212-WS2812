#include "WiFi.h"
#include "ESPAsyncWebServer.h"
#include "SPIFFS.h"
/*
 Hello 
 I design developer tools for embedded electronic systems. You can buy my projects design files.
 https://www.altiumdesignerprojects.com
*/
const char* ssid     = "TP-LINK-MCU";     
const char* password = "15253545"; 

AsyncWebServer server(80);

void setup(){
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

  Serial.println(WiFi.localIP());  // Print ESP32 Local IP Address

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/index.html", "text/html");
  });
  
  // Route to load style.css file
  server.on("/css/bulma.min.css", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/css/bulma.min.css", "text/css");
  });

  // Route to load style.css file
  server.on("/css/colors.css", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/css/colors.css", "text/css");
  });

  server.on("/js/jquery-3.4.1.min.js", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/js/jquery-3.4.1.min.js", "application/javascript");
  });
 
  server.begin();   // Start server
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
