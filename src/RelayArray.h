#include <Arduino.h>
#include <App.h>
class RelayArray {
    private:
        int pin;
        long curMillis = 0;
        long prevMillis = 0;
        int refreshRate = 250;
        App *parent;
    public:
        RelayArray();
        RelayArray(int pin);
        void setParent(App *_parent);
        void setRelay(int module, int relay, int status);
        void loop();  
        void reset();  
};