#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
/*
 Hello 
 I design developer tools for embedded electronic systems. You can buy my projects design files.
 https://www.altiumdesignerprojects.com
*/
#include "index.h" // index page

WiFiClient client;
WebServer server(80);

const char* ssid     = "TP-LINK-MCU";     
const char* password = "15253545";  


void setup(void){
  Serial.begin(115200);
 
  Serial.println("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while(WiFi.status() != WL_CONNECTED){
   delay(100);
   Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected..!");
  Serial.print("Got IP: ");  Serial.println(WiFi.localIP());

  server.on("/",[](){
    server.send_P(200, "text/html", indexPage);  
  });

  server.on("/test", [](){ server.send(200, "text/html", "<H1>The server status is OK</H1>"); });
  server.begin();                  //Start server
}

void loop(void){
  server.handleClient();
}
