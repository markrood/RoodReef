#include "timer.h"

Timer::Timer(AtoAwcUtil *atoAwcUtil)
{
  _atoAwcUtil = atoAwcUtil;
    spiffsLReset = 1;
    spiffsHWReset = 1;
    spiffsLWReset = 1;
    spiffsAtoReset = 1;
    spiffsAwcReset = 1;
    spiffsHbReset = 1;
    spiffsBdReset = 1;
    spiffsGdReset = 1;
    spiffsYdReset = 1;
    spiffsPdReset = 1;
  Serial.begin(115200);
}

  /////////////////////main timer functions ///////////////////////////////////
  //////////////////////////////////////////////////////////////
//                                                            //
  //   execute                                                //
//                                                            //
  //   input: String type notification or next (ato or awc)   //
//                                                            //
  //   output: bool  true or false                            //
//                                                            //
  //   description:  returns true if it is time to run        //
//                   leak,high water, low water, ato, awc     //
  //////////////////////////////////////////////////////////////
  bool Timer::execute(String nextType){
    if(nextType == "atoNot"){
      //Serial.println("IN ATONOT");
    }
    //Serial.print("timer.executing :");
    //Serial.println(nextType);
/// boolean execute(String nextType){
    boolean retVal = false;
    //setUtcOffsetInSeconds(0);
    String strTime = "";
    String strArr;
    bool sentPrintOnce = false;
///   boolean sentPrintOnce = false;
    setCurrentTime();
    setYrMoDay();
    if(currentTimeArr[1] == 2 && currentTimeArr[3] == 28){
      leapYear = true;
//      WebSerial////System.out.print("LY,");
    }
    //leakNextArr,spiffsLReset,"leakNot","/leakNot.txt"
    int spiffsXReset = 0;
    String notification = "";
    String webSerialCode;
    const char *filename = "";;//const char *file_ato = "";
///   String filename = "";
    String curTime = arrToString(currentTimeArr);
    //Serial.print("----------------------------------------------- current time is :");
    //Serial.println(curTime);
    // ////System.out.println("current time is "+curTime);
    //Web////System.out.print("CT "+curTime+",");
    //NEEDED TO put a one min buffer either way in
    String curTimeMinusOne = arrToString(currentTimeArrMinusOneMin);
    //////System.out.println("current minus one time is "+curTimeMinusOne);
    String curTimePlusOne = arrToString(currentTimeArrPlusOneMin);
    //////System.out.println("current plus one time is "+curTimePlusOne);
    //////System.out.println("current time is: "+curTime);
    if(nextType == "dayChange"){   
      if(changeOfDay()){
        return true;
//        Web////System.out.print("ND,");
      }else{
        return false;
      }
    }
           //leakNextArr,spiffsLReset,"leakNot","/leakNot.txt",
    if(nextType == "leakNot"){
      spiffsXReset = spiffsLReset;
      notification = "leakNot"; 
      webSerialCode = "LN";
      filename = "/leakNot.txt";
      strArr = _atoAwcUtil->readFile(SPIFFS, filename);
      ///strArr = _atoAwcUtil.readFile("Spiffs", filename);
      //////System.out.println("strArr in leak form spiffs is : "+strArr);
      if(strArr != ""){
        tokenizeNextTime(strTime, leakNextArr); //put strArr in leakNextArr
     /// leakNextArr = tokenizeNextTime(strArr, leakNextArr); //put strArr in leakNextArr
        for(int i = 0;i<5;i++){
          nextArr[i] = leakNextArr[i];
          //////System.out.println("nextArr in loop for leak is "+nextArr[i]);
        }
      }    
    }else if(nextType == "highWaterNot"){
      spiffsXReset = spiffsHWReset;
      notification = "highWaterNot";
      webSerialCode = "HWN";
      filename = "/hwNot.txt";
      strArr = _atoAwcUtil->readFile(SPIFFS, filename);//read leakNext time from memory
      ///strArr = _atoAwcUtil.readFile("SPIFFS", filename);//read leakNext time from memory
      //////System.out.println("strArr in hw form spiffs is : "+strArr);
      if(strArr != ""){
 /*///       hwNextArr = */tokenizeNextTime(strArr, hwNextArr); //put strArr in hwNextArr
        for(int i = 0;i<5;i++){
          nextArr[i] = hwNextArr[i];
          //////System.out.println("nextArr in loop for hw is "+nextArr[i]);
        }
      }
    }else if(nextType == "lowWaterNot"){
      spiffsXReset = spiffsLWReset;
      notification = "lowWaterNot";
      webSerialCode = "LWN";
      filename = "/lwNot.txt";
      strArr = _atoAwcUtil->readFile(SPIFFS, filename);//read leakNext time from memory
///     strArr = _atoAwcUtil.readFile("SPIFFS", filename);//read leakNext time from memory
      //////System.out.println("strArr in lw form spiffs is : "+strArr);
      if(strArr != ""){
        tokenizeNextTime(strArr, lwNextArr); //put strArr in lwNextArr
        for(int i = 0;i<5;i++){
          nextArr[i] = lwNextArr[i];
          //////System.out.println("nextArr in loop for lw is "+nextArr[i]);
        }
      }
    }else if(nextType == "atoNot"){
      //Serial.println("IN ATONOT 2");
      spiffsXReset = spiffsAtoReset;
      notification = "atoNot";
      webSerialCode = "ATON";
      filename = "/atoNot.txt";
      strArr = _atoAwcUtil->readFile(SPIFFS, filename);//read leakNext time from memory
///     strArr = _atoAwcUtil.readFile("SPIFFS", filename);//read leakNext time from memory
//      System.out.println("strArr in ato form spiffs is : "+strArr);
      //Serial.print("strArr in ato form spiffs is : ");
      //Serial.println(strArr);
      //Serial.print("ato filename is ");
      //Serial.println(filename);
      if(strArr != ""){
/*///        atoNextArr = */tokenizeNextTime(strArr, atoNextArr); //put atoArr in leakNextArr
      //Serial.println("In strArr not nil ");
        for(int i = 0;i<5;i++){
          nextArr[i] = atoNextArr[i];
         //////System.out.println("nextArr in loop for ato is "+nextArr[i]);
        }
      }
    }else if(nextType == "awcNot"){
      spiffsXReset = spiffsAwcReset;
      notification = "awcNot";
      webSerialCode = "AWCN";
      filename = "/awcNot.txt";
      strArr = _atoAwcUtil->readFile(SPIFFS, filename);//read leakNext time from memory
//      strArr = _atoAwcUtil.readFile("SPIFFS", filename);//read leakNext time from memory
      //////System.out.println("strArr in awc from spiffs is : "+strArr);
      if(strArr != ""){
 /*///       awcNextArr = */tokenizeNextTime(strArr, awcNextArr); //put strArr in awcNextArr
        for(int i = 0;i<5;i++){
          nextArr[i] = awcNextArr[i];
          //////System.out.println("nextArr in loop for awc is "+nextArr[i]);
        }
      }
    }else if(nextType == "hbNot"){
      spiffsXReset = spiffsHbReset;
      notification = "hbNot";
      webSerialCode = "HBN";
      filename = "/hbNot.txt";
      strArr = _atoAwcUtil->readFile(SPIFFS, filename);//read leakNext time from memory
///     strArr = _atoAwcUtil.readFile("SPIFFS", filename);//read leakNext time from memory
      //////System.out.println("strArr in hb form spiffs is : "+strArr);
      if(strArr != ""){
/*///        hbNextArr = */tokenizeNextTime(strArr, hbNextArr); //put strArr in hbNextArr
        for(int i = 0;i<5;i++){
          nextArr[i] = hbNextArr[i];
          //////System.out.println("nextArr in loop for hb is "+nextArr[i]);
        }
      }
    }else if(nextType == "bdNot"){
      spiffsXReset = spiffsBdReset;
      notification = "bdNot";
      webSerialCode = "BDN";
      filename = "/bdnot.txt";
      strArr = _atoAwcUtil->readFile(SPIFFS, filename);
///     strArr = _atoAwcUtil.readFile("SPIFFS", filename);
      //////System.out.println("strArr in bd form spiffs is : "+strArr);
      if(strArr != ""){
        tokenizeNextTime(strArr, bdNextArr); //put strArr in bdNextArr
        for(int i = 0;i<5;i++){
          nextArr[i] = bdNextArr[i];
          //////System.out.println("nextArr in loop for bd is "+nextArr[i]);
        }
      }
    }else if(nextType == "gdNot"){
      spiffsXReset = spiffsGdReset;
      notification = "gdNot";
      webSerialCode = "GDN";
      filename = "/gdnot.txt";
      strArr = _atoAwcUtil->readFile(SPIFFS, filename);
///     strArr = _atoAwcUtil.readFile("SPIFFS", filename);
      //////System.out.println("strArr in gd form spiffs is : "+strArr);
      if(strArr != ""){
/*///        gdNextArr = */tokenizeNextTime(strArr, gdNextArr); //put strArr in hbNextArr
        for(int i = 0;i<5;i++){
          nextArr[i] = gdNextArr[i];
          //////System.out.println("nextArr in loop for gd is "+nextArr[i]);
        }
      }
    }else if(nextType == "ydNot"){
      spiffsXReset = spiffsYdReset;
      notification = "ydNot";
      webSerialCode = "YDN";
      filename = "/ydnot.txt";
      strArr = _atoAwcUtil->readFile(SPIFFS, filename);
///     strArr = _atoAwcUtil.readFile("SPIFFS", filename);
      //////System.out.println("strArr in yd form spiffs is : "+strArr);
      if(strArr != ""){
/*///        ydNextArr = */tokenizeNextTime(strArr, ydNextArr); //put strArr in hbNextArr
        for(int i = 0;i<5;i++){
          nextArr[i] = ydNextArr[i];
          //////System.out.println("nextArr in loop for yd is "+nextArr[i]);
        }
      }
    }else if(nextType == "pdNot"){
      spiffsXReset = spiffsPdReset;
      notification = "PdNot";
      webSerialCode = "PDN";
      filename = "/pdnot.txt";
      strArr = _atoAwcUtil->readFile(SPIFFS, filename);
///     strArr = _atoAwcUtil.readFile("SPIFFS", filename);
      //////System.out.println("strArr in pd form spiffs is : "+strArr);
      if(strArr != ""){
/*///        odNextArr = */tokenizeNextTime(strArr, pdNextArr); //put strArr in hbNextArr
        for(int i = 0;i<5;i++){
          nextArr[i] = pdNextArr[i];
          //////System.out.println("nextArr in loop for yd is "+nextArr[i]);
        }
      }
    } 
 
    if(nextType == notification){
      String next = arrToString(nextArr);
            //Serial.println("next time for "+notification+" is : "+next);
           //System.out.println("next time for "+notification+" is : "+next);
           //Web//System.out.print(web//System.outCode+":"+next+",");//TODO add notification to print so we know what it is aot or awc or hb, etc


     //strArr = _atoAwcUtil->readFile(SPIFFS, filename);//read Next time from memory
///    strArr = _atoAwcUtil.readFile("SPIFFS", filename);//read Next time from memory
    
      ////System.out.print("from save file is : "+strArr);
      ////System.out.print(" for filename :");
      ////System.out.println(filename);
      if(spiffsXReset == 1){  //FIRST TIME THROUGH OR A RESET HAPPENED (1)
        //Serial.println("IN ATONOT SPIFFX = 1");
        ////System.out.println("next spiffs time is reset 1");
        //Web//System.out.print("?R,");
        //Serial.println("not is spiffsXReset = 1");
        if(notification == "leakNot"){
          spiffsLReset = 0;
        }else if(notification == "highWaterNot"){
          spiffsHWReset = 0;
          ////System.out.println("just set reset  0000000000000000000000000000");
        }else if(notification == "lowWaterNot"){
          spiffsLWReset = 0;
          ////System.out.println("just set reset  0000000000000000000000000000");
        }else if(notification == "atoNot"){
          //Serial.println("IN ATONOT 3");
          spiffsAtoReset = 0;
          ////System.out.println("just set reset  0000000000000000000000000000");
          //Serial.println("just set reset atoNot 0000000000000000000000000000000");
        }else if(notification == "awcNot"){
          spiffsAwcReset = 0;
          ////System.out.println("just set reset  0000000000000000000000000000");
        }else if(notification == "hbNot"){
          spiffsHbReset = 0;
          ////System.out.println("just set reset  0000000000000000000000000000");
        }else if(notification == "bdNot"){
          spiffsBdReset = 0;
          ////System.out.println("just set reset  0000000000000000000000000000");
        }else if(notification == "gdNot"){
          spiffsGdReset = 0;
          ////System.out.println("just set reset  0000000000000000000000000000");
        }else if(notification == "ydNot"){
          spiffsYdReset = 0;
          ////System.out.println("just set reset  0000000000000000000000000000");
        }else if(notification == "pdNot"){
          spiffsPdReset = 0;
          ////System.out.println("just set reset  0000000000000000000000000000");
        }    
       //Serial.print("strArr for : ");
       //Serial.print(notification);
       //Serial.print(" is : ");
       //Serial.println(strArr); 
       if(strArr == ""){
        //Serial.println("strArr is blank");
       }else{
        //Serial.println("strArr isn't blank");
       }
      if(strArr == ""){  //THIS IS FIRST TIME THRU (1.1) CALC/SAVE RETURN FALSE MOVE ON 
          //Serial.println("no spiffs\:");
          //Web//System.out.print("?ES,");
          strTime = calcNextTime(notification,curTime); // put in addArr
           ////System.out.println("next calc time is : "+strTime);
           if(strTime != ""){
              saveNextTime(notification,strTime);
           }else{
            //Serial.println("Error in timer strTime is blank");
           }
          ////System.out.println("saved : "+strTime+" and returned false");
          //Serial.print("saved next time for : ");
          //Serial.print(notification);
          //Serial.print(" is :");
          //Serial.println(strTime);
          //System.out.print("1-1, ");
          return false;
        }else if(isSavedTimeGreaterThanCurTime(strArr) == "<" ){  //1ST TIME THRU.  A RESET!!!!CALC/SAVE
                                                            //HAS THE SYSTEM BEEN OFF SO LONG THE SAVED VALUE HAS PASSED
                                                            //CALC/SAVE MOVE ON...PRETTY MUCH HAVE TO START OVER RESTURN FALSE
          //Serial.println("i should have landed here");
          strTime = calcNextTime(notification,curTime); // put in addArr
          ////System.out.println("First time thru Not in blank and save time greater than strArr and TOKENIZE strArr**** and return falseand strArr is "+strArr);
          saveNextTime(notification,strTime);
          //Serial.print("should have saved strTime ");
          //Serial.println(strTime);
          //Web//System.out.print("#STNG_B,");
          //System.out.print("1-2, ");
          return false;
       }else if(isSavedTimeGreaterThanCurTime(strArr) == "=") {  //FIRST TIME THRU.  A RESET  SYSTEM HASN'T BEEN OFF TOO LONG AND 
                                                     //A VALUE SAVED SO ALL GOOD CHECK TO SEE IF EQUAL IF YES CALC/SAVE
                                                     //RETURN TRUE.  BINGO BINGO WE HIT OUR TIME
        //time to run 
        //calculate next run
          strTime = calcNextTime(notification, arrToString(nextArr));// put in addArr
///       strTime = calcNextTime(notification, strArr);// put in addArr
          saveNextTime(notification,strTime);    
          ////System.out.println("Not in blank and nextArr == curTime and next cal time is strTime is "+strTime+" and return true");
          //System.out.print("1-3, ");
          //Serial.print("1-3___________________________, ");
          return true;
        }else{  //A RESET.  SYSTEM HASN'T BEEN OFF TOO LONG AND SAVED NOT EQUAL SO SIMPLY RETURN FALSE
           //System.out.print("1-4, ");
          return false;
        }
      }else if(isSavedTimeGreaterThanCurTime(strArr) == "<" ){  //NOT A RESET SO NORMAL LOOP MAKE SURE SAVE TIME ISN'T LESS THEN CURRENT TIME
                                                          //WE WOULD NEVER RUN.  NOT SURE HOW WE WOULD HIT THIS SCENARIO.
                                                          //SO IF SAVE NOT GREATER THAN CUR, THEN CALC SAVE RETURN FALSE.  MOVE ON
          strTime = calcNextTime(notification,curTime); // put in addArr
          //System.out.println("Not first, Not in blank and save time greater than strArr and TOKENIZE strArr$$$ and return falseand strArr is "+strArr);
          saveNextTime(notification,strTime);
          //Web//System.out.print("#STNG_NB,");
         //System.out.println("2-1, ");
          return false;
      }else if(isSavedTimeGreaterThanCurTime(strArr) == "=") {   //NOT RESET SO NORMAL LOOP IS SAVE TIME EQUAL TO CURRENT THEN 
                                                      //CALC SAVE RETURN TRUE.  BINGO BINGO BINGO
        //calculate next run
          strTime = calcNextTime(notification, arrToString(nextArr));// put in addArr
///         strTime = calcNextTime(notification, strArr);// put in addArr
          saveNextTime(notification,strTime);    
          //System.out.println("Not first and Not in blank and nextArr == curTime and next cal time is strTime is "+strTime+" and return true");
          //System.out.println("2-2, ");
          return true;
      }else{  //ESLS NOT RESET SO NORAML LOOP AND NOT TIME YET.  RETRUN FALSE
          //System.out.println(curTime+" execute time : "+arrToString(nextArr)+"type is "+nextType);
          //System.out.println("2-3");
          return false;
      }
    }
      //System.out.println("2-4, ");
    return retVal;
  }
  
  //////////////////////////////////////////
  //
  /////////////////////////////////////////////
  void Timer::setCurrentTime(){
    //int i = 0;
  currentTimeArr = _atoAwcUtil->setCurrentTime();
/*    //Serial.print("current year in timer is :");
    Serial.print(currentTimeArr[0]);
    Serial.print(", current month in timer is :");
    Serial.print(currentTimeArr[1]);
    Serial.print(", current day in timer is :");
    Serial.print(currentTimeArr[2]);
    Serial.print(", current hour in timer is :");
    Serial.print(currentTimeArr[3]);
    Serial.print(", current minute in timer is :");
    Serial.print(currentTimeArr[4]);
    Serial.println(i);*/
  }

  ///////////////////////////////////////////////////////////////
  //
  ///////////////////////////////////////////////////////////////
  String Timer::arrToString(int *arra){
/// String arrToString(int arra[]) {
    String retString = "";
    String myStr = "";
    for(int i = 0;i<5;i++){
     //myStr = String.valueOf(arra[i]);
     myStr = String(arra[i]);
      //arra[i];
      if(i == 4){
        retString = retString+myStr;
      }else{
        retString = retString+myStr+",";
      }
    }
    return retString;
  }

  ////////////////////////////////////////////////
  //
  //////////////////////////////////////////////////
  void Timer::setYrMoDay(){
///  void setYrMoDay(){
        setCurrentTime();
        yrMoDay[0] = currentTimeArr[0];  //use these to see when new day occurs
        yrMoDay[1] =currentTimeArr[1];
        yrMoDay[2] = currentTimeArr[2];  
  }
  ////////////////////////////////////////////////
  //
  //////////////////////////////////////////////////
  bool Timer::changeOfDay(){
    bool retVal = false;
    setCurrentTime();
    setYrMoDay();
    int thisDay = currentYrMoDay[2];
    String thisDayStr = String(thisDay);
    String lastSavedDayStr = _atoAwcUtil->readFile(SPIFFS, "/thisDay.txt");
    int lastSavedDay = std::atoi(lastSavedDayStr.c_str());
    if(thisDay != lastSavedDay){
      _atoAwcUtil->writeFile(SPIFFS, "/thisDay.txt" ,thisDayStr.c_str());
      //Serial.print("Current year is: ");
      //Serial.println(currentYrMoDay[0]);
      //Serial.print("Current month is: ");
      //Serial.println(currentYrMoDay[1]);
      //Serial.print("Current day is: ");
      //Serial.println(currentYrMoDay[2]);
      String currDay = String(currentYrMoDay[2]);
      _atoAwcUtil->webSerialWrite(currDay);
      if(lastSavedDayStr == ""){ ///first time through spiffs return nothing
        return false;
      }
      _atoAwcUtil->webSerialWrite("New day is ");
      retVal = true;
    }
    return retVal;
  }

  ////////////////////////////////////////
  //
  //////////////////////////////////////////
  void Timer::saveNextTime(String nextType, String strTime){
/// void saveNextTime(String nextType, String strTime){
    String filename = "";
    if(nextType == "leakNot"){
      _atoAwcUtil->writeFile(SPIFFS, "/leakNot.txt", strTime.c_str());
///     _atoAwcUtil.writeFile("SPIFFS", "/leakNot.txt", strTime);
      //////System.out.print(" saved "+filename+" to spiff :");
      //////System.out.println(allTogether);
      /*///leakNextArr = */tokenizeNextTime(strTime, leakNextArr);
    }else if (nextType == "highWaterNot"){
      _atoAwcUtil->writeFile(SPIFFS, "/hwNot.txt", strTime.c_str());
///     _atoAwcUtil.writeFile("SPIFFS", "/hwNot.txt", strTime);
      //////System.out.print(" saved "+filename+" to spiff :");
     // ////System.out.println(allTogether);
      /*///hwNextArr = */tokenizeNextTime(strTime, hwNextArr);
    }else if (nextType == "lowWaterNot"){
      _atoAwcUtil->writeFile(SPIFFS, "/lwNot.txt", strTime.c_str());
///     _atoAwcUtil.writeFile("SPIFFS", "/lwNot.txt", strTime);
      //////System.out.print(" saved "+filename+" to spiff :");
     // ////System.out.println(allTogether);    
      /*///lwNextArr = */tokenizeNextTime(strTime, lwNextArr);
    }else if (nextType == "atoNot"){
      _atoAwcUtil->writeFile(SPIFFS, "/atoNot.txt", strTime.c_str());
///     _atoAwcUtil.writeFile("SPIFFS", "/atoNot.txt", strTime);
      //////System.out.print(" saved "+filename+" to spiff :");
      //////System.out.println(allTogether);    
      /*///atoNextArr = */tokenizeNextTime(strTime, atoNextArr);
    }else if (nextType == "awcNot"){
      _atoAwcUtil->writeFile(SPIFFS, "/awcNot.txt", strTime.c_str());
///     _atoAwcUtil.writeFile("SPIFFS", "/awcNot.txt", strTime);
      //////System.out.print(" saved "+filename+" to spiff :");
     // ////System.out.println(allTogether);    
      /*///awcNextArr = */tokenizeNextTime(strTime, awcNextArr);
    }else if (nextType == "hbNot"){
       _atoAwcUtil->writeFile(SPIFFS, "/hbNot.txt", strTime.c_str());
///      _atoAwcUtil.writeFile("SPIFFS", "/hbNot.txt", strTime);
      //////System.out.print(" saved "+filename+" to spiff :");
      //////System.out.println(allTogether);    
       /*///hbNextArr = */tokenizeNextTime(strTime, hbNextArr);
    }else if (nextType == "bdNot"){
       _atoAwcUtil->writeFile(SPIFFS, "/bdnot.txt", strTime.c_str());
///      _atoAwcUtil.writeFile("SPIFFS", "/bdnot.txt", strTime);
      //////System.out.print(" saved "+filename+" to spiff :");
      //////System.out.println(allTogether);    
       /*///bdNextArr = */tokenizeNextTime(strTime, bdNextArr);
    }else if (nextType == "gdNot"){
       _atoAwcUtil->writeFile(SPIFFS, "/gdnot.txt", strTime.c_str());
///      _atoAwcUtil.writeFile("SPIFFS", "/gdnot.txt", strTime);
      //////System.out.print(" saved "+filename+" to spiff :");
      //////System.out.println(allTogether);    
       /*///gdNextArr = */tokenizeNextTime(strTime, gdNextArr);
    }else if (nextType == "ydNot"){
       _atoAwcUtil->writeFile(SPIFFS, "/ydnot.txt", strTime.c_str());
///      _atoAwcUtil.writeFile("SPIFFS", "/ydnot.txt", strTime);
      //////System.out.print(" saved "+filename+" to spiff :");
      //////System.out.println(allTogether);    
       /*///ydNextArr = */tokenizeNextTime(strTime, ydNextArr);
    }else if (nextType == "pdNot"){
       _atoAwcUtil->writeFile(SPIFFS, "/pdnot.txt", strTime.c_str());
///      _atoAwcUtil.writeFile("SPIFFS", "/pdnot.txt", strTime);
      //////System.out.print(" saved "+filename+" to spiff :");
      //////System.out.println(allTogether);    
       /*///ydNextArr = */tokenizeNextTime(strTime, pdNextArr);
    }
  }

  //////////////////////////////////////////////////////////////
