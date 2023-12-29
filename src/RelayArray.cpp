#include <Arduino.h>
#include <App.h>
#include <RelayArray.h>
#include "PCF8574.h"



// Set the i2c HEX address
PCF8574 pcf8574A(0x20);
PCF8574 pcf8574B(0x21);
PCF8574 pcf8574C(0x22);
PCF8574 pcf8574D(0x23);
PCF8574 pcf8574E(0x24);
PCF8574 pcf8574F(0x25);

RelayArray::RelayArray(){}

RelayArray::RelayArray(int pin)
{
    this->pin = pin;

    Serial.println("- > RelayArray");

    //pcf8574A
    pcf8574A.pinMode(P0, OUTPUT);
    pcf8574A.pinMode(P1, OUTPUT);
    pcf8574A.pinMode(P2, OUTPUT);
    pcf8574A.pinMode(P3, OUTPUT);
    pcf8574A.pinMode(P4, OUTPUT);
    pcf8574A.pinMode(P5, OUTPUT);
    pcf8574A.pinMode(P6, OUTPUT);
    pcf8574A.pinMode(P7, OUTPUT);   

    pcf8574B.pinMode(P0, OUTPUT);
    pcf8574B.pinMode(P1, OUTPUT);
    pcf8574B.pinMode(P2, OUTPUT);
    pcf8574B.pinMode(P3, OUTPUT);
    pcf8574B.pinMode(P4, OUTPUT);
    pcf8574B.pinMode(P5, OUTPUT);
    pcf8574B.pinMode(P6, OUTPUT);
    pcf8574B.pinMode(P7, OUTPUT);   

    pcf8574C.pinMode(P0, OUTPUT);
    pcf8574C.pinMode(P1, OUTPUT);
    pcf8574C.pinMode(P2, OUTPUT);
    pcf8574C.pinMode(P3, OUTPUT);
    pcf8574C.pinMode(P4, OUTPUT);
    pcf8574C.pinMode(P5, OUTPUT);
    pcf8574C.pinMode(P6, OUTPUT);
    pcf8574C.pinMode(P7, OUTPUT);    

    pcf8574D.pinMode(P0, OUTPUT);
    pcf8574D.pinMode(P1, OUTPUT);
    pcf8574D.pinMode(P2, OUTPUT);
    pcf8574D.pinMode(P3, OUTPUT);
    pcf8574D.pinMode(P4, OUTPUT);
    pcf8574D.pinMode(P5, OUTPUT);
    pcf8574D.pinMode(P6, OUTPUT);
    pcf8574D.pinMode(P7, OUTPUT);    

    pcf8574E.pinMode(P0, OUTPUT);
    pcf8574E.pinMode(P1, OUTPUT);
    pcf8574E.pinMode(P2, OUTPUT);
    pcf8574E.pinMode(P3, OUTPUT);
    pcf8574E.pinMode(P4, OUTPUT);
    pcf8574E.pinMode(P5, OUTPUT);
    pcf8574E.pinMode(P6, OUTPUT);
    pcf8574E.pinMode(P7, OUTPUT);    

    pcf8574F.pinMode(P0, OUTPUT);
    pcf8574F.pinMode(P1, OUTPUT);
    pcf8574F.pinMode(P2, OUTPUT);
    pcf8574F.pinMode(P3, OUTPUT);
    pcf8574F.pinMode(P4, OUTPUT);
    pcf8574F.pinMode(P5, OUTPUT);
    pcf8574F.pinMode(P6, OUTPUT);
    pcf8574F.pinMode(P7, OUTPUT);            





    //pcf8574A.begin();
    /* 
    pcf8574A.digitalWrite(P0, LOW);
    pcf8574A.digitalWrite(P1, HIGH);
    pcf8574A.digitalWrite(P2, HIGH);
    pcf8574A.digitalWrite(P3, HIGH);
    pcf8574A.digitalWrite(P4, HIGH);
    pcf8574A.digitalWrite(P5, HIGH);
    pcf8574A.digitalWrite(P6, HIGH);
    pcf8574A.digitalWrite(P7, HIGH);    

/*
    //pcf8574B
    pcf8574B.pinMode(P0, OUTPUT);
    pcf8574B.pinMode(P1, OUTPUT);
    pcf8574B.pinMode(P2, OUTPUT);
    pcf8574B.pinMode(P3, OUTPUT);
    pcf8574B.pinMode(P4, OUTPUT);
    pcf8574B.pinMode(P5, OUTPUT);
    pcf8574B.pinMode(P6, OUTPUT);
    pcf8574B.pinMode(P7, OUTPUT);    
    pcf8574B.begin();
    pcf8574B.digitalWrite(P0, HIGH);
    pcf8574B.digitalWrite(P1, LOW);
    pcf8574B.digitalWrite(P2, HIGH);
    pcf8574B.digitalWrite(P3, HIGH);
    pcf8574B.digitalWrite(P4, HIGH);
    pcf8574B.digitalWrite(P5, HIGH);
    pcf8574B.digitalWrite(P6, HIGH);
    pcf8574B.digitalWrite(P7, HIGH);    


    //pcf8574C
    pcf8574C.pinMode(P0, OUTPUT);
    pcf8574C.pinMode(P1, OUTPUT);
    pcf8574C.pinMode(P2, OUTPUT);
    pcf8574C.pinMode(P3, OUTPUT);
    pcf8574C.pinMode(P4, OUTPUT);
    pcf8574C.pinMode(P5, OUTPUT);
    pcf8574C.pinMode(P6, OUTPUT);
    pcf8574C.pinMode(P7, OUTPUT);    
    pcf8574C.begin();
    pcf8574C.digitalWrite(P0, HIGH);
    pcf8574C.digitalWrite(P1, HIGH);
    pcf8574C.digitalWrite(P2, LOW);
    pcf8574C.digitalWrite(P3, HIGH);
    pcf8574C.digitalWrite(P4, HIGH);
    pcf8574C.digitalWrite(P5, HIGH);
    pcf8574C.digitalWrite(P6, HIGH);
    pcf8574C.digitalWrite(P7, HIGH);    


    //pcf8574D
    pcf8574D.pinMode(P0, OUTPUT);
    pcf8574D.pinMode(P1, OUTPUT);
    pcf8574D.pinMode(P2, OUTPUT);
    pcf8574D.pinMode(P3, OUTPUT);
    pcf8574D.pinMode(P4, OUTPUT);
    pcf8574D.pinMode(P5, OUTPUT);
    pcf8574D.pinMode(P6, OUTPUT);
    pcf8574D.pinMode(P7, OUTPUT);    
    pcf8574D.begin();
    pcf8574D.digitalWrite(P0, HIGH);
    pcf8574D.digitalWrite(P1, HIGH);
    pcf8574D.digitalWrite(P2, HIGH);
    pcf8574D.digitalWrite(P3, LOW);
    pcf8574D.digitalWrite(P4, HIGH);
    pcf8574D.digitalWrite(P5, HIGH);
    pcf8574D.digitalWrite(P6, HIGH);
    pcf8574D.digitalWrite(P7, HIGH);    


    //pcf8574E
    pcf8574E.pinMode(P0, OUTPUT);
    pcf8574E.pinMode(P1, OUTPUT);
    pcf8574E.pinMode(P2, OUTPUT);
    pcf8574E.pinMode(P3, OUTPUT);
    pcf8574E.pinMode(P4, OUTPUT);
    pcf8574E.pinMode(P5, OUTPUT);
    pcf8574E.pinMode(P6, OUTPUT);
    pcf8574E.pinMode(P7, OUTPUT);    
    pcf8574E.begin();
    pcf8574E.digitalWrite(P0, HIGH);
    pcf8574E.digitalWrite(P1, HIGH);
    pcf8574E.digitalWrite(P2, HIGH);
    pcf8574E.digitalWrite(P3, HIGH);
    pcf8574E.digitalWrite(P4, LOW);
    pcf8574E.digitalWrite(P5, HIGH);
    pcf8574E.digitalWrite(P6, HIGH);
    pcf8574E.digitalWrite(P7, HIGH);    


    //pcf8574F
    pcf8574F.pinMode(P0, OUTPUT);
    pcf8574F.pinMode(P1, OUTPUT);
    pcf8574F.pinMode(P2, OUTPUT);
    pcf8574F.pinMode(P3, OUTPUT);
    pcf8574F.pinMode(P4, OUTPUT);
    pcf8574F.pinMode(P5, OUTPUT);
    pcf8574F.pinMode(P6, OUTPUT);
    pcf8574F.pinMode(P7, OUTPUT);    
    pcf8574F.begin();
    pcf8574F.digitalWrite(P0, HIGH);
    pcf8574F.digitalWrite(P1, HIGH);
    pcf8574F.digitalWrite(P2, HIGH);
    pcf8574F.digitalWrite(P3, HIGH);
    pcf8574F.digitalWrite(P4, HIGH);
    pcf8574F.digitalWrite(P5, LOW);
    pcf8574F.digitalWrite(P6, HIGH);
    pcf8574F.digitalWrite(P7, HIGH);    


*/


}

void RelayArray::setParent(App *_parent){
    parent = _parent;
}

void RelayArray::setRelay(int module, int relay, int status){

    if(module == 0) { pcf8574A.digitalWrite(relay, status);}
    else if(module == 1) { pcf8574B.digitalWrite(relay, status);}
    else if(module == 2) { pcf8574C.digitalWrite(relay, status);}
    else if(module == 3) { pcf8574D.digitalWrite(relay, status);}
    else if(module == 4) { pcf8574E.digitalWrite(relay, status);}
    else if(module == 5) { pcf8574F.digitalWrite(relay, status);}
}



void RelayArray::reset()
{
    pcf8574A.begin(); 
    pcf8574B.begin(); 
    pcf8574C.begin(); 
    pcf8574D.begin(); 
    pcf8574E.begin(); 
    pcf8574F.begin(); 
}



int firstExec = 0;
void RelayArray::loop()
{


    this->curMillis = millis();
    if (this->curMillis - this->prevMillis > this->refreshRate)
    {
        if(firstExec==0){ //first execution
            reset();
        }
        this->prevMillis = this->curMillis;
        firstExec++;
    }
}




