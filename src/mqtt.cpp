#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <mqtt.h>


WiFiClient espClient;
PubSubClient mqtt_client(espClient);
App *parent;

bool connecting = false;

void subscribe_callback(char *topic, byte *payload, unsigned int length)
{

    payload[length] = '\0';

    String strTopic = (String)topic;
    //Serial.println("topic:" + strTopic);

    String strPayload = String((char *)payload); // convert to string
    //Serial.println("mqtt command received:" + strPayload);
    parent->runCommand(strPayload);
}


MQTTManager::MQTTManager()
{

}


void MQTTManager::reconnect()
{
    if(!this->parent->isWifiConnected()) return;
    connecting = true;

    while (!mqtt_client.connected())
    {
        Serial.print(">> Attempting MQTT connection...");
        if (mqtt_client.connect(this->device_id, this->mqtt_user, this->mqtt_pass))
        {
            String topic = "/controllers/" + String(this->device_id) + "/#";
            parent->getLedMonitor().led2("CONNECTED"); //BUG: this is causing exceptions in some circunstances
            Serial.println(">> mqtt broker connected");
            mqtt_client.subscribe(topic.c_str());
            Serial.println(">> subscribed to " + topic);
            connecting = false;
        }
        else
        {
            Serial.print(">> failed, rc=");
            Serial.print(mqtt_client.state());
            Serial.println(">> trying again...");
            parent->getLedMonitor().led2("CONNECTING"); //BUG: this is causing exceptions in some circunstances
           
            delay(1000);
        }
    }
}


void MQTTManager::loop_mqtt()
{
    if (!mqtt_client.connected() && connecting == false)
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
        parent->getLedMonitor().led2("DATA"); //BUG: this is causing exceptions in some circunstances
    }
}

void MQTTManager::setParent(App *_parent)
{
    this->parent = _parent;
    parent = _parent;
}

void MQTTManager::setParams(char* device_id, char* mqtt_host,int mqtt_port,char* mqtt_user,char* mqtt_pass)
{
    this->device_id = device_id;
    this->mqtt_host = mqtt_host;
    this->mqtt_port = mqtt_port;
    this->mqtt_user = mqtt_user;
    this->mqtt_pass = mqtt_pass;
    mqtt_client.setServer(mqtt_host, mqtt_port);
    mqtt_client.setCallback(subscribe_callback);//aqui começa os problemas    
}

bool MQTTManager::isConnected()
{
    return mqtt_client.connected();
}

