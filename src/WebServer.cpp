#include <WebServer.h>
#include <Arduino.h>
#include <App.h>
#include "LittleFS.h"
#include <ESPAsyncWebServer.h>
#include <AsyncTCP.h>
#include <DNSServer.h>
App *parent_;
bool APMode = false;

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);
IPAddress localIP;
IPAddress localGateway;
IPAddress subnet(255, 255, 0, 0);

boolean restart = false;

const char* http_username = "dexter";
const char* http_password = "d33v0";



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
          
          const char *device_id_path = "/device_id.txt";          
          const char *ssid_path = "/ssid.txt";
          const char *pass_path = "/pass.txt";
          const char *gateway_path = "/gateway.txt";
          const char *mqtt_host_path = "/mqtt_host.txt";
          const char *mqtt_port_path = "/mqtt_port.txt";
          const char *mqtt_user_path = "/mqtt_user.txt";
          const char *mqtt_pass_path = "/mqtt_pass.txt";
          
          //custom variables
          const char *refresh_rate_path = "/refresh_rate.txt";
          const char *distance_min_volume_path = "/distance_min_volume.txt";
          const char *distance_max_volume_path = "/distance_max_volume.txt";





          int params = request->params();

          for(int i=0;i<params;i++){
            AsyncWebParameter* p = request->getParam(i);
            
            if(p->isPost()){


              // HTTP POST device_id value
              if (p->name() == "device_id") {
                String _device_id = p->value().c_str();
                Serial.print(">> device_id set to: ");
                Serial.println(_device_id);
                writeFile(LittleFS, device_id_path, _device_id.c_str());
              }

              // HTTP POST ssid value
              if (p->name() == "ssid") {
                String _ssid = p->value().c_str();
                Serial.print(">> SSID set to: ");
                Serial.println(_ssid);
                writeFile(LittleFS, ssid_path, _ssid.c_str());
              }
              
              // HTTP POST pass value
              if (p->name() == "pass") {
                String _pass = p->value().c_str();
                Serial.print(">> Password set to: ");
                Serial.println(_pass);
                writeFile(LittleFS, pass_path, _pass.c_str());
              }

              // HTTP POST gateway value
              if (p->name() == "gateway") {
                String _gateway = p->value().c_str();
                Serial.print(">> Gateway set to: ");
                Serial.println(_gateway);
                writeFile(LittleFS, gateway_path, _gateway.c_str());
              }
              
              // HTTP POST mqtt_host value
              if (p->name() == "mqtt_host") {
                String _mqtt_host = p->value().c_str();
                Serial.print(">> mqtt_host set to: ");
                Serial.println(_mqtt_host);
                writeFile(LittleFS, mqtt_host_path, _mqtt_host.c_str());
              }

              // HTTP POST mqtt_port value
              if (p->name() == "mqtt_port") {
                String _mqtt_port = p->value().c_str();
                Serial.print(">> mqtt_port set to: ");
                Serial.println(_mqtt_port);
                writeFile(LittleFS, mqtt_port_path, _mqtt_port.c_str());
              }
              
              // HTTP POST mqtt_user value
              if (p->name() == "mqtt_user") {
                String _mqtt_user = p->value().c_str();
                Serial.print(">> mqtt_user set to: ");
                Serial.println(_mqtt_user);
                writeFile(LittleFS, mqtt_user_path, _mqtt_user.c_str());
              }
              
              // HTTP POST mqtt_pass value
              if (p->name() == "mqtt_pass") {
                String _mqtt_pass = p->value().c_str();
                Serial.print(">> mqtt_pass set to: ");
                Serial.println(_mqtt_pass);
                writeFile(LittleFS, mqtt_pass_path, _mqtt_pass.c_str());
              }
              
              // HTTP POST refresh_rate value
              if (p->name() == "refresh_rate") {
                String _refresh_rate = p->value().c_str();
                Serial.print(">> refresh_rate set to: ");
                Serial.println(_refresh_rate);
                writeFile(LittleFS, refresh_rate_path, _refresh_rate.c_str());
              }
              
              // HTTP POST distance_min_volume value
              if (p->name() == "distance_min_volume") {
                String distance_min_volume = p->value().c_str();
                Serial.print(">> distance_min_volume set to: ");
                Serial.println(distance_min_volume);
                writeFile(LittleFS, distance_min_volume_path, distance_min_volume.c_str());
              }
              
              // HTTP POST distance_max_volume value
              if (p->name() == "distance_max_volume") {
                String _distance_max_volume = p->value().c_str();
                Serial.print(">> distance_max_volume set to: ");
                Serial.println(_distance_max_volume);
                writeFile(LittleFS, distance_max_volume_path, _distance_max_volume.c_str());
              }
              
              


              Serial.printf("POST[%s]: %s\n", p->name().c_str(), p->value().c_str());
            }
          }

}



