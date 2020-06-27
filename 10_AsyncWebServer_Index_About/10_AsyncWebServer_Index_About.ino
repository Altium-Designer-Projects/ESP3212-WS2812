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

AsyncWebServer server(80);       // Create AsyncWebServer object on port 80

void setup(){
  Serial.begin(115200);

  if(!SPIFFS.begin(true)){           // Initialize SPIFFS
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  Serial.println(WiFi.localIP());

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/index.html", "text,html");
  });

  server.on("/about.html", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/about.html", "text/html");
  });

  server.begin();
}
 
void loop(){
  
}

// 192.168.2.109/about.html
