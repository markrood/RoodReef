//TODO change writeArray() to write to SD card and not spiffs
#include "graphs.h"


Graphs::Graphs(AtoAwcUtil *atoAwcUtil)
{
  _atoAwcUtil = atoAwcUtil;
  Serial.begin(115200);
}


//////////////////////////////////////////////////////////////
//                                                          //
//   writeArray                                          //
//                                                          //
//    input:  string type (ato or awc), int year,           //
//            String month, int day float value             //
//                                                          //
//    output: string                                        //
//                                                          //
//    description: this builds our array for the chart      //
//                write this ONCE a day                     //
//   format: {"month":10,"year":2020,"days":[19.0,0.0,2.0,14.0,14.0,
//            15.0,13.0,18.0,8.0,10.0,0.0,11.0,11.0,8.0,14.0,1.0,11.0,
//            10.0,17.0,14.0,5.0,18.0,16.0,14.0,11.0,11.0,18.0,6.0,
//            11.0,12.0,11.00]}
//
//   result:  write a file to spiffs /october     //
//                                                          //
//////////////////////////////////////////////////////////////
bool Graphs::writeArray(String type,int thisYear,String thisMonth, int thisDay,float value){
    //String strMonth = String(thisMonth);
    
    StaticJsonDocument<1024> docOut;
    String arr = "";
    String theEnd = "]}";
        String retMonth = _atoAwcUtil->changeMonthStrToNum(thisMonth);
    String json1Ato = "{\"month\":"+retMonth+",\"year\":"+thisYear+",\"days\":[";

    //String json1Ato = "{\"month\":\""+thisMonth+"\",\"year\":2020,\"days\":[";
    String filename = "";
    String atoFile ="";
    String awcFile = "";
    String blueFile = "";
    String greenFile = "";
    String yellowFile = "";
    String purpleFile = "";
    String curFile = "";
    const char *newArrComplete;
  if(type.equals("ato")){
    filename = "/"+thisMonth+"atoArr.txt";
    atoFile = _atoAwcUtil->readFile(SPIFFS, filename.c_str());
    curFile = atoFile;
  }else if (type == "awc"){
    filename = "/"+thisMonth+"awcArr.txt";
    awcFile = _atoAwcUtil->readFile(SPIFFS, filename.c_str());
    curFile =awcFile;
  }else if (type == "blue"){
    filename = "/"+thisMonth+"blueArr.txt";
    blueFile = _atoAwcUtil->readFile(SPIFFS, filename.c_str());
    curFile =blueFile;    
  }else if (type == "green"){
    filename = "/"+thisMonth+"greenArr.txt";
    greenFile = _atoAwcUtil->readFile(SPIFFS, filename.c_str());
    curFile =greenFile;    
  }else if (type == "yellow"){
    filename = "/"+thisMonth+"yellowArr.txt";
    yellowFile = _atoAwcUtil->readFile(SPIFFS, filename.c_str());
    curFile =yellowFile;    
  }else if (type == "purple"){
    filename = "/"+thisMonth+"purpleArr.txt";
    purpleFile = _atoAwcUtil->readFile(SPIFFS, filename.c_str());
    curFile = purpleFile;    
  }
  if(thisMonth == "january"||thisMonth == "march"||
      thisMonth == "may"||thisMonth == "july"||
      thisMonth == "august"||thisMonth == "october"||
      thisMonth == "december"){
    if(curFile == ""){
    
     //Serial.println("no file written yet");
      //bool currDay = false;
      for(int i=0;i<31;i++){
        if( i == thisDay-1 && i != 0){//TODO add the -1 to all below
          //append thisDay
          arr = arr+","+value;
        }else if(i == thisDay-1 && i == 0){//TODO add the -1 to all below
          arr = arr+value;
        }else if(i == 0){
          arr = arr+"0.0";
        }else{
          arr = arr+",0.0";
        }
      }
    
      }else{
         auto error = deserializeJson(docOut, curFile);
          if (error) {
                Serial.print(F("deserializeJson() failed with code "));
                //WebSerial.print("deserializeJson() faled with code ");
                Serial.println(error.c_str());
                //WebSerial.println(error.c_str());
                //return false;
            }
            //put back together
              for(int i=0;i<31;i++){
                if( i == thisDay && i != 0){
                  //append thisDay
                  arr = arr+","+value;
                }else if(i == thisDay && i == 0){
                  arr = arr+value;
                }else if(i < thisDay && i == 0){
                  float dayx = docOut["days"][i];
                  arr = arr+String(dayx,1);
                }else if(i< thisDay && i != 0){
                  float dayx = docOut["days"][i];
                  arr = arr+","+String(dayx,1);
                }else if(i > thisDay && i == 0){
                  float dayx = docOut["days"][i];
                  arr = arr+String(dayx,1);
                }else if (i > thisDay && i != 0){
                   float dayx = docOut["days"][i];
                    arr = arr+","+String(dayx,1);
                }
              }
      }
      }
      
  if(thisMonth == "april"||thisMonth == "june"||
      thisMonth == "september"||thisMonth == "november"){
    //bool currDay = false;
        if(curFile == ""){
      //file not written yet
        //Serial.println("no file written yet");
          for(int i=0;i<29;i++){
            if( i == thisDay && i != 0){
              //append thisDay
              arr = arr+","+value;
            }else if(i == thisDay && i == 0){
              arr = arr+value;
            }else if(i == 0){
              arr = arr+"0.0";
            }else{
              arr = arr+",0.0";
            }
          }
         }else{
         auto error = deserializeJson(docOut, curFile);
          if (error) {
                Serial.print(F("deserializeJson() failed with code "));
                ////WebSerial.print("deserializeJson() faled with code ");
                Serial.println(error.c_str());
                //WebSerial.println(error.c_str());
                //return false;
            }
            //put back together
              for(int i=0;i<30;i++){
                if( i == thisDay && i != 0){
                  //append thisDay
                  arr = arr+","+value;
                }else if(i == thisDay && i == 0){
                  arr = arr+value;
                }else if(i < thisDay && i == 0){
                  float dayx = docOut["days"][i];
                  arr = arr+String(dayx,1);
                }else if(i< thisDay && i != 0){
                  float dayx = docOut["days"][i];
                  arr = arr+","+String(dayx,1);
                }else if(i > thisDay && i == 0){
                  float dayx = docOut["days"][i];
                  arr = arr+String(dayx,1);
                }else if (i > thisDay && i != 0){
                   float dayx = docOut["days"][i];
                    arr = arr+","+String(dayx,1);
                }
              }
      }
      }
  if(thisMonth == "february"){
    //bool currDay = false;    
    if(curFile == ""){
      //file not written yet
   Serial.println("no file written yet");   
    for(int i=0;i<28;i++){
      if( i == thisDay && i != 0){
        //append thisDay
        arr = arr+","+value;
      }else if(i == thisDay && i == 0){
        arr = arr+value;
      }else if(i == 0){
        arr = arr+"0.0";
      }else{
        arr = arr+",0.0";
      }
    }
    }else{
         auto error = deserializeJson(docOut, curFile);
          if (error) {
                Serial.print(F("deserializeJson() failed with code "));
                //WebSerial.print("deserializeJson() faled with code ");
                Serial.println(error.c_str());
                //WebSerial.println(error.c_str());
                //return false;
            }
            //put back together
              for(int i=0;i<28;i++){
                if( i == thisDay && i != 0){
                  //append thisDay
                  arr = arr+","+value;
                }else if(i == thisDay && i == 0){
                  arr = arr+value;
                }else if(i < thisDay && i == 0){
                  float dayx = docOut["days"][i];
                  arr = arr+String(dayx,1);
                }else if(i< thisDay && i != 0){
                  float dayx = docOut["days"][i];
                  arr = arr+","+String(dayx,1);
                }else if( i > thisDay && i == 0){
                  float dayx = docOut["days"][i];
                  arr = arr+String(dayx,1);
                }else if (i > thisDay && i != 0){
                   float dayx = docOut["days"][i];
                    arr = arr+","+String(dayx,1);
                }
              }
      }
  }
      String newArr = json1Ato+arr+theEnd;
    newArrComplete = newArr.c_str();
    //Serial.println("Thiiiiiiiiiiiiis is "+newArr);

  ////////write the file now ////////////////
  if(type == "ato"){
    //write ato
    file_ato = filename.c_str();
    Serial.println("in writeArray and filename is:"+filename+"and value is:"+newArrComplete);
    _atoAwcUtil->writeFile(SPIFFS, file_ato, newArrComplete);
    //Serial.println("write!!!!!!!!!!!!");
    //Serial.println(newArrComplete);
    //Serial.println("USING");
    //Serial.println(file_ato);
  }else if(type == "awc"){
    //write awc
    file_awc = filename.c_str();
    _atoAwcUtil->writeFile(SPIFFS, file_awc, newArrComplete);
  }else if(type == "blue"){
    //write blue
    file_blue = filename.c_str();
    _atoAwcUtil->writeFile(SPIFFS, file_blue, newArrComplete);
  }else if(type == "green"){
    //write green
    file_green = filename.c_str();
    _atoAwcUtil->writeFile(SPIFFS, file_green, newArrComplete);
  }else if(type == "yellow"){
    //write yellow
    file_yellow = filename.c_str();
    //Serial.println("just wrote yellow file "+filename);
    _atoAwcUtil->writeFile(SPIFFS, file_yellow, newArrComplete);
  }
  return true;
}

