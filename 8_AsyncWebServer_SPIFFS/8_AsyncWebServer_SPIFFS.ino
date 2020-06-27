#include "WiFi.h"
#include "ESPAsyncWebServer.h"
#include "SPIFFS.h"
/*
 Hello 
 I design developer tools for embedded electronic systems. You can buy my projects design files.
 https://www.altiumdesignerprojects.com
*/

// https://bulma.io    Bulma is a free, open source CSS framework based
                       // on Flexbox and used by more than 200,000 developers.
const char* ssid     = "TP-LINK-MCU";     
const char* password = "15253545"; 

AsyncWebServer server(80);   // Create AsyncWebServer object on port 80

void setup(){
  Serial.begin(115200);

  // Initialize SPIFFS
  if(!SPIFFS.begin(true)){
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED){
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
  server.on("/css/bulma.min.css", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/css/bulma.min.css", "text/css");
  });
  
  server.begin();  // Start server
}
 
void loop(){
  
}
