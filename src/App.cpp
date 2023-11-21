
//Basic modules
#include <Arduino.h>
#include <App.h>
#include <EnvVars.h>
#include <WifiManager.h>
#include <mqtt.h>
#include <WebServer.h>
#include <LedMonitor.h>
#include <OneWire.h>
#include <DallasTemperature.h>

EnvVars vars;
WebServer webserver;
WifiManager wifiClient;
MQTTManager mqttClient;
LedMonitor ledMonitor(1);

//Tank limit sensor
bool tank_limit = false;

//waterflow sensor
#include <SensorWaterflow.h>
SensorWaterflow sensorWaterflow(13);
float flow_rate = 0.00;

// Ultrasonic distance sensor HR-SR04 
int distance_surface = 500;
float water_volume = 0.00;
#include <Ultrasonic.h>
Ultrasonic ultrasonic(14, 12);

// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
#define SENSOR_PIN 2
OneWire oneWire(SENSOR_PIN);
DallasTemperature DS18B20(&oneWire);
float tempC; // temperature in Celsius



#include <BME280I2C.h>
BME280I2C::Settings settings(
   BME280::OSR_X1,
   BME280::OSR_X1,
   BME280::OSR_X1,
   BME280::Mode_Forced,
   BME280::StandbyTime_1000ms,
   BME280::Filter_16,
   BME280::SpiEnable_False,
   BME280I2C::I2CAddr_0x76
);

BME280I2C bme(settings);


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
    //pinMode(tank_limit_pin, INPUT);    


    // Start up the library
    DS18B20.begin();
    bme.begin();

  switch(bme.chipModel())
  {
     case BME280::ChipModel_BME280:
       Serial.println("Found BME280 sensor! Success.");
       break;
     case BME280::ChipModel_BMP280:
       Serial.println("Found BMP280 sensor! No Humidity available.");
       break;
     default:
       Serial.println("Found UNKNOWN sensor! Error!");
  }

    //if vars not loaded, start APMode
    if(vars.ssid == "" || vars.pass == ""){
        startAPMode();
    }
}



void App::loop()
{
    //Serial.println(">>> main loop started");
    if (webserver.isAPMode()) {
        ledMonitor.loop();
        webserver.loop();        
        return;
    }

    curMillis = millis();

    //water temperature
    DS18B20.requestTemperatures();       // send the command to get temperatures
    tempC = DS18B20.getTempCByIndex(0);  // read temperature in °C
    
    //ambient temp, humidity and pressure
    float temp(NAN), hum(NAN), pres(NAN);
    BME280::TempUnit tempUnit(BME280::TempUnit_Celsius);
    BME280::PresUnit presUnit(BME280::PresUnit_Pa);
    bme.read(pres, temp, hum, tempUnit, presUnit);
    if(isnan(temp)) temp = 0;
    if(isnan(hum)) hum = 0;
    if(isnan(pres)) pres = 0;


    // Ultrasonic distance sensor HR-SR04 
    //Serial.println(">>> main loop: distance sensor");
    distance_surface = ultrasonic.read(CM);   

    if(distance_surface > vars.distance_min_volume.toInt()) distance_surface = vars.distance_min_volume.toInt();
    if(distance_surface < vars.distance_max_volume.toInt()) distance_surface = vars.distance_max_volume.toInt();
    int deltad = vars.distance_min_volume.toInt() - vars.distance_max_volume.toInt();
    //Serial.println(">>> main loop: delta d: " + deltad); 
    if(deltad == 0){
        water_volume = 0;
    }  else {
        water_volume = 1-(((distance_surface - vars.distance_max_volume.toInt())/deltad));
    }

    // waterflow sensor  
    //Serial.println(">>> main loop: waterflow sensor");
    sensorWaterflow.loop();
    flow_rate = sensorWaterflow.getFlowRate();
    int flow_count = sensorWaterflow.getFlowCount();

    //establishes pubInterval based on flow_rate
    //Serial.println(">>> main loop: publish results");
    if(vars.refresh_rate.toInt()>1000) pubInterval = vars.refresh_rate.toInt() * 1000;
    int shortInterval = pubInterval;
    if(flow_rate>0) shortInterval = 1000;
    if (curMillis - prevMillis > shortInterval)
    {
        // Publishing results 
        if(isMQTTConnected()){
            String json = String("{") + "\"device_id\":\"" + vars.device_id + "\",\"data\":{\"tank_limit\":" + tank_limit + ",\"flow_count\":" + flow_count + ",\"flow_rate\":" + flow_rate + ",\"distance_surface\":" + distance_surface + ", \"water_vol\":" + water_volume + ",\"water_temp\":" + tempC + ", \"temp\":"+temp+", \"hum\":"+hum+", \"press\":"+pres+"}}";
            //String json = String("{") + "\"device_id\":\"" + vars.device_id + "\",\"data\":{\"tank_limit\":" + tank_limit + ",\"flow_count\":" + flow_count + ",\"flow_rate\":" + flow_rate + ",\"water_temperature\":" + tempC + ", \"temperature\":"+temp+", \"humidity\":"+hum+", \"pressure\":"+pres+"}}";
            Serial.println(json);
            mqttClient.publish_mqtt((char*)json.c_str());
        }
        
        //closing loop
        prevMillis = curMillis;
    }


    //loop basic modules
    //Serial.println(">>> main loop component loops");
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

void App::tankLimitReached(boolean t){
    tank_limit = t;
}

void App::setMonitorLed(String led, String status){
    if(led=="led1") ledMonitor.led1(status);
    else if(led=="led2") ledMonitor.led2(status);
}