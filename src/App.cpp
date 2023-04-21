#include <Arduino.h>
#include <App.h>
#include <SensorWaterflow.h>
#include <WebServer.h>
#include <mqtt.h>
#include <EnvVars.h>



SensorWaterflow sensorWaterflow(13);
WebServer webserver;
MQTTManager mqtt_manager;
EnvVars vars;
LedMonitor ledMonitor(1);



const int tank_limit_pin = 3;
bool tank_limit = false;
String wifi_status = "disconnected";




//===================== Ultrasonic distance sensor HR-SR04 =====================================
#include <Ultrasonic.h>
Ultrasonic ultrasonic(14, 12);


void App::setup()
{
    
    Serial.begin(115200);
    Serial.println("");
    Serial.println(">>> starting app");
    this->ledMonitor = ledMonitor;
    vars.initFS();
    
    pinMode(tank_limit_pin, INPUT);
    
    webserver.setParent(this);
    
    this->ledMonitor.led1("OFF");
    //localGateway.fromString(vars.gateway);
}

void App::reconnectWifi(){

    
    wifi_status =  "connecting";
    Serial.println(">> Connecting to WiFi...");

    WiFi.disconnect(true);
    WiFi.begin(vars.ssid, vars.pass);

    int tries = 0;
    while (WiFi.status() != WL_CONNECTED)
    {
        this->ledMonitor.led1("CONNECTING");
        delay(1000);
        Serial.println(">> connecting...");
        tries++;
        if(tries>5){
            webserver.initAPMode(vars.device_id);
            return;
        }
    }

    Serial.print(">> IP Address: ");
    Serial.println(WiFi.localIP());    
    WiFi.setHostname(vars.device_id.c_str());
    Serial.println(">> hostname: " + WiFi.hostname());
    delay(5000);
    this->ledMonitor.led1("CONNECTED");    
    wifi_status = "connected";
    //starting webserver
    webserver.startWebserver();
}


void App::loop()
{
    curMillis = millis();
    webserver.loop();
    ledMonitor.loop();


    if(wifi_status == "connecting") {
        return;
    }

    if(wifi_status != "connected") {
        reconnectWifi();
        return;
    }

    //Serial.println(wifi_status);
    mqtt_manager.loop_mqtt();


    // waterflow sensor  
    sensorWaterflow.loop();
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