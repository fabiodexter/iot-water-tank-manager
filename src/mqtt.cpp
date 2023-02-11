#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <mqtt.h>


WiFiClient espClient;
PubSubClient mqtt_client(espClient);

int tries = 0;
const int maxtries = 5;
int mqtt_connect_millis = 0;
App *parent;

void subscribe_callback(char *topic, byte *payload, unsigned int length)
{

    payload[length] = '\0';

    String strTopic = (String)topic;
    Serial.println("topic:" + strTopic);

    String strPayload = String((char *)payload); // convert to string
    Serial.println("mqtt command received:" + strPayload);
    parent->runCommand(strPayload);
}


MQTTManager::MQTTManager()
{
    mqtt_client.setServer("172.16.0.11", 1883);
    mqtt_client.setCallback(subscribe_callback);//aqui começa os problemas
}


void MQTTManager::reconnect()
{
    while (!mqtt_client.connected())
    {
        Serial.print("Attempting MQTT connection...");
        if (mqtt_client.connect("iot-water-tank-manager", "renato", "$tr0nz0"))
        {
            parent->getLedMonitor().led2("CONNECTED");
            Serial.println("mqtt broker connected");
            Serial.println(mqtt_client.subscribe("/controllers/iot-water-tank-manager/#"));
            Serial.println("subscribed to " + String("/controllers/iot-water-tank-manager/#"));
        }
        else
        {
            tries = tries + 1;
            if (tries > maxtries){
                parent->getLedMonitor().led2("ERROR");
                return;
            }
            Serial.print("failed, rc=");
            Serial.print(mqtt_client.state());
            Serial.println(" try again in 5 seconds");
            parent->getLedMonitor().led2("CONNECTING");
            delay(1000);
        }
    }
}


void MQTTManager::loop_mqtt()
{
    if (!mqtt_client.connected())
    {
        reconnect();
    }
    mqtt_client.loop();
}

void MQTTManager::publish_mqtt(char *copy)
{
    if (mqtt_client.connected())
    {
        mqtt_client.publish("/sensors", copy);
        parent->getLedMonitor().led2("DATA");
    }
}

void MQTTManager::setParent(App *_parent)
{
    Serial.println("mqtt-> setting parent");
    this->parent = _parent;
    parent = _parent;
}
