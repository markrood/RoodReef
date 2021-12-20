
#include "dosing.h"


Dosing::Dosing(AtoAwcUtil *atoAwcUtil, WiFiClient client)
{
  _atoAwcUtil = atoAwcUtil;
  _client = client;
  Serial.begin(115200);

 }

 /* WiFi.mode(WIFI_STA);
    // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Trasnmitted packet
  esp_now_register_send_cb(onDataSent);
  
  // Register peer
  esp_now_peer_info_t peerInfo;
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  
  // Add peer        
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }*/



////////////////////////////////////////////////////////////////////
//
//  Fuction: dose
//
//  Input:  String pump ("blue", "green", "yellow"
//
//  Output:  none
//
//  Discription:  Doses the correct pump (blue, green, yellow) for
//                the save amonth of time for the slider setting based on 
//                the hundred ml calibration saved earlier.
//
//  Calls:  recordRordRun(). sendJson(), storeFillTime(), storeFillTime()
//
//  SPIFFS:  "/hundredMlBlue.txt/green/yellow", "/slider1.txt"/2/3
//
//  TEST_MODE:   yes
//
/////////////////////////////////////////////////////////////////////
void Dosing::dose(String pump){
  String blueCalibration = "";
  String greenCalibration = "";
  String yellowCalibration = "";
  String purpleCalibration = "";
  int blueCal = 0;
  int greenCal =0;
  int yellowCal = 0;
  int purpleCal = 0;
  int blueDosingAmount = 0;
  int greenDosingAmount = 0;
  int yellowDosingAmount = 0;
  int purpleDosingAmount = 0;
  
  if(pump == "blue"){
    blueCalibration = _atoAwcUtil->readFile(SPIFFS, "/blcal.txt");
    blueCal = blueCalibration.toInt();
    String blDosingAmount = _atoAwcUtil->readFile(SPIFFS, "/slider1.txt");
    blueDosingAmount = blDosingAmount.toInt();
    int bluePumpDuration = calculateDosingLength(blueCal, blueDosingAmount);
    int bluePumpDurMs = bluePumpDuration*1000;
    Serial.print("Blue pump duration is ");
    Serial.println(bluePumpDuration);
    sensorValue0 = 1;
    Serial.println("Blue motor running");
    if(!wswBlue7_1){
      _atoAwcUtil->webSerialWrite(", EBD_7_1");
      wswBlue7_1 = true;
    }
    //sendJson();
    sendCommand("blue",1 );
    float blueDose = _atoAwcUtil->recordRun("blue",true);
    delay(bluePumpDurMs);
    sensorValue0 = 0;
//    sendJson();
    sendCommand("blue",0 );
    blueDose = _atoAwcUtil->recordRun("blue",false);
    Serial.println("Blue motor stopped");
    if(!wswBlue7_2){
      _atoAwcUtil->webSerialWrite(", EBD_7_2");
      wswBlue7_2 = true;
    }
    wswBlue7_1 = false;
    wswBlue7_2 = false;
    _atoAwcUtil->storeFillTime("blueDose", blueDosingAmount);
  }else if(pump == "green"){
    greenCalibration = _atoAwcUtil->readFile(SPIFFS, "/gncal.txt"); 
    greenCal = greenCalibration.toInt();   
    String grnDosingAmount = _atoAwcUtil->readFile(SPIFFS, "/slider2.txt");
    greenDosingAmount = grnDosingAmount.toInt();
    int greenPumpDuration = calculateDosingLength(greenCal, greenDosingAmount);
    int greenPumpDurMs = greenPumpDuration*1000;
    Serial.print("Green pump duration is ");
    Serial.println(greenPumpDuration);
    sensorValue1 = 1;
    Serial.println("Green motor running");
    if(!wswGreen8_1){
      _atoAwcUtil->webSerialWrite(", EBG_8_1");
      wswGreen8_1 = true;
    }
  
//    sendJson();
    sendCommand("green",1);

    float greenDose = _atoAwcUtil->recordRun("green",true);
    delay(greenPumpDurMs);
    sensorValue1 = 0;
   // sendJson();
    sendCommand("green",0);
    greenDose = _atoAwcUtil->recordRun("green",false);
    Serial.println("Green motor stopped");
    if(!wswGreen8_2){
      _atoAwcUtil->webSerialWrite(", EBG_8_2");
      wswGreen8_2 = true;
    }
    wswGreen8_1 = false;
    wswGreen8_2 = false;
    _atoAwcUtil->storeFillTime("greenDose", greenDosingAmount);
  }else if(pump == "yellow"){
    yellowCalibration = _atoAwcUtil->readFile(SPIFFS, "/ywcal.txt");
    yellowCal = yellowCalibration.toInt();  
    String ywDosingAmount = _atoAwcUtil->readFile(SPIFFS, "/slider3.txt");
    yellowDosingAmount = ywDosingAmount.toInt();
    int yellowPumpDuration = calculateDosingLength(yellowCal, yellowDosingAmount);
    int yellowPumpDurMs = yellowPumpDuration*1000;
    Serial.print("Yellow pump duration is ");
    Serial.println(yellowPumpDuration);
    sensorValue2 = 1;
    Serial.println("Yellow motor running");
    if(!wswYellow9_1){
      _atoAwcUtil->webSerialWrite(", EBY_9_1");
      wswYellow9_1 = true;
    }
    //sendJson();
    sendCommand("yellow",1);
    float yellowDose = _atoAwcUtil->recordRun("yellow",true);
    delay(yellowPumpDurMs);
    sensorValue2 = 0;
    //sendJson();
    sendCommand("yellow",0);
    yellowDose = _atoAwcUtil->recordRun("yellow",false);
    Serial.println("Yellow motor stopped");
    if(!wswYellow9_2){
      _atoAwcUtil->webSerialWrite(", EBY_9_2");
      wswYellow9_2 = true;
    }
    wswYellow9_1 = false;
    wswYellow9_2 = false;
    _atoAwcUtil->storeFillTime("yellowDose", yellowDosingAmount);
  }else if(pump == "purple"){
    purpleCalibration = _atoAwcUtil->readFile(SPIFFS, "/pwcal.txt");
    purpleCal = purpleCalibration.toInt();  
    String pwDosingAmount = _atoAwcUtil->readFile(SPIFFS, "/slider4.txt");
    purpleDosingAmount = pwDosingAmount.toInt();
    int purplePumpDuration = calculateDosingLength(purpleCal, purpleDosingAmount);
    int purplePumpDurMs = purplePumpDuration*1000;
    Serial.print("Purple pump duration is ");
    Serial.println(purplePumpDuration);
    sensorValue3 = 1;
    Serial.println("Purple motor running");
    if(!wswPurple10_1){
      _atoAwcUtil->webSerialWrite(", EBP_10_1");
      wswPurple10_1 = true;
    }
    //sendJson();
    sendCommand("Purple",1);
    float purpleDose = _atoAwcUtil->recordRun("purple",true);
    delay(purplePumpDurMs);
    sensorValue3 = 0;
    //sendJson();
    sendCommand("purple",0);
    purpleDose = _atoAwcUtil->recordRun("purple",false);
    Serial.println("Purple motor stopped");
    if(!wswPurple10_2){
      _atoAwcUtil->webSerialWrite(", EBP_10_2");
      wswPurple10_2 = true;
    }
    wswPurple10_1 = false;
    wswPurple10_2 = false;
    _atoAwcUtil->storeFillTime("purpleDose", purpleDosingAmount);
  }
}

