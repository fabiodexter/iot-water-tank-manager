#include <Arduino.h>
#include <LittleFS.h>
#include <App.h>

class EnvVars
{
private:
    // File paths to save input values permanently
    const char *ssidPath = "/ssid.txt";
    const char *passPath = "/pass.txt";
    const char *ipPath = "/ip.txt";
    const char *gatewayPath = "/gateway.txt";

public:
    EnvVars();
    void initFS();
    String readFile(fs::FS &fs, const char *path);
    String ssid;
    String pass;
    String ip;
    String gateway;
};