//                                                            //
  //   tokenizeNextTime                                       //
//                                                            //
  //   input: String nexttime time array "d,h,m"              //
//                                                            //
  //   output: array int [d,h,m]                              //
//                                                            //
  //   description:  runs the automatic water change (awc)    //
//                    1.)empties water to lower level         //
//                    2.)fills water back up to normal level  //
//                    calculates and stores how long both     //
//                    emty and fill took the first time and   //
//                    uses padded time to determin if either  //
//                    fill or empty took too long.            //
//                                                            //
  //////////////////////////////////////////////////////////////
  void Timer::tokenizeNextTime(String nexttime, int* ar){
/// int[] tokenizeNextTime(String nexttime, int arr[]){
///    int ar[]  = {0,0,0,0,0};
     //string my_string = "Hello,World,India,Earth,London";
     std::stringstream ss(nexttime.c_str()); //convert my_string into string stream

     std::vector<std::string> tokens;
    std::string temp_str;
///   StringTokenizer tokenizer=new StringTokenizer(nexttime,",");
 
     while(getline(ss, temp_str, ',')){ //use comma as delim for cutting string
        tokens.push_back(temp_str);
     }
    
///   while(tokenizer.hasMoreTokens()){
///     int tok = Integer.parseInt(tokenizer.nextToken());
///     ar[iii] = tok;
///     iii++;
///   }
///   iii = 0;
     for(int i = 0; i < tokens.size(); i++) {
        int ele = std::atoi(tokens[i].c_str());
        ar[i] = ele;
     }
///    return ar;  
    //free (ptr);
  }

  /////////////////////////////////
  //
  ////////////////////////////////
  String Timer::calcNextTime(String nextType, String lastTime){
/// String calcNextTime(String nextType, String lastTime){
    String strTime = "";
    String fileVal = "";
    String timeArr = "0,0,0,0,0"; //default
    if(nextType == "leakNot"){ 
     fileVal = _atoAwcUtil->readFile(SPIFFS, "/leaknotfreq.txt");
///     fileVal = _atoAwcUtil.readFile("SPIFFS", "/leaknotfreq.txt");
      //Serial.print("filename from web is : ");
      //Serial.println(fileVal);
    }else if (nextType == "highWaterNot"){
      fileVal = _atoAwcUtil->readFile(SPIFFS, "/hwnotfreq.txt");
///     fileVal = _atoAwcUtil.readFile("SPIFFS", "/hwnotfreq.txt");
      //////System.out.print("ato filename from web is : ");
      //////System.out.println(fileVal);
    }else if (nextType == "lowWaterNot"){
      //Serial.print("nextType in lowWaternot is :");
      //Serial.println(nextType);
      fileVal = _atoAwcUtil->readFile(SPIFFS, "/lwnotfreq.txt");
///     fileVal = _atoAwcUtil.readFile("SPIFFS", "/lwnotfreq.txt");
      //////System.out.print("filename from web is : ");
      //////System.out.println(fileVal);
    }else if (nextType == "atoNot"){
      fileVal = _atoAwcUtil->readFile(SPIFFS, "/atofreq.txt");
///     fileVal = _atoAwcUtil.readFile("SPIFFS", "/atofreq.txt");
      //////System.out.print("filename from web is : ");
      //////System.out.println(fileVal);
      //Serial.print("nextType in atonot is :");
      //Serial.println(nextType);
      //Serial.print("/atofreq.txt is : ");
      //Serial.println(fileVal);
    }else if (nextType == "awcNot"){
      fileVal = _atoAwcUtil->readFile(SPIFFS, "/awcfreq.txt");
///     fileVal = _atoAwcUtil.readFile("SPIFFS", "/awcfreq.txt");
      //////System.out.print("filename from web is : ");
      //////System.out.println(fileVal);
    }else if (nextType == "hbNot"){
      fileVal = _atoAwcUtil->readFile(SPIFFS, "/hbnotfreq.txt");
///     fileVal = _atoAwcUtil.readFile("SPIFFS", "/hbnotfreq.txt");
      //////System.out.print("filename from web is : ");
      //////System.out.println(fileVal);
   }else if (nextType == "bdNot"){
      fileVal = _atoAwcUtil->readFile(SPIFFS, "/bdnotfreq.txt");
///     fileVal = _atoAwcUtil.readFile("SPIFFS", "/bdnotfreq.txt");
      //////System.out.print("filename from web is : ");
      //////System.out.println(fileVal);
   }else if (nextType == "gdNot"){
      fileVal = _atoAwcUtil->readFile(SPIFFS, "/gdnotfreq.txt");
///     fileVal = _atoAwcUtil.readFile("SPIFFS", "/gdnotfreq.txt");
      //////System.out.print("filename from web is : ");
      //////System.out.println(fileVal);
   }else if (nextType == "ydNot"){
      fileVal = _atoAwcUtil->readFile(SPIFFS, "/ydnotfreq.txt");
///     fileVal = _atoAwcUtil.readFile("SPIFFS", "/ydnotfreq.txt");
      //////System.out.print("filename from web is : ");
      //////System.out.println(fileVal);
   }else if (nextType == "pdNot"){
      fileVal = _atoAwcUtil->readFile(SPIFFS, "/pdnotfreq.txt");
///     fileVal = _atoAwcUtil.readFile("SPIFFS", "/pdnotfreq.txt");
      //////System.out.print("filename from web is : ");
      //////System.out.println(fileVal);
   }
    if(fileVal == ""){
      fileVal = "4hour";
      //Serial.println("we are in default 4hour in timer");
    }
    //Serial.println("before convertWebTimeToStrArr() in calc");
    timeArr = convertWebTimeToStrArr(fileVal);
    //Serial.print("timeArr is :");
    //Serial.println(timeArr);
    //////System.out.println("time array is : ");
    //////System.out.print(timeArr);
    int tempArr[5];
///   int tempArr[] = {0,0,0,0,0};
     tokenizeNextTime(timeArr, tempArr); 
      //Serial.println("tempArr[0] is : ");
      //Serial.print(tempArr[0]);
    strTime = addTime(lastTime, tempArr[0],tempArr[1],tempArr[2],tempArr[3],tempArr[4]);
    //Serial.print("final strTime is : ");
    //Serial.println(strTime);
    if(nextType == "awcNot"){
      strTime = normalizeTimeOfDay(strTime);
    }
    //System.out.println("calc value is :"+strTime+"for type "+nextType);
    return strTime;
  }

  /////////////////////////////////////////
  //
  //////////////////////////////////////////
  String Timer::convertWebTimeToStrArr(String input){
/// String convertWebTimeToStrArr(String input){
    if(input == "always"){
      //Serial.println("we are in always in timer");
      return"0,0,0,0,5";
    }
    if(input == "twiceaday"){
      return"0,0,0,12,0";
    }
     if(input == "everyother"){
      return "0,0,2,0,0";
    }
     if(input == "onceaweek"){
      return "0,0,7,0,0";
    }
     if(input == "onceaday"){
      return "0,0,1,0,0";
    }
     if(input == "onceamonth"){
      return "0,1,0,0,0";
    }
     if(input == "never"){
      return "0,0,0,0,0";
    }
     if(input == "halfhour"){
      return "0,0,0,0,30";
    }
     if(input == "onehour"){
      return "0,0,0,1,0";
    }
     if(input == "1hour"){
      return "0,0,0,1,0";
    }
     if(input == "2hour"){
      return "0,0,0,2,0";
    }
     if(input == "3hour"){
      return "0,0,0,3,0";
    }
     if(input == "4hour"){
      return "0,0,0,4,0";
    }
     if(input == "8hour"){
      return "0,0,0,8,0";
    }
     return "";
  }

  //////////////////////////////////////////////////////////////
