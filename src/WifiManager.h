#include <Arduino.h>
#include <App.h>
#include <ESPAsyncWebServer.h>

class WifiManager
{
private:
    App *parent;
    //void processConfig(AsyncWebServerRequest *request);

public:
    WifiManager();
    void setParent(App *_parent);
    //void init();
    void startWebserver();
    bool initAPMode(String ssid,String gateway,String pass, String newHostname);
    void loop();
};