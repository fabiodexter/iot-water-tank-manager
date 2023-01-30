#include <Arduino.h>
#include <App.h>
#include "PCF8574.h"

// Set the i2c HEX address
PCF8574 pcf8574(0x20);

LedMonitor::LedMonitor(){}

LedMonitor::LedMonitor(int pin)
{
    this->pin = pin;
    //pinMode(this->pin, OUTPUT);
    pcf8574.pinMode(P0, OUTPUT);
    pcf8574.pinMode(P1, OUTPUT);
    pcf8574.pinMode(P2, OUTPUT);
    pcf8574.pinMode(P3, OUTPUT);
    pcf8574.pinMode(P4, OUTPUT);
    pcf8574.pinMode(P5, OUTPUT);
    pcf8574.pinMode(P6, OUTPUT);
    pcf8574.pinMode(P7, OUTPUT);    
    pcf8574.begin();
    pcf8574.digitalWrite(P0, LOW);
    pcf8574.digitalWrite(P1, LOW);
    pcf8574.digitalWrite(P2, LOW);
    pcf8574.digitalWrite(P3, LOW);
    pcf8574.digitalWrite(P4, LOW);
    pcf8574.digitalWrite(P5, LOW);
    pcf8574.digitalWrite(P6, LOW);
    pcf8574.digitalWrite(P7, LOW);



}



void LedMonitor::led1(String status){
    this->led1_status = status;
    if(this->led1_status=="OFF"){
        pcf8574.digitalWrite(P0, LOW);
        pcf8574.digitalWrite(P1, LOW);
    }
    else if(this->led1_status=="ERROR"){
        pcf8574.digitalWrite(P0, HIGH);
        pcf8574.digitalWrite(P1, LOW);
    }
    else if(this->led1_status=="CONNECTING"){
        pcf8574.digitalWrite(P0, HIGH);
        pcf8574.digitalWrite(P1, HIGH);
        delay(250);
        pcf8574.digitalWrite(P0, LOW);
        pcf8574.digitalWrite(P1, LOW);
    }
    else if(this->led1_status=="CONNECTED"){
        pcf8574.digitalWrite(P0, LOW);
        pcf8574.digitalWrite(P1, HIGH);
    }

    
};
void LedMonitor::led2(String status){
    this->led2_status = status;
    if(this->led2_status=="ERROR"){
        pcf8574.digitalWrite(P2, HIGH);
        pcf8574.digitalWrite(P3, LOW);
    }
    else if(this->led3_status=="DATA"){
        pcf8574.digitalWrite(P2, LOW);
        pcf8574.digitalWrite(P3, HIGH);
    }

};
void LedMonitor::led3(String status){
    this->led3_status = status;
    if(this->led3_status=="ERROR"){
        pcf8574.digitalWrite(P4, HIGH);
        pcf8574.digitalWrite(P5, LOW);
    }
    else if(this->led3_status=="DATA"){
        pcf8574.digitalWrite(P4, LOW);
        pcf8574.digitalWrite(P5, HIGH);
    }

};  


String LedMonitor::led1(){
    return this->led1_status;
};
String LedMonitor::led2(){
    return this->led2_status;
};
String LedMonitor::led3(){
    return this->led3_status;
};  


void LedMonitor::loop()
{
    this->curMillis = millis();

    if (this->curMillis - this->prevMillis > this->refreshRate)
    {

        /*
        if(this->led1_status == "ERROR"){
            if(pcf8574.digitalRead(P0)==LOW){
                pcf8574.digitalWrite(P0, HIGH);
            }else{
                pcf8574.digitalWrite(P0, LOW);
            }
        }

        if(this->led2_status == "ERROR"){
            if(pcf8574.digitalRead(P2)==LOW){
                pcf8574.digitalWrite(P2, HIGH);
            }else{
                pcf8574.digitalWrite(P2, LOW);
            }
        }

        if(this->led3_status == "ERROR"){
            if(pcf8574.digitalRead(P4)==LOW){
                pcf8574.digitalWrite(P4, HIGH);
            }else{
                pcf8574.digitalWrite(P4, LOW);
            }
        }
        */



        this->prevMillis = this->curMillis;
    }
}



