#include <Arduino.h>
#include <App.h>
#include <ESPAsyncWebServer.h>

class WebServer
{
private:
    App *parent;

public:
    WebServer();
    void setParent(App *_parent);
    void startWebserver();
    bool initAPMode(String newHostname);
    void loop();
};