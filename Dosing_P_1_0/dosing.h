/*
  Dosing.h - Library supporting atoawc.
  Created by Mark Rood, Markch 9, 2021.
  Not Released into the public domain.
*/
#ifndef DOSING_h
#define DOSING_h

#include "Arduino.h"
#include "atoawc_util.h";
#include <WiFiClientSecure.h>
//#include <WiFiManager.h>
#include <esp_now.h>
#include <iostream>
#include <string>
using namespace std;

class Dosing
{
  public:
    Dosing(AtoAwcUtil *atoAwcUtil,WiFiClient client);
    void sendJson();
    void calibrate();
    void dose(String pump);
    int calculateDosingLength(int cal, int amount);
    void setCkBox1(bool value);
    bool getCkBox1();
    void setCkBox2(bool value);
    bool getCkBox2();
    void setCkBox3(bool value);
    bool getCkBox3();
    void setCkBox4(bool value);
    bool getCkBox4();
    bool getCalibrating();
    void sendCommand(String pump, int value);
    void setCalibrating(bool cal);

     static void onDataSent(const uint8_t *mac_addr, esp_now_send_status_t status);
     bool sendPing();
    bool containsString(String str1, String str2);
    
  private:
//  bool atoRunning;
    //WiFiManager wifiManager;
    AtoAwcUtil *_atoAwcUtil;
    WiFiClient _client;
    int sensorValue0 = 0;    //sensor value, I'm usingg 0/1 button state
    int sensorValue1 = 0;        
    int sensorValue2 = 0;        
    int sensorValue3 = 0;  
    int sensorValue4 = 0;  
    String slider1;
    String slider2;
    String slider3;
    String slider4;
    String chkboxValue1;
    String chkboxValue2;
    String chkboxValue3;
    String chkboxValue4;
    bool ckBox1 = false;
    bool ckBox2 = false;
    bool ckBox3 = false;
    bool ckBox4 = false;
    bool calibrating = false;
    int oneHundredmlBlueTime = 0;
    int oneHundredmlGreenTime = 0;
    int oneHundredmlYellowTime = 0;
    int oneHundredmlPurpleTime = 0;
    bool blueStarted = false;
    bool greenStarted = false;
    bool yellowStarted = false;
    bool purpleStarted = false;
    bool blueRan = false;
    bool greenRan = false;
    bool yellowRan = false;
    bool purpleRan = false;
    bool wswBlue7_1 = false;
    bool wswBlue7_2 = false;
    bool wswGreen8_1 = false;
    bool wswGreen8_2 = false;
    bool wswYellow9_1 = false;
    bool wswYellow9_2 = false;
    bool wswPurple10_1 = false;
    bool wswPurple10_2 = false;
    WiFiClient client;
    bool clientConnect = false;
    int clientConnectTry = 0;
    bool clientConnectPing = false;
    int clientConnectTryPing = 0;
    const char* hostIP = "192.168.1.175";
    //const char* hostIP = "172.20.10.175";
/*   uint8_t broadcastAddress[6] = {0xF4, 0xCF, 0xA2, 0xED, 0x8F, 0xA3};
  String success = "";
  typedef struct struct_message{
    int pump1;
    int pump2;
    int pump3;
  }struct_message;

  struct_message pumps;*/
  
};

#endif
