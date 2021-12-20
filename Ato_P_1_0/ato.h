/*
  ATO_.h - Library supporting atoawc.
  Created by Mark Rood, Markch 9, 2021.
  Not Released into the public domain.
*/
#ifndef ATO__h
#define ATO__h

#include <atoawc_util.h>
#include <testHarness.h>


class Ato
{
  
  public:
    Ato(AtoAwcUtil *atoAwcUtil, Testharness *testharness, bool debug);
    void doAto();
    bool _debug;
    bool checkLowWater();
    bool checkHighWater();
    bool checkLeaks();
    
    bool getWaterChange();
    void setWaterChange(bool val);
    bool getExecuteAto();
    void setExecuteAto(bool val);
    bool getExecutedAto();
    void setExecutedAto(bool val);
    bool getExecutingAto();
    void setExecutingAto(bool val);
    bool getSentAtoHttp();
    void setSentAtoHttp(bool val);
    bool getItRan();
    void setItRan(bool val);
    //new sensor lights
    void doWaterLights();
    void beginAto();
    bool displaySensorFirst = true;
    int dSTime = 0;
  int dsTimeNow = 0;
  int dsTime=0;
  int dsTimeElapse = 0;
  bool displaySensors = false;

  private:
    AtoAwcUtil *_atoAwcUtil;
    Testharness *_testharness;
    //AsyncWebServer *server;
    bool waterChange = false;
    bool executeAto = false;
    bool executedAto = false;
    bool executingAto = false;
    bool sentAtoHttp = false;
    bool sentWebSerialWrite = false;
    //bool didAto = false;
    float startFill = 0;
    float runningFill = 0;
    int resVal = 0;
     //const int resPin = 34; //analog leak 34 
     bool itRan = false;
     int delayCount = 0; 
     String llevel = "";
     int lklevel = 0;
     int pinTwo = 1;
     bool wswAto1 = false;
     bool wswAto2 = false;

};

#endif