////////////////////////////////////////////////////////////////////
//
//  Fuction: readFilecloseDayPutInArr
//
//  Input:  String type ("ato","blue, etc.), 
//          float sayAccumulation
//          int testYear
//          int testMon
//          int testDay
//
//  Output:  none
//
//  Discription:  Simply call writeArray with appropiate type
//
/////////////////////////////////////////////////////////////////////
void Graphs::closeDayPutInArr(String type, float dayAccumulation,int testYear,int testMonth, int testDay){
  //yrMoDay is set at begining then is updated on every day change
  //we are using yrMoDay from yesterday before we change it to today in the new day event
  currentTimeArr = _atoAwcUtil->setCurrentTime();
  String mo = _atoAwcUtil->changeMonthNubToStr(currentTimeArr[1]);
  Serial.println("year is:"+String(currentTimeArr[0])+", mo is:"+mo+" and Day is:"+String(currentTimeArr[2]));
  if(type == "ato"){
    Serial.println("writeArray with "+String(dayAccumulation));
    if(testDay != 0){
      //used just for debug to increment day
      writeArray("ato",currentTimeArr[0],mo, currentTimeArr[2]+testDay,dayAccumulation);
    }else{
      writeArray("ato",currentTimeArr[0],mo, currentTimeArr[2],dayAccumulation);
    }
  }else if (type == "awc"){
    writeArray("awc",currentTimeArr[0],mo, currentTimeArr[2],dayAccumulation);
  }else if (type == "yellow"){
    bool retVal = writeArray("yellow",currentTimeArr[0],mo, currentTimeArr[2],dayAccumulation);
    Serial.print("return from Graph::writeArray is ");
    Serial.println(retVal);
  }else if (type == "purple"){
    bool retVal = writeArray("purple",currentTimeArr[0],mo, currentTimeArr[2],dayAccumulation);
    Serial.print("return from Graph::writeArray is ");
    Serial.println(retVal);
  }else if (type == "green"){
    writeArray("green",currentTimeArr[0],mo, currentTimeArr[2],dayAccumulation);
  }else if (type == "blue"){
    writeArray("blue",currentTimeArr[0],mo, currentTimeArr[2],dayAccumulation);
  }
}

