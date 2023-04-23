#include <WifiManager.h>

String wifi_status = "";

WifiManager::WifiManager(){

}


void WifiManager::reconnect(){

    wifi_status =  "connecting";

    //if(this->ssid=="" || this->pass=="") {
        //this->parent->getWebServer.initAPMode(this->device_id);
       // return;
    //}

    WiFi.disconnect(true);
    WiFi.begin(this->ssid, this->pass);

    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.println(">> Connecting to WiFi...");
        parent->getLedMonitor().led1("CONNECTING");
        delay(5000);
    }

    Serial.print(">> WiFi connected! IP Address: ");
    Serial.println(WiFi.localIP());    
    WiFi.setHostname(this->device_id);
    Serial.println(">> hostname: " + WiFi.hostname());
    delay(1000);
    parent->getLedMonitor().led1("CONNECTED");    
    wifi_status = "connected";
    //starting webserver
    //this->parent->getWebServer.startWebserver();
}


void WifiManager::loop()
{
    if(wifi_status == "connecting") {
        return;
    }

    if(wifi_status != "connected") {
        reconnect();
        return;
    }
}


void WifiManager::setParent(App *_parent)
{
    this->parent = _parent;
}

void WifiManager::setParams(char* device_id, char* ssid,char* gateway,char* pass)
{
    this->device_id = device_id;
    this->ssid = ssid;
    this->gateway = gateway;
    this->pass = pass;
}

bool WifiManager::isConnected()
{
    if(WiFi.status() == WL_CONNECTED) return true;
    else return false;
}

