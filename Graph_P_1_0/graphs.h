/*
  Graphs - Library supporting atoawc.
  Created by Mark Rood, Markch 9, 2021.
  Not Released into the public domain.
*/
#ifndef GRAPHS_h
#define GRAPHS_h

#include <Arduino.h>
#include "graphs.h"
#include "atoawc_util.h";


class Graphs
{
  public:
    Graphs(AtoAwcUtil *atoAwcUitl);
    bool writeArray(String type,int thisYear,String thisMonth, int thisDay,float value);
    void closeDayPutInArr(String type, float dayAccumulation,int testYear,int testMonth, int testDay);
    String buildYearJson(String type,int testMonth);
    void setCurrentTime();
    void setUtcOffsetInSeconds(int offset);

  private:
//  bool atoRunning;
    AtoAwcUtil *_atoAwcUtil;

    ///////////////json file names ////////
    const char *file_ato = "";
    const char *file_awc = ""; 
    const char *file_blue = ""; 
    const char *file_green = ""; 
    const char *file_yellow = ""; 
    const char *file_purple = ""; 
    int currentYear =0;
    int currentMonth = 0;
    int currentDay = 0;
    int currentHour = 0;
    int currentMinute = 0;
    int utcOffsetInSeconds;
     int *currentTimeArr;
    int currentTimeArrMinusOneMin[5];// buffer for minute time
    int currentTimeArrPlusOneMin[5];//buffer for minute timer
    //NTPClient *_timeClient;
    WiFiUDP ntpUDP;

};

#endif