//                                                            //
  //   addTime                                                //
//                                                            //
  //   input: String current time "yr,mo,da,h,m"              //
//                                                            //
  //   output: array int [y,mo,d,h,m]                              //
//                                                            //
  //   description:  takes current time and add time for      //
//                   the new timer                            //
//                       yr     mo   day   hr     min         //
//                    ("02021,00001,00017,00010,00042")       //
//                    just simple math wit carry returns      //
//                    the new time for event                  //
  //////////////////////////////////////////////////////////////
  String Timer::addTime(String curT, int yr, int mo, int dday, int hhour, int m){
/// String addTime(String curT, int yr, int mo, int dday, int hhour, int m){
    String addedStrTime;
    //Serial.print("Curent time in addTime is ::");
    //Serial.println(curT);
      //for testing
      /*  curT = ("2020,2,28,23,59");
        leapYear = true;
        yr =0;
        mo = 0;
        dday = 0;
        hhour = 0;
        m=1; */
    bool push = false; //used for carry over if already at max i.e. 60+1 and next is already 60
///   boolean push = false;
      tokenizeNextTime(curT, addArr);
    //we can add minute to 59 once hit 60 start at zero and continue increment hhour
    //increment minutes
    //Serial.println(addArr[1]);
    //Serial.println("above passed into addTime");
    int curMin = addArr[4];
    //Serial.println(curMin);
    int curHour = addArr[3];
    //Serial.println(curMin);
    for(int i=0;i<m;i++){
      //Serial.println(i);
      if(curMin != 59){
        //Serial.println(curMin);
        curMin++;
        //Serial.println(curMin);
      }else{
        push = true;
        curMin = 0;///REALY
        //Serial.println(curMin);
        curHour++;
      }
    }
    addArr[4] = curMin;
    //Serial.println(addArr[4]);
    addArr[3] = curHour;
    
    curHour = addArr[3];
    int curDay = addArr[2];
      //Serial.print("curHour is : ");
      //Serial.println(curHour);
      if(curHour == 24 && push == true){
        curHour =0;
        curDay++;
        push = true;
      }else{
        push = false;
      }

    for(int i=0;i<hhour;i++){
        if(curHour != 23){
          curHour++;
        }else{
          push = true;
          curHour = 1;
          curDay++;
        }
    }
    addArr[3] = curHour;
    addArr[2] = curDay;
    curDay = addArr[2];
    int curMo = addArr[1];
    if(addArr[1] ==1 || addArr[1] == 3 || addArr[1] == 5 || addArr[1] == 7 ||
      addArr[1] == 8 || addArr[1] == 10 || addArr[1] == 12){
       //Serial.print("curDay is : ");
      //Serial.print(curDay);
     if(curDay == 32 && push == true){
        curDay =1;
        curMo++;
        push = true;
      }else{
        push = false;
      }
      for(int i=0;i<dday;i++){
        if(curDay != 31){
          curDay++;
        }else{
          push = true;
          curDay = 1;
          curMo++;
        }
    }
      addArr[2] = curDay;
      addArr[1] = curMo;
      }
     if(addArr[1] == 4 || addArr[1] == 6 || addArr[1] == 9 || addArr[1] == 11){
      //Serial.print("curDay is : ");
      //Serial.print(curDay);
       if(curDay == 31 && push == true){
        curDay =1;
        curMo++;
        push = true;
      }else{
        push = false;
      }  
      for(int i=0;i<dday;i++){
        if(curDay != 30){
          curHour++;
        }else{
          push = true;
          curDay = 1;
          curMo++;
        }
    }
      addArr[2] = curDay;
      addArr[1] = curMo;
     }
    if(addArr[1] == 2 && leapYear == true){
      //Serial.print("curDay is : ");
      //Serial.print(curDay);
        if(curDay == 29 && push == true){
        curDay =1;
        curMo++;
        push = true;
      }else{
        push = false;
      }
      for(int i=0;i<dday;i++){
        if(curDay != 29){
          curDay++;
        }else{
          push = true;
          curDay = 1;
          curMo++;
        }
      }
       addArr[2] = curDay;
      addArr[1] = curMo;

    }else  if(addArr[1] == 2){
      //Serial.print("curDay is : ");
      //Serial.print(curDay);
        if(curDay == 28 && push == true){
        curDay =1;
        curMo++;
        push = true;
      }else{
        push = false;
      }
      for(int i=0;i<dday;i++){
        if(curDay != 28){
          curDay++;
        }else{
          push = true;
          curDay = 1;
          curMo++;
        }
      }
       addArr[2] = curDay;
      addArr[1] = curMo;

    }
     
    curMo = addArr[1];
    //Serial.print("damn month is ");
    //Serial.println(curMo);
    int curYr = addArr[0];
    //Serial.print("Curent year is :");
    //Serial.println(curYr);
      if(curMo == 13 && push == true){
        curYr++;
        curMo = 1;
        push = false;
      }
      for(int i=0;i<mo;i++){
        if(curMo != 12){
          curMo++;
        }else{
          push = true;
          curMo = 1;
          curYr++;
        }
      }
      addArr[1] = curMo;
      addArr[0] = curYr;
  //char str[24] = "";
      String str = "";
      
      for(int j=0;j<5;j++){
        String s  = String(addArr[j]);
        //Serial.print(s);
        if(j!=4){
          //strcat(str,st);
          //strcat(str,",");
           str = str+s;
           str = str+",";
        }else{
          //strcat(str,st);
         str = str+s;
        }
      }
      addedStrTime = String(str);
      //Serial.println("*************************************");
      //Serial.println(addedStrTime);
      return addedStrTime;
  }



  ///////////////////////////////////////
  //
  //////////////////////////////////
  String Timer::isSavedTimeGreaterThanCurTime(String savedTime){
/// String isSavedTimeGreaterThanCurTime(String savedTime){
    String retVal = "<";
    setCurrentTime();
    setYrMoDay();
/*///     curSaveArr = */tokenizeNextTime(savedTime, curSaveArr);
    int adjCurYear = currentTimeArr[0]*1000000;
    int adjCurMo = currentTimeArr[1]*1000000;
    int adjCurDay = currentTimeArr[2]*1000;
    int adjCurHour = currentTimeArr[3]*100;
    int adjCurMin = currentTimeArr[4];
    int totalCur = (adjCurYear+adjCurMo+adjCurDay+adjCurHour+adjCurMin);
    //System.out.print("totalCur is :");
    //System.out.println(totalCur);

    int adjSavedCurYear = curSaveArr[0]*1000000;
    int adjSavedCurMo = curSaveArr[1]*1000000;
    int adjSavedCurDay = curSaveArr[2]*1000;
    int adjSavedCurHour = curSaveArr[3]*100;
    int adjSavedCurMin = curSaveArr[4];
    int totalSaved = (adjSavedCurYear+adjSavedCurMo+adjSavedCurDay+
        adjSavedCurHour+adjSavedCurMin);
    //System.out.print("totalSaved is :");
    //System.out.println(totalSaved);
    if(adjSavedCurYear > adjCurYear || adjSavedCurMo > adjCurMo ||
        adjSavedCurDay > adjCurDay) {
      return ">";
    }

    if(totalSaved == totalCur){
      return "=";
    }
    if(totalSaved >= totalCur){
      //Serial.println("time greater returning true");
       retVal = ">";
    }else{
      //Serial.println("time greater returning false");
      retVal =  "<";
    }
    return retVal;
  }



  ////////////////////////////////////////
  //
  //////////////////////////////////////////
  String  Timer::normalizeTimeOfDay(String strTime){
///   String  normalizeTimeOfDay(String strTime){
    String awcTod = _atoAwcUtil->readFile(SPIFFS, "/atotod.txt");
///   String  awcTod = _atoAwcUtil.readFile("SPIFFS", "/awctod.txt");
   
    int tempArr[5];
///   int tempArr[] = {0,0,0,0,0};
    int tod = awcTod.toInt();
///   int tod =Integer.valueOf(awcTod);
    int todPlusTwelve = tod+12;
    int curHour = currentTimeArr[3];
    for(int i = 0;i<5;i++){
      tempArr[i] = currentTimeArr[i];
    }
    if(curHour < tod || curHour > tod){
      if(curHour >= 12){
        tempArr[3]= tod;
        tempArr[2] = tempArr[2]+1;
      }else{
        tempArr[3] = tod+12;
      }
    }else {
        tempArr[3] = tod+12;
    }
    return arrToString(tempArr);
  }
    
