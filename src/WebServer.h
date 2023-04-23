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
    void start();
    void stop();
    bool startAPMode(String newHostname);
    void loop();
};