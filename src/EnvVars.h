#ifndef ENV_VARS_INCLUDED
#define ENV_VARS_INCLUDED

#include <Arduino.h>
#include <LittleFS.h>
#include <App.h>

class EnvVars
{
private:
    // File paths to save input values permanently
    const char *device_idPath = "/device_id.txt";
    
    const char *ssidPath = "/ssid.txt";
    const char *passPath = "/pass.txt";
    const char *gatewayPath = "/gateway.txt";

    const char *mqtt_hostPath = "/mqtt_host.txt";
    const char *mqtt_portPath = "/mqtt_port.txt";
    const char *mqtt_userPath = "/mqtt_user.txt";
    const char *mqtt_passPath = "/mqtt_pass.txt";

public:
    EnvVars();
    void initFS();
    String readFile(fs::FS &fs, const char *path);
    
    String device_id;
    
    String ssid;
    String pass;
    String gateway;

    String mqtt_host;
    String mqtt_port;
    String mqtt_user;
    String mqtt_pass;
};
#endif // ENV_VARS_INCLUDED