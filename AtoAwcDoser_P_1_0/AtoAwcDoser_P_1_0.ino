/*
 * Mark Rood
 * 
 * Wireless Doser
 * Send http commands over wifi to control pumps
 * Pin 14 Blue
 * Pin 18 Green
 * Pin 19 Yellow
 * PIn 21 Purple
 * 
 * example test command is browser http://192.168.1.175/command?pump=blue&&value=1
 * 
 * Board: ESP32 Dev Module
*/

//#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ESPAsyncWiFiManager.h>         //https://github.com/tzapu/WiFiManager
//#include <WiFiManager.h>
#include <ESPmDNS.h>
#include <WebSerial.h>



AsyncWebServer server(80);
DNSServer dns;

//WiFiManager wifiManager;
void configModeCallback (AsyncWiFiManager *myWiFiManager);

int pumpStartedTime = 0;
int pumpNowTime = 0;
int pumpElaspeTime = 0;
int MAXPUMPRUN = 120000;
int pingReturn = 0;
bool dosePing = true;

void motor(String pump, int value);
void handleCommand();
void handlePing();
int count = 0;


void setup(void) {
  Serial.begin(115200);  
  ///////////////////Start WiFi ///////////////////////////////////////
  //WiFiManager
  //Local intialization. Once its business is done, there is no need to keep it around
  AsyncWiFiManager wifiManager(&server, &dns);
  //reset settings - for testing
  //wifiManager.resetSettings();
  wifiManager.setSTAStaticIPConfig(IPAddress(192,168,1,175), IPAddress(192,168,1,1), IPAddress(255,255,255,0), IPAddress(192,168,1,1), IPAddress(192,168,1,1));
  //set callback that gets called when connecting to previous WiFi fails, and enters Access Point mode
  wifiManager.setAPCallback(configModeCallback);

  //fetches ssid and pass and tries to connect
  //if it does not connect it starts an access point with the specified name
  //here  "AutoConnectAP"
  //and goes into a blocking loop awaiting configuration
  if (!wifiManager.autoConnect("DOSER")) {
    Serial.println("failed to connect and hit timeout");
    Serial.println("restarting esp");
    //reset and try again, or maybe put it to deep sleep
    ESP.restart();
    delay(1000);
  }
  delay(50);
  //Serial.print("FreeHeap is :");
  //Serial.println(ESP.getFreeHeap());
  delay(50);
  //if you get here you have connected to the WiFi
  Serial.println("connected...yeey :)");

  Serial.println("local ip");
  Serial.println(WiFi.localIP());
  //WebSerial.print("local ip is: ");
  //WebSerial.println(WiFi.localIP());

  //server.on("/", handleRoot);
  //server.on("/test.svg", drawGraph);
/*  server.on("/inline", []() {
    server.send(200, "text/plain", "this works as well");
  });*/
//  server.on("/command",handleCommand);
 server.on("/command", HTTP_GET, [](AsyncWebServerRequest * request) { 
  if (request->hasParam("pump")&& request->hasParam("value")) {
    String str;
    String pump = request->getParam("pump")->value();
    int val = request->getParam("value")->value().toInt();
    
    Serial.print("pump is: ");
    Serial.println(pump);
    Serial.print("value is: ");
    Serial.print(val);
    String strVal = String(val);
    request->send(200, "text/plain", "Command Received");
    if(pump == "blue"){
      str = ", D_B_"+strVal;
    }else if(pump == "green"){
      str = ", D_G_"+strVal;
    }else    if(pump == "yellow"){
      str = ", D_Y_"+strVal;
    }else    if(pump == "purple"){
      str = ", D_P_"+strVal;
    }
    WebSerial.print(str);
    motor(pump,val);
  }
 });


 //  server.on("/ping",handlePing);
 server.on("/ping", HTTP_GET, [](AsyncWebServerRequest * request) { 
  if (request->hasParam("ping")) {
    int pingReturn = request->getParam("ping")->value().toInt();
    //stateParamValue= request->getParam(PARAM_INPUT_2)->value();
    if(pingReturn == 1){
      dosePing = true;
      //server.send(200, "text/plain", "1");
      WebSerial.print(", D_P_1");
      request->send(200, "text/plain", "1");
    }else{
      dosePing = false;
      //server.send(200, "text/plain", "0");
      WebSerial.print(", D_P_0");
       request->send(200, "text/plain", "0");
    }
  }
 });
  Serial.println("HTTP server started");
 //server.onNotFound(handleNotFound);
//  server.begin();
  
    if (!MDNS.begin("esp32")) {
        Serial.println("Error setting up MDNS responder!");
        while(1) {
            delay(1000);
        }
    }

  server.begin();
  delay(10);
  WebSerial.begin(&server);
  
  pinMode(2,OUTPUT);
  digitalWrite(2, LOW);
  pinMode(14,OUTPUT);
  digitalWrite(14,LOW);
  pinMode(18,OUTPUT);
  digitalWrite(18,LOW);
  pinMode(19,OUTPUT);
  digitalWrite(19,LOW);
  pinMode(21,OUTPUT);
  digitalWrite(21,LOW);
}

