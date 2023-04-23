
//Basic modules
#include <Arduino.h>
#include <App.h>
#include <EnvVars.h>
#include <WifiManager.h>
#include <mqtt.h>
#include <WebServer.h>
EnvVars vars;
WebServer webserver;
WifiManager wifi_manager;
MQTTManager mqtt_manager;
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
    this->ledMonitor = ledMonitor;
    webserver.setParent(this);
    wifi_manager.setParams(this,vars);
    mqtt_manager.setParams(this,vars);

    //tank limit sensor
    pinMode(tank_limit_pin, INPUT);    
}



void App::loop()
{
    curMillis = millis();

    //loop basic modules
    webserver.loop();
    ledMonitor.loop();
    wifi_manager.loop();
    mqtt_manager.loop();


    // waterflow sensor  
    sensorWaterflow.loop();
    flow_rate = sensorWaterflow.getFlowRate();
    int flow_count = sensorWaterflow.getFlowCount();


    //establishes pubInterval based on flow_rate
    int ii = pubInterval;
    if(flow_rate>0) ii = 1000;
    if (curMillis - prevMillis > ii)
    {
        // water level sensor 
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

        // Publishing results 
        if(isMQTTConnected()){
            String json = String("{") + "\"device_id\":\"" + vars.device_id + "\",\"data\":{\"tank_limit\":" + tank_limit + ",\"flow_count\":" + flow_count + ",\"flow_rate\":" + flow_rate + ",\"distance_surface\":" + distance_surface + "}}";
            Serial.println(json);
            char copy[250];
            json.toCharArray(copy, 250);
            mqtt_manager.publish_mqtt(copy);
        }
        
        //closing loop
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



String App::exposeMetrics(String var)
{
    if(var=="DEVICE_ID") return String(vars.device_id);
    else if(var=="DISTANCE_SURFACE") return String(distance_surface);
    else if(var=="WATER_FLOW") return String(flow_rate);
    else if(var=="TANK_LIMIT") return String(tank_limit);

    return String("N/A");
}

LedMonitor App::getLedMonitor(){
    return this->ledMonitor;
}

boolean App::isWifiConnected(){
    return wifi_manager.isConnected();
}


boolean App::isMQTTConnected(){
    return mqtt_manager.isConnected();
}



/*
WebServer App::getWebServer(){
    return this->webserver;
}
*/