//////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////

/*    public int getSpiffsLReset() {
      return spiffsLReset;
    }

    public void setSpiffsLReset(int spiffsLReset) {
      this.spiffsLReset = spiffsLReset;
    }

    public int getSpiffsHWReset() {
      return spiffsHWReset;
    }

    public void setSpiffsHWReset(int spiffsHWReset) {
      this.spiffsHWReset = spiffsHWReset;
    }

    public int getSpiffsLWReset() {
      return spiffsLWReset;
    }

    public void setSpiffsLWReset(int spiffsLWReset) {
      this.spiffsLWReset = spiffsLWReset;
    }

    public int getSpiffsAtoReset() {
      return spiffsAtoReset;
    }

    public void setSpiffsAtoReset(int spiffsAtoReset) {
      this.spiffsAtoReset = spiffsAtoReset;
    }

    public int getSpiffsAwcReset() {
      return spiffsAwcReset;
    }

    public void setSpiffsAwcReset(int spiffsAwcReset) {
      this.spiffsAwcReset = spiffsAwcReset;
    }

    public int getSpiffsHbReset() {
      return spiffsHbReset;
    }

    public void setSpiffsHbReset(int spiffsHbReset) {
      this.spiffsHbReset = spiffsHbReset;
    }

    public int getSpiffsBdReset() {
      return spiffsBdReset;
    }

    public void setSpiffsBdReset(int spiffsBdReset) {
      this.spiffsBdReset = spiffsBdReset;
    }

    public int getSpiffsGdReset() {
      return spiffsGdReset;
    }

    public void setSpiffsGdReset(int spiffsGdReset) {
      this.spiffsGdReset = spiffsGdReset;
    }

    public int getSpiffsYdReset() {
      return spiffsYdReset;
    }

    public void setSpiffsYdReset(int spiffsYdReset) {
      this.spiffsYdReset = spiffsYdReset;
    }
*/


  bool Timer::executeTest(String nextType, int howLong){
    //////System.out.println("in executeTest");
    if(executingTest == false){
      executingTest = true;
      ////System.out.println("started timer");
      testStart = millis();    
    }
    float cur = millis();
    float nowT = cur - testStart;
    if(nowT >= howLong){
      executingTest = false;
      ////System.out.println("execute test returning true");
      return true;
    }
    return false;
  }

  void Timer::setCurrentDay(){
    currentYrMoDay[0] = yrMoDay[0];
    currentYrMoDay[1] = yrMoDay[1];
    currentYrMoDay[2] = yrMoDay[2];
 /*   Serial.print("Current year is: ");
    Serial.println(currentYrMoDay[0]);
    Serial.print("Current month is: ");
    Serial.println(currentYrMoDay[1]);
    Serial.print("Current day is: ");
    Serial.println(currentYrMoDay[2]);*/
  }

  void Timer::setSpiffsLReset(int reset){
    spiffsLReset = reset;
  }
  void Timer::setSpiffsHWReset(int reset){
    spiffsHWReset = reset;
  }
  void Timer::setSpiffsLWReset(int reset){
    spiffsLWReset = reset;
  }
  void Timer::setSpiffsAtoReset(int reset){
    spiffsAtoReset - reset;
  }
  void Timer::setSpiffsAwcReset(int reset){
    spiffsAwcReset = reset;
  }
  void Timer::setSpiffsHbReset(int reset){
    spiffsHbReset;
  }
  void Timer::setSpiffsBdReset(int reset){
    spiffsBdReset = reset;
  }
  void Timer::setSpiffsGdReset(int reset){
    spiffsGdReset = reset;
  }
  void Timer::setSpiffsYdReset(int reset){
    spiffsYdReset = reset;
  }
  void Timer::setSpiffsPdReset(int reset){
    spiffsPdReset = reset;
  }