void loop(void) {
delay(500);
digitalWrite(2,HIGH);
delay(500);
digitalWrite(2,LOW);
if(count >= 60){
  count = 0;
  Serial.print("!");
  WebSerial.print("!");
}else{
  count++;
}

}

/*void handleCommand(){
  String pump = server.arg("pump");
  int val = server.arg("value").toInt();
    if (request->hasParam("ping") ){&& request->hasParam("value")) {
    String pump = request->getParam("ping")->value();
    int value = request->getParam("value")->value().toInt();

  Serial.print("pump is: ");
  Serial.println(pump);
  Serial.print("value is: ");
  Serial.print(val);
  server.send(200, "text/plain", "Command Received");
  motor(pump,val);
}*/
void motor(String pump, int value){
  bool pumpRunning = false;
  if(pump == "blue"){
    digitalWrite(19,value);
  }else if(pump == "green"){
    digitalWrite(21, value);
  }else if(pump == "yellow"){
    digitalWrite(18, value);
  }else if(pump == "purple"){
    digitalWrite(14, value);
  }
  
  if(value == 1){
    pumpRunning = true;
    pumpStartedTime = millis();
  }else{
    pumpRunning == false;
  }
  if(pumpRunning){
    pumpNowTime = millis();
    pumpElaspeTime = pumpNowTime - pumpStartedTime;
    if(pumpElaspeTime >= MAXPUMPRUN){
      digitalWrite(14,0);  //pumps can only run as long as MAXPUMPRUN
      digitalWrite(18,0);
      digitalWrite(19,0);
      digitalWrite(21,0);
      pumpRunning = false;
      pumpNowTime = 0;
      pumpElaspeTime = 0;
    }
  }
}

/*void handlePing(){
  pingReturn = server.arg("ping").toInt();
  if(pingReturn == 1){
    dosePing = true;
    server.send(200, "text/plain", "1");
  }else{
    dosePing = false;
    server.send(200, "text/plain", "0");
  }
  if (request->hasParam("ping")) {
    int pingReturn = request->getParam("ping")->value().toInt();
    //stateParamValue= request->getParam(PARAM_INPUT_2)->value();
  if(pingReturn == 1){
    dosePing = true;
    server.send(200, "text/plain", "1");
  }else{
    dosePing = false;
    server.send(200, "text/plain", "0");
  }
}*/

//////////////////////////////////////////////////////////////
// call back needed for wifi
/////////////////////////////////////////////////////////////
void configModeCallback (AsyncWiFiManager *myWiFiManager) {
  Serial.println("Entered config mode");
  //myWiFiManager->startConfigPortal("ATOAWC");
  //myWiFiManager->autoConnect("DOSER");
  Serial.println(WiFi.softAPIP());
  //if you used auto generated SSID, print it
  Serial.println(myWiFiManager->getConfigPortalSSID());

}