////////////////////////////////////////////////////////////////////
//
//  Fuction: sendJson
//
//  Input:  none
//
//  Output:  none
//
//  Discription:  call the wireless doser to start and stop
//                blue, green, yellow doser.  The doser is set
//                with sensorValue0, sendorValue1, sensorValue2.
//                these ard set in calibrate() when calibrating
//                and in dosing when dosing. 
//
/////////////////////////////////////////////////////////////////////
void Dosing::sendJson(){
    // Use WiFiClient class to create TCP connections
  
  const char * host = "192.168.1.99";            //default IP address
  const int httpPort = 80;

  if (!_client.connect(host, httpPort)) {
    Serial.println("connection failed");
    //WebSerial.println(",?DCF");
    return;
  }else{
    IPAddress ip = _client.remoteIP();
    Serial.println("ip in Dosing::sendJson is :");
    Serial.print(ip);
  }

    // We now create a URI for the request. Something like /data/?sensor_reading=123
  String url = "/data/";
  url += "?sensor_reading=";
  url +=  "{\"sensor0_reading\":\"sensor0_value\",\"sensor1_reading\":\"sensor1_value\",\"sensor2_reading\":\"sensor2_value\",\"sensor3_reading\":\"sensor3_value\"}";

  url.replace("sensor0_value", String(sensorValue0));
  url.replace("sensor1_value", String(sensorValue1));
  url.replace("sensor2_value", String(sensorValue2));
  url.replace("sensor3_value", String(sensorValue3));

   //Serial.println("sending :"+url);

  // This will send the request to the server
  _client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Connection: close\r\n\r\n");
  unsigned long timeout = millis();
  while (_client.available() == 0) {
    if (millis() - timeout > 5000) {
      Serial.println(">>> Client Timeout !");
      _client.stop();
      return;
    }
  }
}

