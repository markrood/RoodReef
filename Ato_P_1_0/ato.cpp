#include "ato.h"


Ato::Ato(AtoAwcUtil *atoAwcUtil, Testharness *testharness,bool debug)
{
  _atoAwcUtil = atoAwcUtil;
  _testharness = testharness;
  _debug = debug;
  

}

//////////////////////////////////////////////////////////////////
//
//  beginAto()
//
//  sets up pins
//
/////////////////////////////////////////////////////////////////
void Ato::beginAto(){
  pinMode(2, INPUT);  //normal sensor  (YELLOW 02)
  //digitalWrite(2,LOW);     //want normal to be in water by default (0 = in water)
  pinMode(0, INPUT);  //low sensor     (PURPLE 0)
  //digitalWrite(0,LOW);     //want low water to be in water by default (0 = in water)
  pinMode(13, INPUT);  //high sensor     (REDn13pz)
  //digitalWrite(13,HIGH);     //want hight water to be OUT of water by default (1 = out of water)
  pinMode(12, OUTPUT); //RO pump         (BLUE 12)  M1
  digitalWrite(12,0);
  pinMode(18, OUTPUT); //New pump        (WHITE 27) M3
  digitalWrite(18,0);
  pinMode(14, OUTPUT); //Sump pump       (GREEN 14) M2
  digitalWrite(14,0);
  pinMode(5, OUTPUT); //norm light     (YELLOW)  LIGHT1
  digitalWrite(5,0);
  pinMode(32, OUTPUT);  //low light     (PURPLE)  LIGHT2
  digitalWrite(32,0);
  pinMode(25, OUTPUT);  //high light     (RED)     LIGHT3
  digitalWrite(25,0);
  //pinMode(27,INPUT);  //leak detect     (BIG BLUE)  LEAK A0
  pinMode(15, OUTPUT);  //tank pump relay (BROWN/GREY)  SILVER
  digitalWrite(15,0);
  
  pinMode(39,INPUT);  //Battery voltage Analog input
  pinMode(36,INPUT);  //USB voltage  Analog input
}


//////////////////////////////////////////////////////////////
//                                                           //
//   Function: doAto()                                      //
//                                                          //
//   input: none                                            //
//                                                          //
//   output:  bool                                          //
//                                                          //
//   description:  runs auto top off (ato)                  //
//                                                          //
//////////////////////////////////////////////////////////////
void Ato::doAto(){
  //didAto = false;
  if(waterChange == false){
    sentAtoHttp = true;
    executeAto = true;
    
    Serial.println("Executing ATO");
    Serial.print("Sensor is low in water, high out of water :");
    Serial.println(digitalRead(0));
    if(_debug){
      Serial.println("_debug in ato is true");
      pinTwo = _testharness->digRead(2);
      Serial.print("pinTwo in ato is ");
      Serial.println(pinTwo);
    }else{
      pinTwo = digitalRead(2);
    }
      if( pinTwo == 1){  // normal level sensor out of water
        if(executeAto){
          itRan = true;
          executingAto = true;
          startFill = millis();
          executeAto = false;
          
        }
        Serial.println("Sensor1 normal level is out of water");
        if(_debug){
          _testharness->digWrite(12,1);
        }else{
          digitalWrite(12, 1);  //run RO pump
        }     
        Serial.println("Pump 1 RO water pump is running");
        float empty = _atoAwcUtil->recordRun("ato",true);
        if(!sentWebSerialWrite){
          if(!wswAto1){
            wswAto1 = true;
            _atoAwcUtil->webSerialWrite("EATO_5_1");
          }
          sentWebSerialWrite = true;
          //didAto = true;
        }
    }else{
      Serial.println("Ato water is full again!");
      if(itRan){
        if(_debug){
          _testharness->digWrite(12,0);
        }else{
          digitalWrite(12, 0);//turn off RO pump
        }     
        
        float full = _atoAwcUtil->recordRun("ato",false);
        runningFill = millis();
          if(!wswAto2){
            wswAto2 = true;
            _atoAwcUtil->webSerialWrite("EATO_5_2");
          }
        Serial.println("Pump 1 Ro pump is off");
        executingAto = false;
        sentAtoHttp = false;
        executeAto = true;
        float elapse = runningFill-startFill;
        Serial.print("Ato ran this long : ");
        Serial.println(elapse/1000);
        startFill = 0;
        runningFill = 0;
        executingAto = _atoAwcUtil->checkStoreRun("ato_fill", elapse);
      }
        executingAto = false;
        sentAtoHttp = false;
        executeAto = true;
        startFill = 0;
        runningFill = 0;
        itRan = false;
        sentWebSerialWrite = false;
        wswAto1 = false;
        wswAto2 = false;
    }

    
  }
    
} //auto top off

