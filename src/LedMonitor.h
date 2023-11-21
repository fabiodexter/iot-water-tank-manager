#include <Arduino.h>
#include <App.h>
class LedMonitor {
    private:
        int pin;
        long curMillis = 0;
        long prevMillis = 0;
        int refreshRate = 250;
        App *parent;
        String led1_status = "OFF";
        String led2_status = "OFF";
        //String led3_status = "OFF";
    public:
        LedMonitor();
        LedMonitor(int pin);
        void setParent(App *_parent);
        void loop();
        String led1();
        String led2();
        //String led3();       
        void led1(String status);
        void led2(String status);
        //void led3(String status);     
};