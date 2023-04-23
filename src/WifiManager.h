#include <ESP8266WiFi.h>
#include <Arduino.h>
#include <App.h>

class WifiManager{

    private:
        App *parent;
        char* device_id;
        char* ssid;
        char* gateway;
        char* pass;

    public:
        WifiManager();
        bool isConnected();
        void reconnect();
        void loop();
        void setParent(App *_parent);
        void setParams(char* device_id,char* ssid,char* gateway,char* pass);
};