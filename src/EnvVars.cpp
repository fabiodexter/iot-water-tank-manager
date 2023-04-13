#include <EnvVars.h>
#include "LittleFS.h"
#include <Arduino.h>

// instantiating EnvVars
EnvVars::EnvVars(){

}



// Initialize LittleFS
void EnvVars::initFS()
{
    Serial.println(">> initfs");

    if (!LittleFS.begin())
    {
        Serial.println(">> An error has occurred while mounting LittleFS");
    }
    else
    {
        Serial.println(">> LittleFS mounted successfully");

        device_id = readFile(LittleFS, device_idPath);
        Serial.println(">> device_id set to: " + device_id);

        ssid = readFile(LittleFS, ssidPath);
        pass = readFile(LittleFS, passPath);
        gateway = readFile(LittleFS, gatewayPath);

        mqtt_host = readFile(LittleFS, mqtt_hostPath);
        mqtt_port = readFile(LittleFS, mqtt_portPath);
        mqtt_user = readFile(LittleFS, mqtt_userPath);
        mqtt_pass = readFile(LittleFS, mqtt_passPath);


         
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

