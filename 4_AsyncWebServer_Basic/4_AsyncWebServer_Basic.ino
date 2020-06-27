#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
/*
 Hello 
 I design developer tools for embedded electronic systems. You can buy my projects design files.
 https://www.altiumdesignerprojects.com
*/
AsyncWebServer server(80);

const char* ssid     = "TurkTelekom_T44C6";     
const char* password = "vedat12345";  

const char* PARAM_MESSAGE = "message";

void notFound(AsyncWebServerRequest *request) {
    request->send(404, "text/plain", "Not found");
}

void setup(){

    Serial.begin(115200);
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    if (WiFi.waitForConnectResult() != WL_CONNECTED) {
        Serial.printf("WiFi Failed!\n");
        return;
    }

    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());

    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){ // sayfa açıldığında
        request->send(200, "text/plain", "Hello, world");
    });

    // type 192.168.2.109/get?message=hello world in the address bar  (your ip) 
    // Send a GET request to <IP>/get?message=<message>
    
    server.on("/get", HTTP_GET, [] (AsyncWebServerRequest *request){
        String message;
        if(request->hasParam(PARAM_MESSAGE)) {
           message = request->getParam(PARAM_MESSAGE)->value();
        }else{
           message = "No message sent";
        }
        request->send(200, "text/plain", "Hello, GET: " + message);
    });
    
    // Send a POST request to <IP>/post with a form field message set to <message>
    server.on("/post", HTTP_POST, [](AsyncWebServerRequest *request){
        String message;
        if (request->hasParam(PARAM_MESSAGE, true)) {
            message = request->getParam(PARAM_MESSAGE, true)->value();
        } else {
            message = "No message sent";
        }
        request->send(200, "text/plain", "Hello, POST: " + message);
    });

    server.onNotFound(notFound);

    server.begin();
}

void loop(){
  
}
