/*
  ATOAWC_UTIL.h - Library supporting atoawc.
  Created by Mark Rood, Markch 9, 2021.
  Not Released into the public domain.
*/
#ifndef TIMER_32_h
#define TIMER_32_h


#include "Arduino.h"
#include "atoawc_util.h"
//#include <NTPClient.h>
#include <WiFiUdp.h>
//#include <TimeLib.h>                 // Include Arduino time library
//#include <time.h>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>
#include <cstdlib>
using namespace std;

class Timer
{
  public:
    Timer(AtoAwcUtil *atoAwcUtil);
    bool execute(String nextType);
    //void setCurrentTime();
    String arrToString(int *arra);
    String calcNextTime(String nextType, String lastTime);
    void saveNextTime(String nextType, String strTime);
    void tokenizeNextTime(String nexttime, int* ar);
    String convertWebTimeToStrArr(String input);
    String addTime(String curT, int yr, int mo, int dday, int hhour, int m);
    String isSavedTimeGreaterThanCurTime(String savedTime);
    //void setTimeZone(String tz); 
    String  normalizeTimeOfDay(String strTime);
    bool changeOfDay();
    //int getYear();
    //int getMonth();
    //int getDay();
    //int getDates(int val);
    void setCurrentTime();
    //void setUtcOffsetInSeconds(int utcOffsetInSec);
    bool isDaylightSavingsTime();
    void setYrMoDay();
    bool executeTest(String nextType, int howLong);
    void setCurrentDay();
    int counter = 0;
    void setSpiffsLReset(int reset);
    void setSpiffsHWReset(int reset);
    void setSpiffsLWReset(int reset);
    void setSpiffsAtoReset(int reset);
    void setSpiffsAwcReset(int reset);
    void setSpiffsHbReset(int reset);
    void setSpiffsBdReset(int reset);
    void setSpiffsGdReset(int reset);
    void setSpiffsYdReset(int reset);
    void setSpiffsPdReset(int reset);

    
  private:
    AtoAwcUtil *_atoAwcUtil;
    bool leapYear = false;
    int spiffsLReset = 1;
    int spiffsHWReset = 1;
    int spiffsLWReset = 1;
    int spiffsAtoReset = 1;
    int spiffsAwcReset = 1;
    int spiffsHbReset = 1;
    int spiffsBdReset = 1;
    int spiffsGdReset = 1;
    int spiffsYdReset = 1;
    int spiffsPdReset = 1;
  bool sentPrintOnceLN = false;
   bool sentPrintOnceHW = false;
   bool sentPrintOnceLW = false;
   bool sentPrintOnceATON = false;
   bool sentPrintOnceAWCN = false;
   bool sentPrintOnceHBN = false;
   bool sentPrintOnceBDN = false;
   bool sentPrintOnceGDN = false;
   bool sentPrintOnceYDN = false;
   bool sentPrintOncePDN = false;
  int leakNextArr[5];
  int hwNextArr[5];
  int lwNextArr[5];
  int atoNextArr[5];
  int awcNextArr[5];
  int hbNextArr[5];
  int bdNextArr[5];
  int gdNextArr[5];
  int ydNextArr[5];
  int pdNextArr[5];
  int curSaveArr[5];
  int nextArr[5];
  int currentYear =0;
  int currentMonth = 0;
  int currentDay = 0;
  int currentHour = 0;
  int currentMinute = 0;
  int yrMoDay[3];
  int currentYrMoDay[3];
  long utcOffsetInSeconds = 0;   
  int* currentTimeArr;
  int currentTimeArrMinusOneMin[5];// buffer for minute time
  int currentTimeArrPlusOneMin[5];//buffer for minute timer
  //NTPClient *_timeClient;
 // WiFiUDP ntpUDP;
  float testStart = 0;
  bool executingTest = false;
        String lNext = "";
        String hwNext = "";
        String lwNext = "";
        String awNext = "";
        String atNext = "";
        String hbNext = "";
        String bdNext = "";
        String gdNext = "";
        String ydNext = "";
        String pdNext = "";


  int addArr[5];
  char * st;
  
  
};

#endif