String processor(String var)
{
    return parent_->exposeMetrics(var);
}

WebServer::WebServer()
{
}
void WebServer::setParent(App *_parent)
{
    parent_ = _parent;
}


void WebServer::start(){
    APMode = false;
    Serial.println(">> starting webserver");

    server.on("/", HTTP_GET, [this](AsyncWebServerRequest *request){ 
      if(!request->authenticate(http_username, http_password)) return request->requestAuthentication();

        Serial.println(">> serving index page");
        int paramsNr = request->params();
        //Serial.println(paramsNr);
        String pparams = "";
        for(int i=0;i<paramsNr;i++){
            AsyncWebParameter* p = request->getParam(i);
            pparams = pparams + p->name() + "=" + p->value();
        }
        parent_->runCommand(pparams);

        request->send(LittleFS, "/index.html", "text/html", false, processor); 
    });

    server.on("/settings", HTTP_GET, [this](AsyncWebServerRequest *request){ 
        if(!request->authenticate(http_username, http_password)) return request->requestAuthentication();      
        request->send(LittleFS, "/settings.html", "text/html", false, processor); 
    });

    //protected txt files
    server.on("^\\/([a-zA-Z0-9_])*.txt$", HTTP_GET, [this](AsyncWebServerRequest *request){ 
        if(!request->authenticate(http_username, http_password)) return request->requestAuthentication();
        request->send(LittleFS, request->url(), "text/plain", false, processor); 
    });


    server.on("/config", HTTP_POST, [this](AsyncWebServerRequest *request){
        Serial.println(">> changing config settings");
        
        processConfig(request);

        //restarting
        request->send(LittleFS, "/config_done.html", "text/html"); 
        Serial.println(">> WiFi Manager will restart");
        restart = true;
    });

    server.serveStatic("/", LittleFS, "/");
    server.begin();
    Serial.println(">> webserver running");
}


void WebServer::stop(){
  server.end();
}


bool WebServer::startAPMode(String newHostname)
{
      if(APMode) return false;
      APMode = true;
      server.end();
      WiFi.disconnect();

      if(newHostname=="") newHostname = "iot-generic-device-AP";
      else newHostname = newHostname + "-AP";

      parent_->setMonitorLed("led1","ERROR");
      //parent_->setMonitorLed("led2","OFF");
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
        request->send(LittleFS, "/settings.html", "text/html", false, processor); 
      });

      server.serveStatic("/", LittleFS, "/");

      server.on("/config", HTTP_POST, [this](AsyncWebServerRequest *request){
          //process config page 
          processConfig(request);
  
          //restarting
          request->send(LittleFS, "/config_done.html", "text/html"); 
          Serial.println(">> WiFi Manager will restart");
          restart = true;
      });


      server.begin();
      Serial.println(">> (ap mode) SERVER BEGIN");
      return false;

}

bool WebServer::isAPMode(){
  return APMode;
}

void WebServer::loop()
{
  if(restart){
    delay(3000);
    ESP.restart();
  }
}