#include <Arduino.h>
#include <App.h>
#include <RelayController.h>
#include <sensor_soil.h>
#include <SensorWaterflow.h>
#include <WifiManager.h>
#include <mqtt.h>
#include <EnvVars.h>

//RelayController pumpRelay(14);
SensorWaterflow sensorWaterflow(13);
WifiManager wifiManager;
MQTTManager mqtt_manager;
EnvVars vars;
const char *device_id = "iot-water-tank-manager";
const int tank_limit_pin = 15;
/*
const int led_red_pin = 12;
const int led_green_pin = 13;
const int led_blue_pin = 15;
*/
int tank_limit = 0;



//===================== Ultrasonic distance sensor HR-SR04 =====================================
#include <Ultrasonic.h>
Ultrasonic ultrasonic(14, 12);


void App::setup()
{
    Serial.begin(115200);
    Serial.println("");
    Serial.println(">>> starting app");
    vars.initFS();
    wifiManager.init(vars.ssid,vars.gateway,vars.pass,device_id);
    wifiManager.setParent(this);
    mqtt_manager.setParent(this);
    pinMode(tank_limit_pin, INPUT);
    /*
    pinMode(led_red_pin, OUTPUT);
    pinMode(led_green_pin, OUTPUT);
    pinMode(led_blue_pin, OUTPUT);
    */
}



void App::loop()
{
    curMillis = millis();
    sensorWaterflow.loop();
    wifiManager.loop();
    mqtt_manager.loop_mqtt();

    // json output =========================================================================
    if (curMillis - prevMillis > pubInterval)
    {

        float flow = sensorWaterflow.getFlowRate();

        if (digitalRead(tank_limit_pin) == HIGH)
            tank_limit = 1;
        else
            tank_limit = 0;
        /*
        digitalWrite(12, HIGH);
        digitalWrite(13, HIGH);
        digitalWrite(15, LOW);    
*/
        //===================== Ultrasonic distance sensor HR-SR04 =====================================
        int distance_surface = ultrasonic.read(CM);          
        String json = String("{") + "\"device_id\":\"" + device_id + "\",\"data\":{\"tank_limit\":" + tank_limit + ",\"flow\":" + flow + ",\"distance_surface\":" + distance_surface + "}}";
        Serial.println(json);
        char copy[250];
        json.toCharArray(copy, 250);
        mqtt_manager.publish_mqtt(copy);
        prevMillis = curMillis;
    }

}

void App::runCommand(String command)
{
    int pos = command.indexOf("=");
    String key = command.substring(0, pos);
    String value = command.substring(pos + 1);
    Serial.println("==> running command: " + key + ":" + value);
    if (key == "pump_status")
    {
        //pumpRelay.status(value);
    }
}

// Replaces placeholder with LED state value
String App::exposeMetrics(String var)
{
    //Serial.println(var);
    float flow = sensorWaterflow.getFlowRate();
    /*
    if(var=="PUMP_STATUS") return pumpRelay.status();
    else if(var=="WATER_FLOW") return String(flow);
    else if(var=="SOIL_HUMIDITY") return String(int(getSoilHumidity()*100));
    */
    return String("N/A");

}