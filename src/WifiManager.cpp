#include <WifiManager.h>

String wifi_status = "disconnected";

WifiManager::WifiManager(){
    
}

void WifiManager::setParams(App *_parent, EnvVars vars){
    Serial.println(">> Setting up WifiManager...");
    this->parent = _parent;
    this->device_id = vars.device_id;
    this->ssid = vars.ssid;
    this->gateway = vars.gateway;
    this->pass = vars.pass;
    parent->setMonitorLed("led1","OFF");
}

void WifiManager::reconnect(){

    wifi_status =  "connecting";
    parent->stopWebServer();

    if(this->ssid.isEmpty() || this->pass.isEmpty()) {
        wifi_status = "APMode";
        parent->startAPMode();
        return;
    }

    WiFi.disconnect(true);
    WiFi.begin(this->ssid, this->pass);


    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.println(">> Connecting to WiFi...");
        parent->setMonitorLed("led1","CONNECTING");
        delay(2000);
    }

    Serial.print(">> WiFi connected! IP Address: ");
    Serial.println(WiFi.localIP());    
    WiFi.setHostname((char*) this->device_id.c_str());
    //Serial.println(">> hostname: " + WiFi.hostname());
    //localGateway.fromString(vars.gateway); //not being used yet...dont know why
    parent->setMonitorLed("led1","CONNECTED");
    wifi_status = "connected";
    
    //starting webserver
    parent->startWebServer();
}


void WifiManager::loop()
{
    if(wifi_status == "connecting" || wifi_status == "APMode") {
        return;
    }

    if(wifi_status != "connected") {
        reconnect();
        return;
    }
}


bool WifiManager::isConnected()
{
    if(WiFi.status() == WL_CONNECTED) return true;
    else return false;
}