#ifndef ENV_VARS_INCLUDED
#define ENV_VARS_INCLUDED

#include <Arduino.h>
#include <LittleFS.h>
#include <App.h>

class EnvVars
{
private:
    // File paths to save input values permanently
    const char *device_id_path = "/device_id.txt";
    
    const char *ssid_path = "/ssid.txt";
    const char *pass_path = "/pass.txt";
    const char *gateway_path = "/gateway.txt";

    const char *mqtt_host_path = "/mqtt_host.txt";
    const char *mqtt_port_path = "/mqtt_port.txt";
    const char *mqtt_user_path = "/mqtt_user.txt";
    const char *mqtt_pass_path = "/mqtt_pass.txt";

    const char *refresh_rate_path = "/refresh_rate.txt";
    const char *distance_min_volume_path = "/distance_min_volume.txt";
    const char *distance_max_volume_path = "/distance_max_volume.txt";


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

    String refresh_rate;
    String distance_min_volume;
    String distance_max_volume;

};
#endif // ENV_VARS_INCLUDED