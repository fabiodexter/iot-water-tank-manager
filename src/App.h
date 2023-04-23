#ifndef APP_INCLUDED
#define APP_INCLUDED
#include <Arduino.h>
#include <LedMonitor.h>

class App
{
private:
    const char *device_id = "iot-water-tank-manager";
    long curMillis = 0;
    long prevMillis = 0;
    int pubInterval = 10000;
    LedMonitor ledMonitor;

public:
    App(){

    }    
    void setup();
    void loop();
    void runCommand(String command);
    String exposeMetrics(String var);
    boolean isWifiConnected();
    boolean isMQTTConnected();
    LedMonitor getLedMonitor();
    void startWebServer();
    void stopWebServer();
    void startAPMode(String newHostname);    
};
#endif // APP_INCLUDED