////////////////////////////////////////////////////////////////////
//
//  Fuction: buildYearJson
//
//  Input:  String  Type ("ato", "awc", "blue", "green", "yellow"
//
//  Output:  String the complet year by month json for a certain type
//          {months:[
//            {"month":1,"year":2021,"days":[1263.0,1363.0,1401.0,1384.0,
//              1159.0,1370.0,1315.0,1390.0,1274.0,1305.0,1132.0,1178.0,
//              1286.0,1472.0,1250.0,951.0,1199.0,1336.0,1214.0,1220.0,1300.0,
//              1231.0,1393.0,1112.0,1355.0,1142.0,1255.0,1127.0,1363.0,1199.0,1120.00]},
//              {"month":2, y....."days":[....]}
//            ]}
//
//  Discription:  This function build the complete year json for each
//                type i.e. ato,awc,blue,green, yellow. After 12 months
//                it deletes this string and starts over for new year.
//
//  Calls:  writeArray()
//
//  SPIFFS:  Doesn't write anything.
//            "/januaryatoArr.txt"... reads these files to build year json
//
//  TEST_MODE:   yes
//
/////////////////////////////////////////////////////////////////////

String Graphs::buildYearJson(String type,int testMonth){
  String prefix = "{\"months\": [";
  String endStr = "]}";
  String finalStr = "";
  String jsonYearato = "";
  String jsonYearawc = "";
  String jsonYearblue = "";
  String jsonYeargreen = "";
  String jsonYearyellow = "";
  String jsonYearpurple = "";
  String str = "";
  String strPlusOne = "";
  String temp = "";
  String tempPlusOne = "";
 setCurrentTime();

  if(type == "ato"){
    for(int i=1;i<13;i++){

      str = "/"+_atoAwcUtil->changeMonthNubToStr(i)+"atoArr.txt";
      temp = _atoAwcUtil->readFile(SPIFFS,  str.c_str());
      Serial.println("in build year and month is:"+str+" and file read is:"+temp);
      //Serial.println("*************************** "+tempPlusOne);
      if(temp == "" && i < (currentMonth)){
        String mo = _atoAwcUtil->changeMonthNubToStr(i);
        writeArray("ato",currentYear,mo, 1,0);//just putting zero in previous months
        Serial.println("Put in zeros for month "+str);
       temp = _atoAwcUtil->readFile(SPIFFS,  str.c_str());       
      }else if(temp == "" && i >(currentMonth)){
        //Serial.println("broke out of month loop in build year");
        break;
      }
      if(i == 1){
        jsonYearato = jsonYearato+temp;
      }else{
        //Serial.println("json before plus is "+jsonYearato);
        jsonYearato = jsonYearato+","+temp;
        //Serial.println("json after plus is "+jsonYearato);
      }
    }
    finalStr = prefix+jsonYearato+endStr;
    //jsonYearAto = finalStr;
    return finalStr;
    
   // Serial.println("final string "+finalStr);
  }else if(type == "awc"){
    for(int i=1;i<13;i++){
      str = "/"+_atoAwcUtil->changeMonthNubToStr(i)+"awcArr.txt";
      temp = _atoAwcUtil->readFile(SPIFFS,  str.c_str());
      if(temp == "" && i < currentMonth){
        String mo = _atoAwcUtil->changeMonthNubToStr(i);
        writeArray("awc",currentYear,mo, 1,0);//just putting zero in previous months
        Serial.println("Put in zeros for month "+str);
        temp = _atoAwcUtil->readFile(SPIFFS,  str.c_str());
      }else if(temp == "" && i >currentMonth){
        //Serial.println("broke out of month loop in build year");
        break;
      }
      if(i == 1){
        jsonYearawc = jsonYearawc+temp;
      }else{
        jsonYearawc = jsonYearawc+","+temp;
      }
    }
    finalStr = prefix+jsonYearawc+endStr;
    return finalStr;
   // Serial.println("final string "+finalStr);
  }else if(type == "blue"){  
    for(int i=1;i<13;i++){
      str = "/"+_atoAwcUtil->changeMonthNubToStr(i)+"blueArr.txt";
      temp = _atoAwcUtil->readFile(SPIFFS,  str.c_str());
      if(temp == "" && i < currentMonth){
        String mo = _atoAwcUtil->changeMonthNubToStr(i);
        writeArray("blue",currentYear,mo, 1,0);//just putting zero in previous months
        Serial.println("Put in zeros for month "+str);
        temp = _atoAwcUtil->readFile(SPIFFS,  str.c_str());
      }else if(temp == "" && i >currentMonth){
        //Serial.println("broke out of month loop in build year");
        break;
      }
      if(i == 1){
        jsonYearblue = jsonYearblue+temp;
      }else{
        jsonYearblue = jsonYearblue+","+temp;
      }
    }
    finalStr = prefix+jsonYearblue+endStr;
    return finalStr;
   // Serial.println("final string "+finalStr);
  }else if(type == "green"){
    for(int i=1;i<13;i++){
      str = "/"+_atoAwcUtil->changeMonthNubToStr(i)+"greenArr.txt";
      temp = _atoAwcUtil->readFile(SPIFFS,  str.c_str());
      if(temp == "" && i < currentMonth){
        String mo = _atoAwcUtil->changeMonthNubToStr(i);
        writeArray("green",currentYear,mo, 1,0);//just putting zero in previous months
        Serial.println("Put in zeros for month "+str);
        temp = _atoAwcUtil->readFile(SPIFFS,  str.c_str());
      }else if(temp == "" && i >currentMonth){
        //Serial.println("broke out of month loop in build year");
        break;
      }
      if(i == 1){
        jsonYeargreen = jsonYeargreen+temp;
      }else{
        jsonYeargreen = jsonYeargreen+","+temp;
      }
    }
    finalStr = prefix+jsonYeargreen+endStr;
    return finalStr;
   // Serial.println("final string "+finalStr);
  }else if(type == "yellow"){
    for(int i=1;i<13;i++){
      str = "/"+_atoAwcUtil->changeMonthNubToStr(i)+"yellowArr.txt";
      temp = _atoAwcUtil->readFile(SPIFFS,  str.c_str());
      if(temp == "" && i < currentMonth){
        String mo = _atoAwcUtil->changeMonthNubToStr(i);
        writeArray("yellow",currentYear,mo, 1,0);//just putting zero in previous months
        Serial.println("Put in zeros for month "+str);
        temp = _atoAwcUtil->readFile(SPIFFS,  str.c_str());
      }else if(temp == "" && i >currentMonth){
        //Serial.println("broke out of month loop in build year");
        break;
      }
      if(i == 1){
        jsonYearyellow = jsonYearyellow+temp;
      }else{
        jsonYearyellow = jsonYearyellow+","+temp;
      }
    }
    finalStr = prefix+jsonYearyellow+endStr;
    return finalStr;
   // Serial.println("final string "+finalStr);
  }else if(type == "purple"){
    for(int i=1;i<13;i++){
      str = "/"+_atoAwcUtil->changeMonthNubToStr(i)+"purpleArr.txt";
      temp = _atoAwcUtil->readFile(SPIFFS,  str.c_str());
      if(temp == "" && i < currentMonth){
        String mo = _atoAwcUtil->changeMonthNubToStr(i);
        writeArray("purple",currentYear,mo, 1,0);//just putting zero in previous months
        Serial.println("Put in zeros for month "+str);
        temp = _atoAwcUtil->readFile(SPIFFS,  str.c_str());
      }else if(temp == "" && i >currentMonth){
        //Serial.println("broke out of month loop in build year");
        break;
      }
      if(i == 1){
        jsonYearpurple = jsonYearpurple+temp;
      }else{
        jsonYearpurple = jsonYearpurple+","+temp;
      }
    }
    finalStr = prefix+jsonYearpurple+endStr;
    return finalStr;
   // Serial.println("final string "+finalStr);
  }
  //Serial.println("finalStr in build year is !!!! "+finalStr);
  //return finalStr;
}

/////////////////////////////////////////////
void Graphs::setCurrentTime(){
  int i = 0;
  currentTimeArr = _atoAwcUtil->setCurrentTime();
    while(currentTimeArr[0] == 1970){
        currentTimeArr = _atoAwcUtil->setCurrentTime(); 
        i++;  
    }
    currentYear = currentTimeArr[0];
    currentMonth = currentTimeArr[1];
    currentDay = currentTimeArr[2];
    currentHour = currentTimeArr[3];
    currentMinute = currentTimeArr[4];

 /*   Serial.print("current year in graph is :");
    Serial.println(currentTimeArr[0]);
    Serial.print(i);*/
}

void Graphs::setUtcOffsetInSeconds(int offset){
  utcOffsetInSeconds = offset;
}
