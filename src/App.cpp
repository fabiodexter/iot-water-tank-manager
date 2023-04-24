
//Basic modules
#include <Arduino.h>
#include <App.h>
#include <EnvVars.h>
#include <WifiManager.h>
#include <mqtt.h>
#include <WebServer.h>
#include <LedMonitor.h>
EnvVars vars;
WebServer webserver;
WifiManager wifiClient;
MQTTManager mqttClient;
LedMonitor ledMonitor(1);



//Tank limit sensor
const int tank_limit_pin = 3;
bool tank_limit = false;

//waterflow sensor
#include <SensorWaterflow.h>
SensorWaterflow sensorWaterflow(13);
float flow_rate = 0.00;

// Ultrasonic distance sensor HR-SR04 
int distance_surface = 500;
#include <Ultrasonic.h>
Ultrasonic ultrasonic(14, 12);



void App::setup()
{

    //setup Serial port    
    Serial.begin(115200);
    Serial.println("");
    Serial.println(">>> starting app");

    //Basic modules setup
    vars.initFS();
    ledMonitor.setParent(this);
    webserver.setParent(this);
    wifiClient.setParams(this,vars);
    mqttClient.setParams(this,vars);

    //tank limit sensor
    pinMode(tank_limit_pin, INPUT);    
}



void App::loop()
{
    curMillis = millis();

    // tank limit sensor 
    if (digitalRead(tank_limit_pin) == HIGH){
        tank_limit = false;
        ledMonitor.led3("OFF");
    }
    else{
        tank_limit = true;
        ledMonitor.led3("ERROR");
    }

    // Ultrasonic distance sensor HR-SR04 
    distance_surface = ultrasonic.read(CM);   

    // waterflow sensor  
    sensorWaterflow.loop();
    flow_rate = sensorWaterflow.getFlowRate();
    int flow_count = sensorWaterflow.getFlowCount();


 

    //establishes pubInterval based on flow_rate
    int shortInterval = pubInterval;
    if(flow_rate>0) shortInterval = 1000;
    if (curMillis - prevMillis > shortInterval)
    {

        // Publishing results 
        if(isMQTTConnected()){
            String json = String("{") + "\"device_id\":\"" + vars.device_id + "\",\"data\":{\"tank_limit\":" + tank_limit + ",\"flow_count\":" + flow_count + ",\"flow_rate\":" + flow_rate + ",\"distance_surface\":" + distance_surface + "}}";
            Serial.println(json);
            mqttClient.publish_mqtt((char*)json.c_str());
        }
        
        //closing loop
        prevMillis = curMillis;
    }


    //loop basic modules
    ledMonitor.loop();
    wifiClient.loop();
    mqttClient.loop();
    webserver.loop();

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



String App::exposeMetrics(String var)
{
    if(var=="DEVICE_ID") return String(vars.device_id);
    else if(var=="DISTANCE_SURFACE") return String(distance_surface);
    else if(var=="WATER_FLOW") return String(flow_rate);
    else if(var=="TANK_LIMIT") return String(tank_limit);

    return String("N/A");
}


boolean App::isWifiConnected(){
    return wifiClient.isConnected();
}

boolean App::isMQTTConnected(){
    return mqttClient.isConnected();
}

void App::startWebServer(){
    webserver.start();
}
void App::stopWebServer(){
    webserver.stop();
}
void App::startAPMode(){
    webserver.startAPMode(vars.device_id);
}

void App::setMonitorLed(String led, String status){
    if(led=="led1") ledMonitor.led1(status);
    else if(led=="led2") ledMonitor.led2(status);
    else if(led=="led3") ledMonitor.led3(status);
}