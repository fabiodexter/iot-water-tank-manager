#include <WiFi.h>
#include <Arduino.h>
#include <App.h>
#include <EnvVars.h>

class WifiManager{

    private:
        App *parent;
        String device_id;
        String ssid;
        String gateway;
        String pass;

    public:
        WifiManager();
        bool isConnected();
        void reconnect();
        void loop();
        void setParams(App *_parent, EnvVars vars);
};