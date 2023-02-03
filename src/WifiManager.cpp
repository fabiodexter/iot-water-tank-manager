#include <WifiManager.h>
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
const char *PARAM_INPUT_3 = "ip";
const char *PARAM_INPUT_4 = "gateway";


// Timer variables
unsigned long previousMillis = 0;
const long interval = 10000; // interval to wait for Wi-Fi connection (milliseconds)

boolean restart = false;


// Initialize WiFi
bool initWiFi(String ssid, String gateway,String pass, String newHostname)
{

    parent__->getLedMonitor().led1("OFF");   
    if (ssid == "")
    {
        parent__->getLedMonitor().led1("ERROR");   
        Serial.println("Undefined SSID or IP address.");
        return false;
    }

    localGateway.fromString(gateway.c_str());

    Serial.println("Connecting to WiFi...");

    WiFi.disconnect(true);
    WiFi.begin(ssid.c_str(), pass.c_str());

    int tries = 0;
    int maxtries = 10;
    while (WiFi.status() != WL_CONNECTED)
    {
        parent__->getLedMonitor().led1("CONNECTING");
        delay(1000);
        Serial.println("connecting...");
        tries ++;
        if(tries>maxtries){
          Serial.println("Failed to connect.");
          parent__->getLedMonitor().led1("ERROR");          
          return false;
        }

    }
    WiFi.setHostname(newHostname.c_str());
    Serial.println("> hostname: " + WiFi.hostname());
    parent__->getLedMonitor().led1("CONNECTED");    
    return true;
}

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



String processor(String var)
{
    return parent__->exposeMetrics(var);
}

WifiManager::WifiManager()
{
}
void WifiManager::setParent(App *_parent)
{
    this->parent = _parent;
    parent__ = _parent;
}

bool WifiManager::init(String ssid,String gateway,String pass, String newHostname)
{
    if (initWiFi(ssid,gateway,pass,newHostname))
    {
        Serial.println("connected to wifi router");

        server.on("/", HTTP_GET, [this,ssid,gateway,pass](AsyncWebServerRequest *request)
                  { 
            Serial.println("serving index page");
            int paramsNr = request->params();
            //Serial.println(paramsNr);
            String pparams = "";
            for(int i=0;i<paramsNr;i++){
                AsyncWebParameter* p = request->getParam(i);
                pparams = pparams + p->name() + "=" + p->value();
            }
            this->parent->runCommand(pparams);

            request->send(LittleFS, "/index.html", "text/html", false, processor); });

        server.serveStatic("/", LittleFS, "/");
        server.begin();
        Serial.println("webserver running");
        return true;
    }
    else
    {

        parent__->getLedMonitor().led1("ERROR");
        Serial.println("## ssid:" + ssid);
        Serial.println("## gateway:" + gateway);
        Serial.println("## pass:" + pass);
        // Connect to Wi-Fi network with SSID and password
        Serial.println("Setting AP (Access Point)");
        // NULL sets an open Access Point
        WiFi.softAP(newHostname.c_str(), NULL);

        IPAddress IP = WiFi.softAPIP();
        Serial.print("AP IP address: ");
        Serial.println(IP);

        // Web Server Root URL
        server.on("/", HTTP_GET, [ssid,gateway,pass](AsyncWebServerRequest *request){ 
          request->send(LittleFS, "/wifimanager.html", "text/html"); 
        });

        server.serveStatic("/", LittleFS, "/");

        server.on("/", HTTP_POST, [ssid,gateway,pass](AsyncWebServerRequest *request){


      int params = request->params();
      String _ip;
      const char *ssidPath = "/ssid.txt";
      const char *passPath = "/pass.txt";
      const char *ipPath = "/ip.txt";
      const char *gatewayPath = "/gateway.txt";

      for(int i=0;i<params;i++){
        AsyncWebParameter* p = request->getParam(i);
        
        if(p->isPost()){
          // HTTP POST ssid value
          if (p->name() == PARAM_INPUT_1) {
            String _ssid = p->value().c_str();
            Serial.print("SSID set to: ");
            Serial.println(_ssid);
            // Write file to save value
            writeFile(LittleFS, ssidPath, _ssid.c_str());
          }
          
          // HTTP POST pass value
          if (p->name() == PARAM_INPUT_2) {
            String _pass = p->value().c_str();
            Serial.print("Password set to: ");
            Serial.println(_pass);
            // Write file to save value
            writeFile(LittleFS, passPath, _pass.c_str());
          }
          // HTTP POST ip value
          if (p->name() == PARAM_INPUT_3) {
            _ip = p->value().c_str();
            Serial.print("IP Address set to: ");
            Serial.println(_ip);
            // Write file to save value
            writeFile(LittleFS, ipPath, _ip.c_str());
          }
          // HTTP POST gateway value
          if (p->name() == PARAM_INPUT_4) {
            String _gateway = p->value().c_str();
            Serial.print("Gateway set to: ");
            Serial.println(_gateway);
            // Write file to save value
            writeFile(LittleFS, gatewayPath, _gateway.c_str());
          }
          
          Serial.printf("POST[%s]: %s\n", p->name().c_str(), p->value().c_str());
        }
      }
  
        //restart = true;
        request->send(200, "text/plain", "Done. ESP will restart, connect to your router and go to IP address: " + _ip); 
      });
      server.begin();
      Serial.println("## (ap mode) SERVER BEGIN");
      return false;
    }
}

void WifiManager::loop()
{
  //Serial.println("## WiFi Manager loop");
    if (restart)
    {
       Serial.println("## WiFi Manager restarting");
        delay(5000);
        ESP.restart();
    }
}