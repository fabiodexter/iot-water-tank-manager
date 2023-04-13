#include <Arduino.h>
#include <App.h>
#include <SensorWaterflow.h>
#include <WifiManager.h>
#include <mqtt.h>
#include <EnvVars.h>



SensorWaterflow sensorWaterflow(13);
WifiManager wifiManager;
MQTTManager mqtt_manager;
EnvVars vars;
LedMonitor ledMonitor(1);



const int tank_limit_pin = 3;
bool tank_limit = false;
bool connected = false;


//===================== Ultrasonic distance sensor HR-SR04 =====================================
#include <Ultrasonic.h>
Ultrasonic ultrasonic(14, 12);


void App::setup()
{
    
    Serial.begin(115200);
    Serial.println("");
    Serial.println(">>> starting app");
    this->ledMonitor = ledMonitor;
    pinMode(tank_limit_pin, INPUT);

    vars.initFS();
    wifiManager.setParent(this);
    

    if(wifiManager.init(vars.ssid,vars.gateway,vars.pass,vars.device_id)){
        mqtt_manager.setParent(this);
        mqtt_manager.reconnect();
        Serial.println(">>> Wifi connected !");
        connected = true;
    }else{
        Serial.println(">>> No Wifi, starting AP Mode");
        connected = false;
    }
}



void App::loop()
{
    curMillis = millis();
    ledMonitor.loop();

    if(!connected) return;

    sensorWaterflow.loop();
    wifiManager.loop();
    mqtt_manager.loop_mqtt();


    // waterflow sensor  
    float flow_rate = sensorWaterflow.getFlowRate();
    int flow_count = sensorWaterflow.getFlowCount();

    int ii = pubInterval;
    if(flow_rate>0) ii = 1000;

    // json output =========================================================================
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
        int distance_surface = ultrasonic.read(CM);    

        // Publishing results 
        if(mqtt_manager.getStatus()==true){
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
    //Serial.println(var);
    float flow_rate = sensorWaterflow.getFlowRate();
    /*
    if(var=="PUMP_STATUS") return pumpRelay.status();
    else if(var=="WATER_FLOW") return String(flow);
    else if(var=="SOIL_HUMIDITY") return String(int(getSoilHumidity()*100));
    */
    return String("N/A");
    

}


LedMonitor App::getLedMonitor(){
    return this->ledMonitor;
}