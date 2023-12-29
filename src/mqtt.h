#include <WiFi.h>
#include <PubSubClient.h>
#include <Arduino.h>
#include <App.h>
#include <EnvVars.h>
class MQTTManager
{
private:
    App *parent;
    String device_id;
    String mqtt_host;
    String mqtt_port;
    String mqtt_user;
    String mqtt_pass;

public:
    MQTTManager();
    bool isConnected();
    void reconnect();
    void loop();
    void setParams(App *parent, EnvVars vars);
    void publish_mqtt(char *copy);
};