//////////////////////////////
//  callback
///////////////////////////////
      // callback when data is sent
    void Dosing::onDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
      Serial.print("\r\nLast Packet Send Status:\t");
      Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
    }



/////////////////////////////////////////
//  Fucntion: sendCommand
//
//  input:  pump, value
//
//  outupt command
///////////////////////////////////////////////
void Dosing::sendCommand(String pump, int value){
  clientConnect = client.connect(hostIP,80);
  while(!clientConnect || clientConnectTry >= 10){
    delay(100);
    clientConnect = client.connect(hostIP,80);
    clientConnectTry++;
    Serial.print("^");
    WebSerial.print("^");
  }
  if(clientConnect){
    clientConnectTry = 0;
    String url = "/command?pump=";
    url += pump;
    url += "&&value=";
    url += value;
    Serial.print("url is: ");
    Serial.println(url);
    client.print(String("GET ")+ url +" HTTP/1.1\r\n" + "Host: "+ hostIP + "\r\n"+
      "Connection: keep-alive\r\n\r\n");
      delay(10);
      Serial.print("Response: ");
      while(client.available()){
        String line = client.readStringUntil('/r');
        Serial.print(line);
    }

    Serial.println(url);
  
  
    }else{
    Serial.println("client connect failed");
    }
}

/////////////////////////////////////////
//  Fucntion: sendPing
//
//  input:  
//
//  outupt bool 
///////////////////////////////////////////////
bool Dosing::sendPing(){
  bool retVal = false;
  String line = "";
  clientConnectPing = client.connect(hostIP,80);
  while(!clientConnectPing || clientConnectTryPing >= 10){
    delay(100);
    clientConnectPing = client.connect(hostIP,80);
    clientConnectTryPing++;
    Serial.print("P^P");
    WebSerial.print("P^P");
  }
  if(clientConnectPing){
    clientConnectTryPing = 0;
  String url = "/ping?ping=1";
  client.print(String("GET ")+ url +" HTTP/1.1\r\n" + "Host: "+ hostIP + "\r\n"+
    "Connection: keep-alive\r\n\r\n");
    delay(5000);
    Serial.print("Response: ");
    while(client.available()){
      line = client.readStringUntil('/r');
      Serial.print(line);
    }
//    line.contains("200");
    if(containsString(line, "200")){
      retVal = true;
    }else{
      retVal = false;
    }
  }
  return retVal;
}
////////////////////////////////////////////////////////////////////
//
//  Fuction: calibrate
//
//  Input:  node
//
//  Output:  none
//
//  Discription:  if chBox for calibrate is checked start and stop.
//                for calibration amount.  IF IT RUNS LESS THAN 10 SECONDS
//                THIS WAS JUST PRIMING PUMP NOT A TRUE CALIBRATION
//                DOESN'T really call recordRun to record the run it 
//                uses recordRun to see if it ran falsely or real calibrate.
//                Real calibration are stored in Spiffs for a 100ml run.
//
//  Calls:  recordRun, sendJson 
//
//  SPIFFS  "/hundredMlBlue.txt", "/hundredMlGreen.txt", "/hundredMlYellow.txt"
//
/////////////////////////////////////////////////////////////////////
void Dosing::calibrate(){ 
  //Serial.println("in the calibration function"); 
  bool startIt = true;
  bool stopIt = false;
 
  if(ckBox1 && !blueStarted){//start blue calibrate
    //Serial.print("->");
    _atoAwcUtil->setCalibrating(true);
    calibrating = true;
    _atoAwcUtil->recordRun("blue", startIt);  //start stopwatch
    blueStarted = true;
    sensorValue0 = 1;
    Serial.println("Blue motor running");
    _atoAwcUtil->webSerialWrite("BMR");
    //sendJson(); //send wifi message to doser
    sendCommand("blue",1);
  }else if(!ckBox1 && blueStarted){// blue stopped
    //Serial.println("blue stopped calibrating");
    _atoAwcUtil->recordRun("blue", stopIt);  //stop the stopwatch
    _atoAwcUtil->setCalibrating(false);
    calibrating = false;
    if(blueStarted){
      blueRan = true;
      blueStarted = false;
      sensorValue0 = 0;
      Serial.println("Blue motor stopped");
      _atoAwcUtil->webSerialWrite("BMS");
      //sendJson();
      sendCommand("blue",0);
    }else{
      ;//TODO big error blue never stated but now got stop
    }
  }
  if(ckBox2 && !greenStarted){
    //Serial.print("->");
    _atoAwcUtil->setCalibrating(true);
    calibrating = true;
    _atoAwcUtil->recordRun("green", startIt);  //start stopwatch
    greenStarted = true;
    sensorValue1 = 1;
    Serial.println("Green motor running");
    _atoAwcUtil->webSerialWrite("GMR");
    //sendJson(); //send wifi message to doser
    sendCommand("green",1);
  }else if(!ckBox2 && greenStarted){// blue stopped
    //Serial.println("blue stopped calibrating");
    _atoAwcUtil->recordRun("green", stopIt);  //stop the stopwatch
    _atoAwcUtil->setCalibrating(false);
    calibrating = false;
    if(greenStarted){
      greenRan = true;
      greenStarted = false;
      sensorValue1 = 0;
      Serial.println("Green motor stopped");
      _atoAwcUtil->webSerialWrite("GMS");
      //sendJson();
      sendCommand("green",0);
    }else{
      ;//TODO big error blue never stated but now got stop
    }
  }
  if(ckBox3 && !yellowStarted){
    //Serial.print("->");
    _atoAwcUtil->setCalibrating(true);
    calibrating = true;
    _atoAwcUtil->recordRun("yellow", startIt);  //start stopwatch
    yellowStarted = true;
    sensorValue2 = 1;
    Serial.println("Yellow motor running");
    _atoAwcUtil->webSerialWrite("YMR");
    //sendJson(); //send wifi message to doser
    sendCommand("yellow",1);
  }else if(!ckBox3 && yellowStarted){// blue stopped
    //Serial.println("blue stopped calibrating");
    _atoAwcUtil->recordRun("yellow", stopIt);  //stop the stopwatch
    _atoAwcUtil->setCalibrating(false);
    calibrating = false;
    if(yellowStarted){
      yellowRan = true;
      yellowStarted = false;
      sensorValue2 = 0;
      Serial.println("Yellow motor stopped");
      _atoAwcUtil->webSerialWrite("YMS");
      //sendJson();
      sendCommand("yellow",0);
    }else{
      ;//TODO big error blue never stated but now got stop
    }
  }
  if(ckBox4 && !purpleStarted){
    //Serial.print("->");
    _atoAwcUtil->setCalibrating(true);
    calibrating = true;
    _atoAwcUtil->recordRun("purple", startIt);  //start stopwatch
    purpleStarted = true;
    sensorValue3 = 1;
    Serial.println("Purple motor running");
    _atoAwcUtil->webSerialWrite("PMR");
    //sendJson(); //send wifi message to doser
    sendCommand("purple",1);
  }else if(!ckBox4 && purpleStarted){//purple  stopped
    //Serial.println("purple stopped calibrating");
    _atoAwcUtil->recordRun("purple", stopIt);  //stop the stopwatch
    _atoAwcUtil->setCalibrating(false);
    calibrating = false;
    if(purpleStarted){
      purpleRan = true;
      purpleStarted = false;
      sensorValue3 = 0;
      Serial.println("Purple motor stopped");
      _atoAwcUtil->webSerialWrite("PMS");
      //sendJson();
      sendCommand("purple",0);
    }else{
      ;//TODO big error blue never stated but now got stop
    }
  }
/*  if(yellowRan){
    if(oneHundredmlYellowTime <= 10){  //this is a real calibrate ran at least 10 seconds
      calibrating = false; 
    }else{
      String oneHunMlYellowTime = String(oneHundredmlYellowTime);
      _atoAwcUtil->writeFile(SPIFFS, "/hundredMlYellow.txt", oneHunMlYellowTime.c_str());
      //Serial.println("set calibrating to false");
      calibrating = false;
    }
    yellowRan = false;
  }
  if(greenRan){
    if(oneHundredmlGreenTime <= 10){  //this is a real calibrate ran at least 10 seconds
      calibrating = false; 
    }else{
      String oneHunMlGreenTime = String(oneHundredmlGreenTime);
      _atoAwcUtil->writeFile(SPIFFS, "/hundredMlGreen.txt", oneHunMlGreenTime.c_str());
      //Serial.println("set calibrating to false");
      calibrating = false;
    }
    greenRan = false;
  }
  if(blueRan){
    if(oneHundredmlBlueTime <= 10){  //this is a real calibrate ran at least 10 seconds
      Serial.println("blue calibration ran too short");
      //WebSerial.println("blue calibration too short for real calibration");
      calibrating = false; 
    }else{
      String oneHunMlBlueTime = String(oneHundredmlBlueTime);
      _atoAwcUtil->writeFile(SPIFFS, "/hundredMlBlue.txt", oneHunMlBlueTime.c_str());
      //Serial.println("wrote the 100 ml time which is "+oneHunMlBlueTime);
      //Serial.println("set calibrating to false");
      calibrating = false;
    }
    //Serial.println("set blueRan to false");
    blueRan = false;
  }*/
  if(ckBox1){
    Serial.print("b->");
    //WebSerial.print("b->");
  }else if (ckBox2){
    Serial.print("g->");
    //WebSerial.print("g->");
  }else if (ckBox3){
    Serial.print("y->");
    //WebSerial.print("y->");
  }else if (ckBox4){
    Serial.print("p->");
    //WebSerial.print("y->");
  }
}

