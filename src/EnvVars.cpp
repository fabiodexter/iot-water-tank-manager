#include <EnvVars.h>
#include <LittleFS.h>
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

        device_id = readFile(LittleFS, device_id_path);
        Serial.println(">> device_id set to: " + device_id);

        ssid = readFile(LittleFS, ssid_path);
        pass = readFile(LittleFS, pass_path);
        gateway = readFile(LittleFS, gateway_path);

        mqtt_host = readFile(LittleFS, mqtt_host_path);
        mqtt_port = readFile(LittleFS, mqtt_port_path);
        mqtt_user = readFile(LittleFS, mqtt_user_path);
        mqtt_pass = readFile(LittleFS, mqtt_pass_path);
        
        
        refresh_rate = readFile(LittleFS, refresh_rate_path);
        distance_min_volume = readFile(LittleFS, distance_min_volume_path);
        distance_max_volume = readFile(LittleFS, distance_max_volume_path);
         
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

