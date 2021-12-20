#include <atoawc_util.h>

//#include <Arduino.h>
//#include"AtoAwc_Util.h"
#include "awc.h"


Awc::Awc(AtoAwcUtil *atoAwcUtil, Testharness *testharness, bool debug)
{
  _atoAwcUtil = atoAwcUtil; 
  _debug = debug;
  _testharness = testharness; 
}


//////////////////////////////////////////////////////////////
//                                                          //
//   doWaterChange                                          //
//                                                          //
//   input: none                                            //
//                                                          //
//   output: none                                           //
//                                                          //
//   description:  runs the automatic water change (awc)    //
//                  1.)empties water to lower level         //
//                  2.)fills water back up to normal level  //
//                  calculates and stores how long both     //
//                  emty and fill took the first time and   //
//                  uses padded time to determin if either  //
//                  fill or empty took too long.            //
//                                                          //
//////////////////////////////////////////////////////////////
void Awc::doWaterChange(){
  int pinZero = 0; //low sensor
  int pinTwo = 0;  //normal sensor  debug sump pin14, newWater pin27
  if( executingAwc == true){
    sentAwcHttp = true;
    if(!wswAwc1){
      wswAwc1 = true;
      _atoAwcUtil->webSerialWrite("EAWC_4_1");
    }
      if(!emptied) {
      
      executingAwc = true;
      //turn on sump
        if(_debug){
          _testharness->digWrite(14,1);
          //Serial.println("DEBUG sump pump running");
          float empty = _atoAwcUtil->recordRun("awc_empty",true);
        }else{
          digitalWrite(14, 1);  //turn sump pump on
          float empty = _atoAwcUtil->recordRun("awc_empty",true);
        }
          //when emptied
        if(_debug){
          pinZero = _testharness->digRead(0);
        }else{
          pinZero = digitalRead(0);
        }
        if(pinZero == 1) {
            if(_debug){
              _testharness->digWrite(14,0);
              //Serial.println("DEBUG sump pump off water empty");
              float empty = _atoAwcUtil->recordRun("awc_empty",false);
            }else{
              //Serial.println("AWC water is emptied");
              digitalWrite(14, 0);  //turn sump pump off
             // Serial.println("Pump 3 Sump pump is stopped");
              float empty = _atoAwcUtil->recordRun("awc_empty",false);
            }
            emptied = true;
            //Serial.println("emptied = "+emptied);
            //Serial.println("Turn sump pump off");
            if(!wswAwc2){
              wswAwc2 = true;
              _atoAwcUtil->webSerialWrite("EAWC_4_2");
            }
          }
        }
        //fill back up
        if(emptied && !filled) {
        if(_debug){
          pinTwo = _testharness->digRead(2);
        }else{
          pinTwo = digitalRead(2);
        }
          //Serial.println("emptied");
          if(pinTwo == 1) { // normal out of water
            if(!wswAwc3){
              wswAwc3 = true;
             _atoAwcUtil->webSerialWrite("EAWC_4_3");
            }

             if(_debug){
                _testharness->digWrite(27,1);
                //Serial.println("DEBUG filled back up turned New Water pump ON");
                float empty = _atoAwcUtil->recordRun("awc_fill",true);
              }else{
                //Serial.println("AWC fill back up");
                digitalWrite(18, 1);//turn on fill pump
                //Serial.println("Pump 2 new-water pump is started");
                float empty = _atoAwcUtil->recordRun("awc_fill",true);
              }
          }else {
            if(!wswAwc4){
              wswAwc4 = true;
              _atoAwcUtil->webSerialWrite("EAWC_4_4");
            }

             if(_debug){
                _testharness->digWrite(27,0);
                //Serial.println("DEBUG water filled back up turned New Water pump OFF");
                float empty = _atoAwcUtil->recordRun("awc_fill",false);
              }else{
                //Serial.println("AWC water is filled back up");
                digitalWrite(18, 0);//turn off fill pump
                //Serial.println("Pump 2 new-water pump is stopped");
                float empty = _atoAwcUtil->recordRun("awc_fill",false);
              }
            emptied = false;
            filled = false;
            //resetAwcTimer = true;
            executingAwc = false;
            //awcCount++;
            //Serial.println("Awc Count is "+awcCount);
            sentAwcHttp = false;
            wswAwc1 = false;
            wswAwc2 = false;
            wswAwc3 = false;
            wswAwc4 = false;
          }
        }

  }
/*    if(waterEmptied == false && waterFilled == true){
      Serial.println("Executing AWC");
      //_atoAwcUtil->webSerialWrite("EAWC_4_1");
      sentAwcHttp = true;
        if(_debug){
          _testharness->digWrite(14,1);
          Serial.println("DEBUG sump pump running");
        }else{
          digitalWrite(14, 1);  //turn sump pump on
        }
        Serial.println("Pump 3 Sump pump is running");
      if(_debug){
        pinZero = _testharness->digRead(0);
      }else{
        pinZero = digitalRead(0);
      }
      if(pinZero == 1){//low sensor in the water
        Serial.println("Sensor 2 low level sensor is out of water");
        //waterChange = true;
        if(_debug){
          _testharness->digWrite(14,0);
          Serial.println("DEBUG sump pump off water empty");
        }else{
          Serial.println("AWC water is emptied");
          digitalWrite(14, 0);  //turn sump pump off
          Serial.println("Pump 3 Sump pump is stopped");
        }
        if(!wswAwc2){
          wswAwc2 = true;
          //_atoAwcUtil->webSerialWrite("EAWC_4_2");
        }
      }else{
        if(!wswAwc3){
          wswAwc3 = true;
         // _atoAwcUtil->webSerialWrite("EAWC_4_3");
        }
       waterEmptied = true;
       Serial.println("Marked water as emptied");
        waterFilled = false;
      }//emptied water
    }///////////////// EMPTY WATER DONE /////////////////////
    if(waterEmptied == true && waterFilled == false){
      if(_debug){
        pinTwo = _testharness->digRead(2);
      }else{
        pinTwo = digitalRead(2);
      }
      if(pinTwo == 1){//normal sensor out of water
        if(_debug){
          _testharness->digWrite(27,1);
          Serial.println("DEBUG turned on New Water pump");
        }else{
          Serial.println("Sensor 1 normal-level is out of water");
          digitalWrite(27,1);  // new water pump on
          Serial.println("Pump 2 new-water pump is running");
        }
        float empty = _atoAwcUtil->recordRun("awc_fill",true);
        if(!wswAwc4){
          wswAwc4 = true;
          //_atoAwcUtil->webSerialWrite("EAWC_4_4");
        }
      }else{
       if(_debug){
          _testharness->digWrite(27,0);
          Serial.println("DEBUG water filled back up turned New Water pump OFF");
        }else{
          Serial.println("AWC water is filled back up");
          digitalWrite(27, 0);//turn off fill pump
          Serial.println("Pump 2 new-water pump is stopped");
        }
        float full = _atoAwcUtil->recordRun("awc_fill",false);
        if(!wswAwc5){
          wswAwc5 = true;
           //_atoAwcUtil->webSerialWrite("EAWC_4_5");
        }
        waterFilled = true;
        waterEmptied = false;
        waterChange = false; 
        executingAwc = false;
        sentAwcHttp = false;
        wswAwc2 = false;
        wswAwc3 = false;
        wswAwc4 = false;
        wswAwc5 = false;
       Serial.println("Marked waterFilled T, waterChange F, and executingAwc F");
      }
    } 
  } */
}//end of water change and water emptied true

/////////////////////////////////////////////////////////////////////////////////////
//                        end of doWaterChange
/////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////
//
/////////////////////////////////////////////
float Awc::calcPadded(float percent, unsigned long value){
  ;
}
    
bool Awc::getWaterChange(){
  return waterChange;
}
void Awc::setWaterChange(bool val){
  waterChange = val;
}
bool Awc::getExecuteAwc(){
  return executeAwc;
}
void Awc::setExecuteAwc(bool val){
  executeAwc = val;
}
bool Awc::getExecutedAwc(){
  return executedAwc;
}
void Awc::setExecutedAwc(bool val){
  executedAwc = val;
}
bool Awc::getExecutingAwc(){
  return executingAwc;
}
void Awc::setExecutingAwc(bool val){
  executingAwc = val;
}
bool Awc::getSentAwcHttp(){
  return sentAwcHttp;
}
void Awc::setSentAwcHttp(bool val){
  sentAwcHttp = val;
}

bool Awc::getWaterEmptied(){
  return waterEmptied;
}
void Awc::setWaterEmptied(bool val){
  waterEmptied = val;
}