/////////////////////////////////////////////////
//
/////////////////////////////////////////////////////
int Dosing::calculateDosingLength(int cal, int amount){
  return (amount*cal)/100;
}
//NOTE!!!!!!!!!!! type has to be captital (i.e. ATO,) maybe
/* String Dosing::getYearMonthJson(String yr, String mo, String type){
  //String myStr = "/october";
  //jsonAto = _atoAwcUtil->readFile(SPIFFS, myStr.c_str());
  String filename = "/"+mo+type+"Arr.txt";
  String yrMoTypeJson = _atoAwcUtil->readFile(SPIFFS, filename.c_str());
  return yrMoTypeJson;
}*/
//////////////////////////////////////////////////////////////////////
//
//  containsString
//
////////////////////////////////////////////////////////////////////////////////////////////

    bool Dosing::containsString(String str1,String str2){
      bool retVal = false;
      int position = 0;
      int index_str;
      string str1_1 = str1.c_str();
      string str2_2 = str2.c_str();
   
    if (str1_1.find(str2_2) != string::npos) {
      retVal = true;
    }
    return retVal;
    }
//////////////////////////////////////////////////////////////////
    void Dosing::setCkBox1(bool value){
      ckBox1 = value;
    }
    bool Dosing::getCkBox1(){
      return ckBox1;
    }
    void Dosing::setCkBox2(bool value){
      ckBox2 = value;
    }
    bool Dosing::getCkBox2(){
      return ckBox2;
    }
    void Dosing::setCkBox3(bool value){
      ckBox3 = value;
    }
    bool Dosing::getCkBox3(){
      return ckBox3;
    }
    void Dosing::setCkBox4(bool value){
      ckBox4 = value;
    }
    bool Dosing::getCkBox4(){
      return ckBox4;
    }

    bool Dosing::getCalibrating(){
      return calibrating;
    }
    void Dosing::setCalibrating(bool cal){
      calibrating = cal;
      }