//////////////////////////////////////////////////////////////
//                                                          //
//   checkLeaks                                             //
//                                                          //
//   input: none                                            //
//                                                          //
//   output:  bool                                          //
//                                                          //
//   description:  checks for leaks                         //
//                                                          //
//////////////////////////////////////////////////////////////
bool Ato::checkLeaks(){
  Serial.println("in checkLeaks");
  
  llevel = _atoAwcUtil->readFile(SPIFFS, "/leaklevel.txt");
  Serial.print("Leak voltage is : ");
  Serial.println(llevel);
  if(llevel == "moister"){
    lklevel = 500;
  }else if(llevel == "damp"){
    lklevel = 700;
  }else if(llevel == "wet"){
    lklevel = 900;
  }
  //int leaklevel = llevel.toInt();
  //resVal = analogRead(resPin); //pin 34 Leak
//CUT
Serial.print("analog value for leak is ");
Serial.println(resVal);
Serial.print("level for wetness is ");
Serial.println(lklevel);
  if(resVal >=lklevel){
     digitalWrite(15, 1);//turn off pump Pin 15
     //int status = sendHttp("Leak"); 
     return true;
  }else{
    digitalWrite(15, 0);
    return false;
  }
}
//////////////////////////////////////////////////////////////// 

//////////////////////////////////////////////////////////////
//                                                          //
//   checkHighwater                                             //
//                                                          //
//   input: none                                            //
//                                                          //
//   output:  bool                                          //
//                                                          //
//   description:  checks for high water                        //
//                                                          //
//////////////////////////////////////////////////////////////
bool Ato::checkHighWater(){
  //if pin x is high

  int  val = digitalRead(13);//TODO if high water pin is low
  if(val){
     //int status = sendHttp("HighWater"); 
     return true;
  }else{
    return false;
  }
}
//////////////////////////////////////////////////////////////// 

//////////////////////////////////////////////////////////////
//                                                          //
//   checkLowwater                                             //
//                                                          //
//   input: none                                            //
//                                                          //
//   output:  bool                                          //
//                                                          //
//   description:  checks for high water                        //
//                                                          //
//////////////////////////////////////////////////////////////
bool Ato::checkLowWater(){
  //if pin x is high

  int  val = digitalRead(0);//TODO if low water pin is high
  if(val){
     //int status = sendHttp("LowWater"); 
     return true;
  }else{
    return false;
  }
}

void Ato::doWaterLights(){

  if(displaySensorFirst){
    dsTime = millis();
    displaySensorFirst = false;
  }else{
    dsTimeNow = millis();
    dsTimeElapse = dsTimeNow - dsTime;
  }
  if(dsTimeElapse >= 60000){
    displaySensorFirst = true;
    dsTime = 0;
    dsTimeElapse = 0;
    displaySensors = true;
  }
  
  if(displaySensors){
    int nw = 0;
    if(_debug){
      nw = _testharness->digRead(2);
    }else{
      nw = digitalRead(2);  //yellow mcp.out8 normal!!!
    } 
    Serial.print("Water Normal sensor is ");
    Serial.println(nw);
    if(nw == 0){
      digitalWrite(5,LOW);
    }else{
      digitalWrite(5,HIGH);
    }
    
    int lw = 0;
    if(_debug){
      lw = _testharness->digRead(0);
    }else{
      lw = digitalRead(0);  //yellow mcp.out8 normal!!!
    } 
    Serial.print("Water Low sensor is ");
    Serial.println(lw);
     if(lw == 0){
      digitalWrite(32,LOW);
    }else{
      digitalWrite(32,HIGH);
    }
   
    int ehw = 1;
    if(_debug){
      ehw = _testharness->digRead(13);
    }else{
      ehw = digitalRead(13);  //yellow mcp.out8 normal!!!
    } 
    Serial.print("Extreme Water High sensor is ");
    String hws = String(ehw);
    Serial.println(hws);
   if(ehw == 0){
      digitalWrite(25,LOW);
    }else{
      digitalWrite(25,HIGH);
    }
/*    Serial.print("analog value for leak is ");
    Serial.println(resVal);
    Serial.print("level for wetness is ");
    Serial.println(lklevel);*/

    int battVolt = analogRead(36);
    Serial.print("Battery volt value is: ");
    Serial.println(battVolt);

    int usbVolt = analogRead(39);
    Serial.print("USB volt value is: ");
    Serial.println(usbVolt);

    displaySensors = false;
  }
}
bool Ato::getWaterChange(){
  return waterChange;
}
void Ato::setWaterChange(bool val){
  waterChange = val;
}
bool Ato::getExecuteAto(){
  return executeAto;
}
void Ato::setExecuteAto(bool val){
  executeAto = val;
}
bool Ato::getExecutedAto(){
  return executedAto;
}
void Ato::setExecutedAto(bool val){
  executedAto = val;
}
bool Ato::getExecutingAto(){
  return executingAto;
}
void Ato::setExecutingAto(bool val){
  executingAto = val;
}
bool Ato::getSentAtoHttp(){
  return sentAtoHttp;
}
void Ato::setSentAtoHttp(bool val){
  sentAtoHttp = val;
}
bool Ato::getItRan(){
  return itRan;
}
void Ato::setItRan(bool val){
  itRan = val;
}
