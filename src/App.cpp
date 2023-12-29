
//Basic modules
#include <Arduino.h>
#include <App.h>
#include <EnvVars.h>
#include <WifiManager.h>
#include <mqtt.h>
#include <WebServer.h>
#include <LedMonitor.h>
#include <Wire.h>
#include <DallasTemperature.h>

EnvVars vars;
WebServer webserver;
WifiManager wifiClient;
MQTTManager mqttClient;
LedMonitor ledMonitor(1);



//Tank limit sensor
#define TANK_LIMIT_SWITCH_PIN 35 //BROWN WIRE
bool tank_limit = false;

// Ultrasonic distance sensor HR-SR04 
#define ULTRASONIC_TRIGGER_SENSOR_PIN 33 //RED WIRE
#define ULTRASONIC_ECHO_SENSOR_PIN 32 //ORANGE WIRE
int distance_surface = 999;
float water_volume = 0.00;
#include <Ultrasonic.h>
Ultrasonic ultrasonic(ULTRASONIC_ECHO_SENSOR_PIN, ULTRASONIC_TRIGGER_SENSOR_PIN);

//waterflow sensor
#define WATERFLOW_SENSOR_PIN 25 //YELLOW WIRE
unsigned long last_millis = 0;
float calibrationFactor = 4.5;
int flow_count = 0;
float flow_rate = 0.00;

void IRAM_ATTR funcao_ISR()
{
    if ( (millis() - last_millis) >= 10 ){
    flow_count++;
    last_millis = millis();
    }
}


// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
#define WATER_TEMP_SENSOR_PIN 26 //GREEN WIRE
OneWire oneWire(WATER_TEMP_SENSOR_PIN);
DallasTemperature DS18B20(&oneWire);
float tempC; // temperature in Celsius

//Aambient temperature sensor
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#define DHTPIN 27  //BLUE WIRE
#define DHTTYPE DHT11 
DHT dht(DHTPIN, DHTTYPE);

boolean isMQTTConnected(){
    return mqttClient.isConnected();
}


void App::setup()
{

    //setup Serial port    
    Serial.begin(115200);
    Serial.println("");
    Serial.println(">>> starting app...");



    Wire.begin();
    //Basic modules setup
    vars.initFS();
    ledMonitor.setParent(this);
    webserver.setParent(this);
    wifiClient.setParams(this,vars);
    mqttClient.setParams(this,vars);

    pinMode(TANK_LIMIT_SWITCH_PIN, INPUT);  
    pinMode(25, INPUT);
    attachInterrupt(25,funcao_ISR,FALLING);
    Serial.print("interrupt attached");


    // Start up the library
    dht.begin();
    DS18B20.begin();


    //if vars not loaded, start APMode
    if(vars.ssid == "" || vars.pass == ""){
        startAPMode();
    }
}


void gatherSensorsData(){


    if(digitalRead(TANK_LIMIT_SWITCH_PIN)==HIGH) tank_limit = false;
    else tank_limit = true;

    //water temperature
    DS18B20.requestTemperatures();       // send the command to get temperatures
    tempC = DS18B20.getTempCByIndex(0);  // read temperature in °C

    // Ultrasonic distance sensor HR-SR04 
    distance_surface = ultrasonic.read(CM);   
    //if(distance_surface > vars.distance_min_volume.toInt()) distance_surface = vars.distance_min_volume.toInt();
    //if(distance_surface < vars.distance_max_volume.toInt()) distance_surface = vars.distance_max_volume.toInt();
    int deltad = vars.distance_min_volume.toInt() - vars.distance_max_volume.toInt();
    int calc1 = (distance_surface - vars.distance_max_volume.toInt());
    float calc2 = calc1*1.0 / deltad*1.0;
    if(deltad == 0){
        water_volume = 0.00;
    }  else {
        water_volume = 1.00-(calc2);
    }

    
    // Reading temperature or humidity takes about 250 milliseconds!
    float h = dht.readHumidity();
    float t = dht.readTemperature();
    float hic = 0.00;
    // Check if any reads failed and exit early (to try again).
    if (isnan(h) || isnan(t)) {
        Serial.println(F("Failed to read from DHT sensor!"));
        h = 0.00;
        t = 0.00;
    }else{
        hic = dht.computeHeatIndex(t, h, false);
    }



    /* =========================================================================================================*/
    /* ============================================ PUBLISHING DATA ============================================*/
    /* =========================================================================================================*/

    if(isMQTTConnected()){
        String json = String("{") + "\"device_id\":\"" + vars.device_id + "\",\"data\":{\"tank_limit\":" + tank_limit + ",\"flow_count\":" + flow_count + ",\"flow_rate\":" + flow_rate + ",\"distance_surface\":" + distance_surface + ", \"water_vol\":" + water_volume + ",\"water_temp\":" + tempC + ", \"temp\":"+t+", \"hum\":"+h+", \"heat_index\":"+hic+"}}";
        Serial.println(json);
        mqttClient.publish_mqtt((char*)json.c_str());
    }
        

}




void App::loop(){

    //Serial.println(">>> main loop started");
    curMillis = millis();

    if (webserver.isAPMode()) {
        ledMonitor.loop();
        webserver.loop();        
        return;
    } 

    // get flowrate first
    flow_rate = ((1000.00 / (curMillis - prevMillis)) * flow_count * 1.00) / calibrationFactor;

    if(vars.refresh_rate.toInt()>1000) pubInterval = vars.refresh_rate.toInt() * 1000;
    int shortInterval = pubInterval;
    if(flow_rate>0) shortInterval = 1000;

    if (curMillis - prevMillis > shortInterval){
        
        gatherSensorsData();
        
        //closing loop
        prevMillis = curMillis;
        flow_count = 0;
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
    if(command != "") Serial.println("==> running command: " + key + ":" + value);
    if (key == "pump_status")
    {
        //pumpRelay.status(value);
    }
}



String App::exposeMetrics(String var)
{
    /*
    if(var=="DEVICE_ID") return String(vars.device_id);
    else if(var=="DISTANCE_SURFACE") return String(distance_surface);
    else if(var=="WATER_FLOW") return String(flow_rate);
    else if(var=="TANK_LIMIT") return String(tank_limit);
*/
    return String("N/A");
}


boolean App::isWifiConnected(){
    return wifiClient.isConnected();
}


void App::startWebServer(){
    webserver.start();
}
void App::stopWebServer(){
    webserver.stop();
}
void App::startAPMode(){
    Serial.println("Starting AP mode");
    webserver.startAPMode(vars.device_id);
}

void App::setMonitorLed(String led, String status){
    if(led=="led1") ledMonitor.led1(status);
    else if(led=="led2") ledMonitor.led2(status);
}