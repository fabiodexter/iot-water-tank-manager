#include <WebServer.h>
#include <Arduino.h>
#include <App.h>
#include "LittleFS.h"
#include <ESPAsyncWebServer.h>
#include <ESPAsyncTCP.h>
#include <DNSServer.h>
App *parent__;



// Create AsyncWebServer object on port 80
AsyncWebServer server(80);



IPAddress localIP;
IPAddress localGateway;
IPAddress subnet(255, 255, 0, 0);

// Search for parameter in HTTP POST request
const char *PARAM_INPUT_1 = "ssid";
const char *PARAM_INPUT_2 = "pass";
const char *PARAM_INPUT_4 = "gateway";

const char *PARAM_INPUT_5 = "mqtt_host";
const char *PARAM_INPUT_6 = "mqtt_port";
const char *PARAM_INPUT_7 = "mqtt_user";
const char *PARAM_INPUT_8 = "mqtt_pass";

const char *PARAM_INPUT_9 = "device_id";

// Timer variables
unsigned long previousMillis = 0;
const long interval = 10000; // interval to wait for Wi-Fi connection (milliseconds)

boolean restart = false;


// Write file to LittleFS
void writeFile(fs::FS &fs, const char *path, const char *message)
{
    Serial.printf("Writing file: %s\r\n", path);

    File file = fs.open(path, "w");
    if (!file)
    {
        Serial.println("- failed to open file for writing");
        return;
    }
    if (file.print(message))
    {
        Serial.println("- file written");
    }
    else
    {
        Serial.println("- frite failed");
    }
    file.close();
}


void processConfig(AsyncWebServerRequest *request){
          
          const char *device_idPath = "/device_id.txt";          
          const char *ssidPath = "/ssid.txt";
          const char *passPath = "/pass.txt";
          const char *gatewayPath = "/gateway.txt";
          const char *mqtt_hostPath = "/mqtt_host.txt";
          const char *mqtt_portPath = "/mqtt_port.txt";
          const char *mqtt_userPath = "/mqtt_user.txt";
          const char *mqtt_passPath = "/mqtt_pass.txt";

          int params = request->params();

          for(int i=0;i<params;i++){
            AsyncWebParameter* p = request->getParam(i);
            
            if(p->isPost()){
              // HTTP POST ssid value
              if (p->name() == PARAM_INPUT_1) {
                String _ssid = p->value().c_str();
                Serial.print(">> SSID set to: ");
                Serial.println(_ssid);
                // Write file to save value
                writeFile(LittleFS, ssidPath, _ssid.c_str());
              }
              
              // HTTP POST pass value
              if (p->name() == PARAM_INPUT_2) {
                String _pass = p->value().c_str();
                Serial.print(">> Password set to: ");
                Serial.println(_pass);
                // Write file to save value
                writeFile(LittleFS, passPath, _pass.c_str());
              }

              // HTTP POST gateway value
              if (p->name() == PARAM_INPUT_4) {
                String _gateway = p->value().c_str();
                Serial.print(">> Gateway set to: ");
                Serial.println(_gateway);
                // Write file to save value
                writeFile(LittleFS, gatewayPath, _gateway.c_str());
              }
              
              // HTTP POST mqtt_host value
              if (p->name() == PARAM_INPUT_5) {
                String _mqtt_host = p->value().c_str();
                Serial.print(">> mqtt_host set to: ");
                Serial.println(_mqtt_host);
                // Write file to save value
                writeFile(LittleFS, mqtt_hostPath, _mqtt_host.c_str());
              }

              // HTTP POST mqtt_port value
              if (p->name() == PARAM_INPUT_6) {
                String _mqtt_port = p->value().c_str();
                Serial.print(">> mqtt_port set to: ");
                Serial.println(_mqtt_port);
                // Write file to save value
                writeFile(LittleFS, mqtt_portPath, _mqtt_port.c_str());
              }
              
              // HTTP POST mqtt_user value
              if (p->name() == PARAM_INPUT_7) {
                String _mqtt_user = p->value().c_str();
                Serial.print(">> mqtt_user set to: ");
                Serial.println(_mqtt_user);
                // Write file to save value
                writeFile(LittleFS, mqtt_userPath, _mqtt_user.c_str());
              }
              
              // HTTP POST mqtt_pass value
              if (p->name() == PARAM_INPUT_8) {
                String _mqtt_pass = p->value().c_str();
                Serial.print(">> mqtt_pass set to: ");
                Serial.println(_mqtt_pass);
                // Write file to save value
                writeFile(LittleFS, mqtt_passPath, _mqtt_pass.c_str());
              }
              
              
              // HTTP POST device_id value
              if (p->name() == PARAM_INPUT_9) {
                String _device_id = p->value().c_str();
                Serial.print(">> device_id set to: ");
                Serial.println(_device_id);
                // Write file to save value
                writeFile(LittleFS, device_idPath, _device_id.c_str());
              }

              Serial.printf("POST[%s]: %s\n", p->name().c_str(), p->value().c_str());
            }
          }

}



String processor(String var)
{
    return parent__->exposeMetrics(var);
}

WebServer::WebServer()
{
}
void WebServer::setParent(App *_parent)
{
    this->parent = _parent;
    parent__ = _parent;
}


void WebServer::startWebserver(){
    Serial.println(">> starting webserver");

    server.on("/", HTTP_GET, [this](AsyncWebServerRequest *request){ 
        Serial.println(">> serving index page");
        int paramsNr = request->params();
        //Serial.println(paramsNr);
        String pparams = "";
        for(int i=0;i<paramsNr;i++){
            AsyncWebParameter* p = request->getParam(i);
            pparams = pparams + p->name() + "=" + p->value();
        }
        this->parent->runCommand(pparams);

        request->send(LittleFS, "/index.html", "text/html", false, processor); });


    server.on("/config", HTTP_POST, [this](AsyncWebServerRequest *request){
        Serial.println(">> changing config variables");
        
        processConfig(request);

        //restarting
        request->send(200, "text/plain", "Done. ESP will restart! "); 
        Serial.println(">> WiFi Manager will restart");
        restart = true;
    });

    server.serveStatic("/", LittleFS, "/");
    server.begin();
    Serial.println(">> webserver running");
}



bool WebServer::initAPMode(String newHostname)
{
        if(newHostname=="") newHostname = "iot-generic-device-AP";
        else newHostname = newHostname + "-AP";
        parent__->getLedMonitor().led1("ERROR");
        // Connect to Wi-Fi network with SSID and password
        Serial.println(">> Setting AP (Access Point)");
        // NULL sets an open Access Point
        WiFi.softAP(newHostname.c_str(), NULL);

        IPAddress IP = WiFi.softAPIP();
        Serial.print(">> AP IP address: ");
        Serial.print(IP);
        Serial.println("");

        // Web Server Root URL
        server.on("/", HTTP_GET, [this](AsyncWebServerRequest *request){ 
          request->send(LittleFS, "/wifimanager.html", "text/html"); 
        });

        server.serveStatic("/", LittleFS, "/");

        server.on("/config", HTTP_POST, [this](AsyncWebServerRequest *request){
          //process config page 
          processConfig(request);
  
          //restarting
          request->send(200, "text/plain", "Done. ESP will restart! "); 
          Serial.println(">> WiFi Manager will restart");
          restart = true;
      });


      server.begin();
      Serial.println(">> (ap mode) SERVER BEGIN");
      return false;

}

void WebServer::loop()
{
  if(restart){
    delay(3000);
    ESP.restart();
  }
}