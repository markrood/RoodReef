/*
  ATOAWC_UTIL.h - Library supporting atoawc.
  Created by Mark Rood, Markch 9, 2021.
  Not Released into the public domain.
*/
#ifndef ATOAWCUTIL_32_h
#define ATOAWCUTIL_32_h



#include "Arduino.h"
#include <FS.h>




//#include <DNSServer.h>
//#include <DNSServer.h>
//#include <WiFi.h>
#include <Wire.h>
#include <ArduinoJson.h>
//#include "ESPAsyncWebServer.h"
//#include <NTPClient.h>
#include "SPIFFS.h"
#include "datetime.h"
#include <WebSerial.h>

//#include <ESP8266mDNS.h>

class AtoAwcUtil
{
  public:
    AtoAwcUtil(AsyncWebServer *server);
    float recordRun(String whatsRunning, bool startIt);
    void storeFillTime(String type, float fillTime);
    String readFile(fs::FS &fs, const char * path);
    void writeFile(fs::FS &fs, const char * path, const char * message);
    void startSpiffs();
    int sendHttp(String event);
    bool checkIfRanTooLong(String type, int runTime);
    float averageSinceReset(String type,float total,float val);
    void setAtoRunning(bool val);
    bool getAtoRunning();
//    NTPClient*  getTc();
    //AsyncWebServer getServer();
    String changeMonthNubToStr(int thisMonth);
    String changeMonthStrToNum(String thisMonth);
    int* setCurrentTime();
    void setUtcOffsetInSeconds(int utcOffsetInSec);
    bool checkStoreRun(String function, float elapse);
    void webSerialWrite(String text);
    void webSerialWriteLine(String text);
    void beginUtil();
    void recvMsgS(uint8_t *data, size_t len);
    void setTimeZone(String tz);
    bool isDaylightSavingsTime();
    void setUtcOffsetInSec(int offset);
    void setCalibrating(bool cal);
    int* getCurrentTimeArray();
    void clearSpiffs();
    void listAllFiles();
    
    
    
  private:
    AsyncWebServer *_server;
//    NTPClient *_timeClient;
    WiFiUDP ntpUDP;
    WiFiClient client;
    Datetime* _datetime;
    String iPAddress;
    const char* host = "maker.ifttt.com";
    const int httpsPort = 80;
    
    String url = "";
    int aveCount = 0;
    int aveAwcEmtyCount = 0;
    int aveAwcFillCount = 0;
    bool atoRunning;
    bool awcEmptyRunning;
    bool awcFillRunning;
    int currentYear =0;
    int currentMonth = 0;
    int currentDay = 0;
    int currentHour = 0;
    int currentMinute = 0;
    long utcOffsetInSeconds = 0;   
    int currentTimeArr[5];
    int *curTimeArr;
    int currentTimeArrMinusOneMin[5];// buffer for minute time
    int currentTimeArrPlusOneMin[5];//buffer for minute timer
    int blueStart = 0;
    int blueStop = 0;
    int greenStart = 0;
    int greenStop = 0;
    int yellowStart = 0;
    int yellowStop = 0;
    int purpleStart = 0;
    int purpleStop = 0;
    float oneHundredmlBlueTime = 0;
    float oneHundredmlGreenTime = 0;
    float oneHundredmlYellowTime = 0;
    float oneHundredmlPurpleTime = 0;
    float blueDailyFill = 0;
    float greenDailyFill = 0;
    float yellowDailyFill = 0;
    float purpleDailyFill = 0;
    bool calibrating = false;
    String daileybdFill = "";
    String daileygdFill = "";
    String daileyydFill = "";
    String daileypdFill = "";
    const char* ssid = "RoodRouter";
    const char* password = "";
    double currentFill = 0;
    double currentAEmpty = 0;
    double currentAFill = 0;
    unsigned long atoStop = 0;
    unsigned long atoStart = 0;
    int awcEmptyStop = 0;
    int awcEmptyStart = 0;
    int awcFillStop = 0;
    int awcFillStart = 0;


};    
#endif
