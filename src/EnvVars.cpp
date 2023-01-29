#include <EnvVars.h>
#include "LittleFS.h"
#include <Arduino.h>

// instantiating EnvVars
EnvVars::EnvVars(){

}



// Initialize LittleFS
void EnvVars::initFS()
{
    Serial.println("initfs");

    if (!LittleFS.begin())
    {
        Serial.println("An error has occurred while mounting LittleFS");
    }
    else
    {
        Serial.println("LittleFS mounted successfully");
        ssid = readFile(LittleFS, ssidPath);
        pass = readFile(LittleFS, passPath);
        ip = readFile(LittleFS, ipPath);
        gateway = readFile(LittleFS, gatewayPath);

        Serial.println(ssid);
        Serial.println(pass);
        Serial.println(ip);
        Serial.println(gateway);
    }    
}


// Read File from LittleFS
String EnvVars::readFile(fs::FS &fs, const char *path)
{

    Serial.printf("Reading file: %s\r\n", path);

    File file = fs.open(path, "r");
    if (!file || file.isDirectory())
    {
        Serial.println("- failed to open file for reading");
        return String();
    }

    String fileContent;
    while (file.available())
    {
        fileContent = file.readStringUntil('\n');
        break;
    }
    file.close();
    return fileContent;
}

