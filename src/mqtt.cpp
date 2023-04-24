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
    if(!parent->isWifiConnected()) return;
    connecting = true;

    while (!mqtt_client.connected())
    {
        Serial.print(">> Attempting MQTT connection...");
        if (mqtt_client.connect(this->device_id.c_str(), this->mqtt_user.c_str(), this->mqtt_pass.c_str()))
        {
            String topic = "/controllers/" + this->device_id + "/#";
            //parent->getLedMonitor().led2("CONNECTED"); 
            parent->setMonitorLed("led2","CONNECTED");
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
            //parent->getLedMonitor().led2("CONNECTING");            
            parent->setMonitorLed("led2","CONNECTING");
            delay(1000);
        }
    }
}


void MQTTManager::loop()
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
        //parent->getLedMonitor().led2("DATA"); 
        parent->setMonitorLed("led2","DATA");
    }
}


void MQTTManager::setParams(App *_parent, EnvVars vars)
{
    this->parent = _parent;
    this->device_id = vars.device_id;
    this->mqtt_host = vars.mqtt_host;
    this->mqtt_port = vars.mqtt_port;
    this->mqtt_user = vars.mqtt_user;
    this->mqtt_pass = vars.mqtt_pass;
    mqtt_client.setServer((char*) mqtt_host.c_str(), mqtt_port.toInt());
    mqtt_client.setCallback(subscribe_callback);//aqui começam os problemas    
}

bool MQTTManager::isConnected()
{
    return mqtt_client.connected();
}

