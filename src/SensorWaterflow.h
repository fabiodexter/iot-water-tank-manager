#include <Arduino.h>
#include <App.h>

class SensorWaterflow
{
private:
    int pin = 1;
    long curMillis = 0;
    long prevMillis = 0;
    int refreshRate = 1000;
    float calibrationFactor = 4.5;
    float flowRate;

public:
    SensorWaterflow(int pin);
    void loop();
    float getFlowRate();
};