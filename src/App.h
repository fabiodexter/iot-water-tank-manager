#ifndef TESTE_1_H_INCLUDED
#define TESTE_1_H_INCLUDED
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
    LedMonitor getLedMonitor();
    
};
#endif // TESTE_1_H_INCLUDED