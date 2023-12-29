#include <Arduino.h>
#include <App.h>
#include <LedMonitor.h>


#define POWER_LED 2
#define WIFI_RED 16
#define WIFI_GREEN 4  
#define MQTT_RED 18
#define MQTT_GREEN 17
#define AP_MODE_SWITCH 19    



LedMonitor::LedMonitor(){}
LedMonitor::LedMonitor(int pin)
{
    this->pin = pin;
    
    pinMode(POWER_LED, OUTPUT);
    pinMode(WIFI_GREEN, OUTPUT);
    pinMode(WIFI_RED, OUTPUT);  
    pinMode(MQTT_RED, OUTPUT);
    pinMode(MQTT_GREEN, OUTPUT);
    pinMode(AP_MODE_SWITCH, INPUT);    

    digitalWrite(POWER_LED, HIGH);
    digitalWrite(WIFI_GREEN, LOW);
    digitalWrite(WIFI_RED, LOW);
    digitalWrite(MQTT_RED, LOW);
    digitalWrite(MQTT_GREEN, LOW);
}

void LedMonitor::setParent(App *_parent){
    parent = _parent;
}


void LedMonitor::led1(String status){
    this->led1_status = status;

    if(this->led1_status=="OFF"){
        digitalWrite(WIFI_GREEN, LOW);
        digitalWrite(WIFI_RED, LOW);
    }
    else if(this->led1_status=="ERROR"){
        digitalWrite(WIFI_GREEN, LOW);
        digitalWrite(WIFI_RED, HIGH);
    }
    else if(this->led1_status=="CONNECTING"){
        digitalWrite(WIFI_RED, LOW);
        digitalWrite(WIFI_GREEN, HIGH);
        delay(250);
        digitalWrite(WIFI_GREEN, LOW);     
    }
    else if(this->led1_status=="CONNECTED"){
        digitalWrite(WIFI_RED, LOW);
        digitalWrite(WIFI_GREEN, HIGH);
    }    
    else if(this->led1_status=="DATA"){
        digitalWrite(WIFI_RED, LOW);
        digitalWrite(WIFI_GREEN, HIGH);
        delay(250);
        digitalWrite(WIFI_GREEN, LOW);
    }    

};


void LedMonitor::led2(String status){
    this->led2_status = status;

    if(this->led2_status=="OFF"){
        digitalWrite(MQTT_GREEN, LOW);
        digitalWrite(MQTT_RED, LOW);
    }
    else if(this->led2_status=="ERROR"){
        digitalWrite(MQTT_GREEN, LOW);
        digitalWrite(MQTT_RED, HIGH);
    }
    else if(this->led2_status=="CONNECTING"){
        digitalWrite(MQTT_RED, LOW);
        digitalWrite(MQTT_GREEN, HIGH);
        delay(250);
        digitalWrite(MQTT_GREEN, LOW);     
    }
    else if(this->led2_status=="CONNECTED"){
        digitalWrite(MQTT_RED, LOW);
        digitalWrite(MQTT_GREEN, HIGH);
    }    
    else if(this->led2_status=="DATA"){
        digitalWrite(MQTT_GREEN, LOW);
        digitalWrite(MQTT_RED, LOW);
        delay(250);
        digitalWrite(MQTT_GREEN, HIGH);
    }    

};




String LedMonitor::led1(){
    return this->led1_status;
};
String LedMonitor::led2(){
    return this->led2_status;
};


void LedMonitor::loop()
{


    this->curMillis = millis();

    //Serial.println("lED mONITOR lOOP");
    //Serial.println(digitalRead(19));
    if(!digitalRead(AP_MODE_SWITCH)) parent->startAPMode();

    if (this->curMillis - this->prevMillis > this->refreshRate)
    {
        this->prevMillis = this->curMillis;
    }
}



