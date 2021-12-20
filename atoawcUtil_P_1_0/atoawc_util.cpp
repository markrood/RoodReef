//#include <Arduino.h>
#include "atoawc_util.h"


AtoAwcUtil::AtoAwcUtil(AsyncWebServer *server)
{
  _server = server;
  //   _timeClient = new NTPClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);
  //  _timeClient->begin();

  Serial.begin(115200);
  if (!SPIFFS.begin(true)) {
    Serial.println("SPIFFS Mount Failed");
  }
}
////////////////////////////////////////////////////////////////////
//
//  Fuction: recordRun
//
//  Input:  String whatsRunning ("ato","blue, etc.),
//          bool startIt  true if start time false it stop timer
//
//  Output:  current run time
//
//  Discription:  call function with type of run to start (startIt = true
//                and stop with (startIt = false);  Stopwatch
//                this function then calls storFillTime to store the
//                accumulated time into SPIFFS (file system).
//
//  Calls:  storeFillTime
//
/////////////////////////////////////////////////////////////////////
float AtoAwcUtil::recordRun(String whatsRunning, bool startIt) {

  if (whatsRunning == "ato") {
    if (startIt && atoRunning == false) {
      atoRunning = true;
      atoStart = millis(); //TODO can only start it once not every time in loop
    } else if (!startIt && atoRunning) {
      atoRunning = false;
      atoStop = millis();
      float elapse = atoStop - atoStart;
      Serial.print("elapse time is ");
      Serial.println(elapse);
      atoStart = 0;
      atoStop = 0;
      atoRunning = false;
      //TODO use to devide elapse by 1000
      float currentAtoFill = elapse / 1000;
      Serial.print("currentAtoFill is ");
      Serial.println(currentAtoFill);
      storeFillTime("ato", currentAtoFill);
      currentFill = currentAtoFill;
    }
  } else  if (whatsRunning == "awc_empty") {
    if (startIt && awcEmptyRunning == false) {
      awcEmptyStart = millis();
      awcEmptyRunning = true;
    } else if (!startIt && awcEmptyRunning) {
      awcEmptyStop = millis();
      int elapse = awcEmptyStop - awcEmptyStart;
      Serial.print("elapse in awc empty is ");
      Serial.println(elapse);
      awcEmptyStart = 0;
      awcEmptyStop = 0;
      awcEmptyRunning = false;
      int currentAwcEmpty = elapse / 1000; //convert to seconds
      storeFillTime("awc_empty", currentAwcEmpty);
      currentFill = currentAwcEmpty;
    }
  } else  if (whatsRunning == "awc_fill") {
    if (startIt && awcFillRunning == false) {
      awcFillStart = millis();
      awcFillRunning = true;
    } else if (!startIt && awcFillRunning) {
      awcFillStop = millis();
      float elapse = awcFillStop - awcFillStart;
      awcFillStart = 0;
      awcFillStop = 0;
      awcFillRunning = false;
      float currentAwcFill = elapse / 1000; //convert to seconds
      storeFillTime("awc_fill", currentAwcFill);
      currentFill = currentAwcFill;
    }
  } else  if (whatsRunning == "yellow") { //TODO I haven't checked these!!!!!!!!!!!
    if (startIt) {
      yellowStart = millis();
    } else {
      yellowStop = millis();
      float elapse = yellowStop - yellowStart;
      float currentYellowFill = elapse / 1000; //convert to seconds
      yellowStart = 0;
      yellowStop = 0;
      if (calibrating == false) { //dont store and add up calibrate times
        storeFillTime("yellowDose", currentYellowFill);
      } else {
        oneHundredmlYellowTime = currentYellowFill;  //the time it took to fill 100ml
        if (oneHundredmlYellowTime >= 10) {
          storeFillTime("yellowCal", oneHundredmlYellowTime);
        }
      }
      currentFill = currentYellowFill;
    }
  } else  if (whatsRunning == "purple") { //TODO I haven't checked these!!!!!!!!!!!
    if (startIt) {
      purpleStart = millis();
    } else {
      purpleStop = millis();
      float elapse = purpleStop - purpleStart;
      float currentPurpleFill = elapse / 1000; //convert to seconds
      purpleStart = 0;
      purpleStop = 0;
      if (calibrating == false) { //dont store and add up calibrate times
        storeFillTime("purpleDose", currentPurpleFill);
      } else {
        oneHundredmlPurpleTime = currentPurpleFill;  //the time it took to fill 100ml
        if (oneHundredmlPurpleTime >= 10) {
          storeFillTime("purpleCal", oneHundredmlPurpleTime);
        }
      }
      currentFill = currentPurpleFill;
    }
  } else  if (whatsRunning == "green") {
    if (startIt) {
      greenStart = millis();
    } else {
      greenStop = millis();
      float elapse = greenStop - greenStart;
      greenStart = 0;
      greenStop = 0;
      float currentGreenFill = elapse / 1000; //convert to seconds
      if (calibrating == false) { //dont store and add up calibrate times
        storeFillTime("greenDose", currentGreenFill);
      } else {
        oneHundredmlGreenTime = currentGreenFill;  //time it took to fill 100ml
        if (oneHundredmlGreenTime >= 10) {
          storeFillTime("greenCal", oneHundredmlGreenTime);
        }
      }
      currentFill = currentGreenFill;
    }
  } else  if (whatsRunning == "blue") {
    if (startIt) {
      blueStart = millis();
      String strBlueStart = String(blueStart);
      //Serial.println("blueStart is "+strBlueStart);
    } else {
      blueStop = millis();
      String strBlueStop = String(blueStop);
      //Serial.println("blueStop is "+strBlueStop);
      float elapse = blueStop - blueStart;
      blueStart = 0;
      blueStop = 0;
      String strElapse = String(elapse);
      //Serial.println("elapse is "+strElapse);
      float currentBlueFill = elapse / 1000; //convert to seconds
      if (calibrating == false) { //don't store and add up calibrate times
        storeFillTime("blueDose", currentBlueFill);
      } else {
        oneHundredmlBlueTime = currentBlueFill;  //time it took to fill 100ml
        if (oneHundredmlBlueTime >= 10) {
          storeFillTime("blueCal", oneHundredmlBlueTime);
        }
      }
      currentFill = currentBlueFill;
    }
  }
  return currentFill;
}

////////////////////////////////////////////////////////////////////
//
//  Fuction: storeFillTime
//
//  Input:  String type ("ato", "awc",
//
//  Output:  node
//
//  Discription:  Stores puts runtime of ato, awc, blue, green,
//                yellow purple dosing in Spiffs ->
//
//  SPIFFS variables:  atoDailyFill, awcDaileyFill, blueDaileyFilel,
//                     yellowDaileyFill, purpleDaileyFill.
//
/////////////////////////////////////////////////////////////////////
void AtoAwcUtil::storeFillTime(String type, float fillTime) {
  float atoDailyFill = 0;
  float awcDailyFill = 0;
  float awcDailyEmpty = 0;
  //float blueDailyFill = 0;
  //float greenDailyFill = 0;
  //float yellowDailyFill = 0;

  if (type == "ato") {
    String daileyAtoFill = readFile(SPIFFS, "/atodailyfill.txt");
    if (daileyAtoFill == "") {
      Serial.println("in atodailyfill blank");
      atoDailyFill = 0;
      atoDailyFill = atoDailyFill + fillTime;
    } else {
      Serial.println("atodailyfill NOT blank");
      //float dft = daileyAtoFill.toFloat();
      atoDailyFill = daileyAtoFill.toFloat();
      atoDailyFill = atoDailyFill + fillTime;
    }
    String dailyStr = String(atoDailyFill);
    Serial.println("ato daily seconds  is :" + dailyStr); //TODO remove this
    writeFile(SPIFFS, "/atodailyfill.txt", dailyStr.c_str());
    Serial.print("ato daily fill seconds is ");
    Serial.println(dailyStr);
  }
  if (type == "awc") {
    String daileyAwcFill = readFile(SPIFFS, "/awcdailyfill.txt");
    if (daileyAwcFill == "") {
      awcDailyFill = 0;
      awcDailyFill = awcDailyFill + fillTime;
    } else {
      awcDailyFill = daileyAwcFill.toFloat();
      awcDailyFill = awcDailyFill + fillTime;
    }
    String daileyStr = String(awcDailyFill);
    writeFile(SPIFFS, "/awcdailyfill.txt", daileyStr.c_str());
    Serial.println("awc daily seconds is :" + daileyStr); //TODO remove this
    Serial.print("awc daily fill seconds is :");
    Serial.println(daileyStr);
  }
  if (type == "awc_empty") {
    String dailyAwcEmpty = readFile(SPIFFS, "/awcdailyempty.txt");
    if (dailyAwcEmpty == "") {
      awcDailyEmpty = 0;
      awcDailyEmpty = awcDailyEmpty + fillTime;
    } else {
      awcDailyEmpty = dailyAwcEmpty.toFloat();
      awcDailyEmpty = awcDailyEmpty + fillTime;
    }
    String dailyStr = String(awcDailyEmpty);
    writeFile(SPIFFS, "/awcdailyEmpty.txt", dailyStr.c_str());
    Serial.println("awc daily milliseconds is :" + dailyStr); //TODO remove this
    Serial.print("awc daily fill seconds is :");
    Serial.println(dailyStr);
  }
  if (type == "awc_fill") {
    String daileyAwcFill = readFile(SPIFFS, "/awcdailyfill.txt");
    if (daileyAwcFill == "") {
      awcDailyFill = 0;
      awcDailyFill = awcDailyFill + fillTime;
    } else {
      awcDailyFill = daileyAwcFill.toFloat();
      awcDailyFill = awcDailyFill + fillTime;
    }
    String daileyStr = String(awcDailyFill);
    writeFile(SPIFFS, "/awcdailyfill.txt", daileyStr.c_str());
    Serial.println("awc daily milliseconds is :" + daileyStr); //TODO remove this
    Serial.print("awc daily fill seconds is :");
    Serial.println(daileyStr);
  }
  if (type == "blueDose") {
    daileybdFill = readFile(SPIFFS, "/bddailyfill.txt");
    if (daileybdFill == "") {
      blueDailyFill = 0;
      blueDailyFill = blueDailyFill + fillTime;
    } else {
      blueDailyFill = daileybdFill.toFloat();
      blueDailyFill = blueDailyFill + fillTime;
    }
    String daileyStr = String(blueDailyFill);
    writeFile(SPIFFS, "/bddailyfill.txt", daileyStr.c_str());
    Serial.println("blue daily seconds is :" + daileyStr); //TODO remove this
  }
  if (type == "greenDose") {
    daileygdFill = readFile(SPIFFS, "/gddailyfill.txt");
    if (daileygdFill == "") {
      greenDailyFill = 0;
      greenDailyFill = greenDailyFill + fillTime;
    } else {
      greenDailyFill = daileygdFill.toFloat();
      greenDailyFill = greenDailyFill + fillTime;
    }
    String daileyStr = String(greenDailyFill);
    writeFile(SPIFFS, "/gddailyfill.txt", daileyStr.c_str());
    Serial.println("green daily seconds is :" + daileyStr); //TODO remove this
  }
  if (type == "yellowDose") {
    daileyydFill = readFile(SPIFFS, "/yddailyfill.txt");
    if (daileyydFill == "") {
      yellowDailyFill = 0;
      yellowDailyFill = yellowDailyFill + fillTime;
    } else {
      yellowDailyFill = daileyydFill.toFloat();
      yellowDailyFill = yellowDailyFill + fillTime;
    }
    String daileyStr = String(yellowDailyFill);
    writeFile(SPIFFS, "/yddailyfill.txt", daileyStr.c_str());
    Serial.println("yellow daily seconds is :" + daileyStr); //TODO remove this
  }
  if (type == "purpleDose") {
    daileypdFill = readFile(SPIFFS, "/pddailyfill.txt");
    if (daileypdFill == "") {
      purpleDailyFill = 0;
      purpleDailyFill = purpleDailyFill + fillTime;
    } else {
      purpleDailyFill = daileypdFill.toFloat();
      purpleDailyFill = purpleDailyFill + fillTime;
    }
    String daileyStr = String(purpleDailyFill);
    writeFile(SPIFFS, "/pddailyfill.txt", daileyStr.c_str());
    Serial.println("purple daily seconds is :" + daileyStr); //TODO remove this
  }
  if (type == "blueCal") {
    //blueDailyFill = daileybdFill.toFloat();
    String fillTimeStr = String(fillTime);
    writeFile(SPIFFS, "/blcal.txt", fillTimeStr.c_str());
    Serial.println("blue calibration seconds is :" + fillTimeStr); //TODO remove this
  }
  if (type == "greenCal") {
    //greenDailyFill = daileygdFill.toFloat();
    String fillTimeStr = String(fillTime);
    writeFile(SPIFFS, "/gncal.txt", fillTimeStr.c_str());
    Serial.println("green calibration seconds is :" + fillTimeStr); //TODO remove this
  }
  if (type == "yellowCal") {
    // yellowDailyFill = daileyydFill.toFloat();
    String fillTimeStr = String(fillTime);
    writeFile(SPIFFS, "/ywcal.txt", fillTimeStr.c_str());
    Serial.println("yellow calibration seconds is :" + fillTimeStr); //TODO remove this
  }
  if (type == "purpleCal") {
    // purpleDailyFill = daileypdFill.toFloat();
    String fillTimeStr = String(fillTime);
    writeFile(SPIFFS, "/pwcal.txt", fillTimeStr.c_str());
    Serial.println("purple calibration seconds is :" + fillTimeStr); //TODO remove this
  }
}

////////////////////////////////////////////////////////////////////
//
//  Fuction: readFile
//
//  Input:  SPIFFS,
//          char[]  filename path i.e. "/String.txt"
//
//  Output:  String of what was strored
//
//  Discription:  Stores a string in the /path in SPIFFS
//
/////////////////////////////////////////////////////////////////////
String AtoAwcUtil::readFile(fs::FS &fs, const char * path) {
  //Serial.printf("Reading file: %s\r\n", path);
  File file = fs.open(path, "r");
  if (!file || file.isDirectory()) {
    //Serial.println("- empty file or failed to open file");
    return String();
  }
  //Serial.println("- read from file:");
  String fileContent;
  while (file.available()) {
    fileContent += String((char)file.read());
  }
  //Serial.println(fileContent);
  file.close();
  return fileContent;
}

////////////////////////////////////////////////////////////////////
//
//  Fuction: writeFile
//
//  Input:  SPIFFS,
//          char[] filename path i.e. "/String.txt"
//          String to store
//
//  Output:  String of what was strored
//
//  Discription:  Stores a string in the /path in SPIFFS
//
/////////////////////////////////////////////////////////////////////
void AtoAwcUtil::writeFile(fs::FS &fs, const char * path, const char * message) {
  //Serial.printf("Writing file: %s\r\n", path);
  //Serial.print("path is : ");
  //Serial.println(path);
  //fs.remove(path);
  File file = fs.open(path, "w");
  if (!file) {
    Serial.println("- failed to open file for writing");
    //WebSerial.println("?EW"); //TODO change to right code
    return;
  }
  
  if (file.print(message)) {
    //Serial.println("- file written");
    if ((strcmp(path, "/timezone.txt") == 0)) {
      //Serial.println("Timezone changed!!!!!!!!11");
    }

  } else {
    Serial.println("- write failed");
  }
}



//////////////////////////////////////////////////////////////
//                                                          //
//   startSpiffs                                            //
//                                                          //
//   input: none                                            //
//                                                          //
//   output: none                                           //
//                                                          //
//   description:  starts of the file disk system.          //
//                                                          //
//////////////////////////////////////////////////////////////
void AtoAwcUtil::startSpiffs() {
  if (!SPIFFS.begin()) {
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }

}

//////////////////////////////////////////////////////////////
//                                                          //
//   sentHttp                                               //
//                                                          //
//   input: String message                                  //
//                                                          //
//   output: int (not used)                                 //
//                                                          //
//   description:  this uses the ifttt service to send      //
//                  push notifications                      //
//
//                                                          //
//////////////////////////////////////////////////////////////
int AtoAwcUtil::sendHttp(String event) {
  int ret = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    //Serial.print("*");
  }

  //Serial.println("");
  // Serial.println("WiFi connected");
  //Serial.println("IP address: ");
  // Serial.println(WiFi.localIP());


  //Serial.print("connecting to ");
  //Serial.println(host + url);
  //  client.setInsecure();
  if (!client.connect(host, httpsPort)) {

    Serial.println("connection failed");
    return 0;
  }
  //Serial.print("requesting URL: ");
  String iftt = readFile(SPIFFS, "/ifttt.txt");

  if (event == "Email") {
    //Serial.println("sending email");
    //url = "/trigger/" + event + "/with/key/"+iftt;//+",params={ \"value1\" : \""+iPAddress+"\", \"value2\" : \"02\", \"value3\" : \"03\" }";
    //Serial.println(url);
    //https://maker.ifttt.com/trigger/garage_deur/with/key/d56e34gf756/?value1=8&value2=5&value3=good%20morning
    //TESTING JSON CREATION
    url = "/trigger/" + event + "/with/key/" + iftt;
    //Serial.println("Starting JSON");
    StaticJsonDocument<80> jdata;
    //        StaticJsonBuffer<69> jsonBuffer;
    String json = "{ \"value1\" : \"atoawc ip: " + iPAddress + "\", \"value2\" : \", atoawc hotspot pw: ato_awc_\", \"value3\" : \", doser hotspot pw : yourdoser\" }";
    auto error = deserializeJson(jdata, json);
    //        JsonObject& root = jsonBuffer.parseObject(json);
    //Serial.println("TESTING POST");

    client.print(String("POST ") + url + " HTTP/1.1\r\n" +
                 "Host: " + host + "\r\n" +
                 //"Connection: close\r\n" +
                 "Content-Type: application/json\r\n" +
                 "Content-Length: " + json.length() + "\r\n" +
                 "\r\n" + // This is the extra CR+LF pair to signify the start of a body
                 json + "\n");
  } else {

    //url = "/trigger/"+event+"/with/key/bOZMgCFy7Bnhee7ZRzyi19";
    url = "/trigger/" + event + "/with/key/" + iftt;

    //Serial.println(url);

    client.print(String("GET ") + url + " HTTP/1.1\r\n" +
                 "Host: " + host + "\r\n" +
                 "User-Agent: BuildFailureDetectorESP8266\r\n" +
                 "Connection: close\r\n\r\n");

    //Serial.println("request Sent");
  }
  while (client.connected()) {
    String line = client.readStringUntil('\n');
    if (line == "\r") {
      // Serial.println("headers received");
      break;
    }
  }
  String line = client.readStringUntil('\n');

  //Serial.println("reply was:");
  //Serial.println("==========");
  //Serial.println(line);
  //Serial.println("==========");
  //Serial.println("closing connection");
  ret = 1;
  return ret;

}

////////////////////////////////////////////////////////////////////
//
//  Fuction: checkIfRanTooLong
//
//  Input:  //
//  Output:  none
//
//  Discription://
//
//  Calls:
//
/////////////////////////////////////////////////////////////////////
bool AtoAwcUtil::checkIfRanTooLong(String type, int runTime) {
  bool retVal = false;
  int total = 0;
  String atorunduration = readFile(SPIFFS, "/atorunduration.txt");
  String awcemptyduration = readFile(SPIFFS, "/awcemptyduration.txt");
  String awcfillduration = readFile(SPIFFS, "/awcfillduration.txt");
  String runDuration = String(runTime);
  //float runDurationHigh = runTime+(runTime*0.5);
  //float runDurationLow = runTime-(runTime*0.5);
  if (type == "ato") {
    Serial.print("the ATO runtime is for filling passed to check is ");
    Serial.println(runTime);
    if (atorunduration == "") { //no run duration recorded yet!
      writeFile(SPIFFS, "/atorunduration.txt", runDuration.c_str());
      retVal = false;
    } else {
      if (aveCount == 0) {
        total = atorunduration.toInt();
        aveCount = 1;
      }
    }
    float ave = averageSinceReset("ato", total, runTime);
    Serial.println("average is " + String(ave));
    float runDurationHigh = ave + (ave * 0.5);
    Serial.println("run duration highest allowed is " + String(runDurationHigh));
    if (runTime <= runDurationHigh) {
      retVal = false;
    } else {
      retVal = true;
    }
    return retVal;
  } else if (type == "awc_empty") {
    Serial.print("the runtime is for AWCemptying passed to check is ");
    Serial.println(runTime);
    if (awcemptyduration == "") { //no run duration recorded yet!
      writeFile(SPIFFS, "/awcemptyduration.txt", runDuration.c_str());
      retVal = false;
    } else {
      if (aveAwcEmtyCount == 0) {
        total = awcemptyduration.toInt();
        aveAwcEmtyCount = 1;
      }
    }
    float ave = averageSinceReset("awc_empty", total, runTime);
    Serial.println("Awc empty average is " + String(ave));
    float runDurationHigh = ave + (ave * 0.5);
    Serial.println("Awc empty run duration highest allowed is " + String(runDurationHigh));
    if (runTime <= runDurationHigh) {
      retVal = false;
    } else {
      retVal = true;
    }
    return retVal;
  } else if (type == "awc_fill") {
    Serial.print("the  runtime is for AWC filling passed to check is ");
    Serial.println(runTime);
    if (awcfillduration == "") { //no run duration recorded yet!
      writeFile(SPIFFS, "/awcfillduration.txt", runDuration.c_str());
      retVal = false;
    } else {
      if (aveAwcFillCount == 0) {
        total = awcfillduration.toInt();
        aveAwcFillCount = 1;
      }
    }
    float ave = averageSinceReset("awc_fill", total, runTime);
    Serial.println("Awc empty average is " + String(ave));
    float runDurationHigh = ave + (ave * 0.5);
    Serial.println("Awc empty run duration highest allowed is " + String(runDurationHigh));
    if (runTime <= runDurationHigh) {
      retVal = false;
    } else {
      retVal = true;
    }
    return retVal;
  } else if (type == "ato_now") {
    if (atorunduration == "") { //no run duration recorded yet!
      retVal = false;
      Serial.println(1);
      return retVal;
    }
    //TODO fix this changed the value from 0.5 to 2
    float runDurationHigh = atorunduration.toInt() + ((atorunduration.toInt()) * 2.0);
    if (runTime <= runDurationHigh) {
      Serial.println(2);
      retVal = false;
    } else {
      Serial.println(3);
      retVal = true;
      Serial.println("run duration highest allowed is " + String(runDurationHigh));
      Serial.print("the run that took too long value was ");
      Serial.println(String(runTime));
    }
    return retVal;
  } else if (type == "awc_now_empty") {
    if (awcemptyduration == "") { //no run duration recorded yet!
      retVal = false;
      Serial.println(4);
      return retVal;
    }    //TODO fix this changed the value from 0.5 to 2
    int runDurationHigh = awcemptyduration.toInt() + ((awcemptyduration.toInt()) * 2.0);
    if (runTime <= runDurationHigh) {
      Serial.println(5);
      retVal = false;
    } else {
      Serial.println(6);
      retVal = true;
      Serial.println("run awc empty duration highest allowed is " + String(runDurationHigh));
      Serial.print("the run awc empty that took too long value was ");
      Serial.println(String(runTime));
    }
    return retVal;
  } else if (type == "awc_now_fill") {
    if (awcfillduration == "") { //no run duration recorded yet!
      retVal = false;
      return retVal;
    }    //TODO fix this changed the value from 0.5 to 2
    float runDurationHigh = awcfillduration.toInt() + ((awcfillduration.toInt()) * 2.0);
    if (runTime <= runDurationHigh) {
      retVal = false;
    } else {
      retVal = true;
      Serial.println("run awc fill duration highest allowed is " + String(runDurationHigh));
      Serial.print("the run awc fill that took too long value was ");
      Serial.println(String(runTime));
    }

  }
  return retVal;
}

///////////////////////////////////////////////
//
//
////////////////////////////////////////////////////
float AtoAwcUtil::averageSinceReset(String type, float total, float val) {
  float ave = 0;
  if (type == "ato_fill") {
    total = total + val;
    aveCount++;
    ave = total / aveCount;
    Serial.println("ave is " + String(ave));
  } else if (type == "awc_empty") {
    Serial.print("total is :");
    Serial.println(total);
    Serial.print("val is :");
    Serial.println(val);
    total = total + val;
    aveAwcEmtyCount++;
    ave = total / aveAwcEmtyCount;
    Serial.println("ave is " + String(ave));
  } else if (type == "awc_fill") {
    total = total + val;
    aveAwcFillCount++;
    ave = total / aveAwcFillCount;
    Serial.println("ave is " + String(ave));
  }
  return ave;
}

///////////////////////////////////////////////
//
//////////////////////////////////////////////
//////////////////////////////////////////////////////////////
//                                                          //
//   changeMonthStrToNum                                    //
//                                                          //
//   input: string                                          //
//                                                          //
//   output: string                                            //
//                                                          //
//   description:  Need month string to number string       //
//                  for Highchart array                     //
//                                                          //
//////////////////////////////////////////////////////////////
String AtoAwcUtil::changeMonthStrToNum(String thisMonth) {
  String thisMonthNum = "0";
  if (thisMonth == "january") {
    thisMonthNum = "1";
  } else if (thisMonth == "february") {
    thisMonthNum = "2";
  } else if (thisMonth == "march") {
    thisMonthNum = "3";
  } else if (thisMonth == "april") {
    thisMonthNum = "4";
  } else if (thisMonth == "may") {
    thisMonthNum = "5";
  } else if (thisMonth == "june") {
    thisMonthNum = "6";
  } else if (thisMonth == "july") {
    thisMonthNum = "7";
  } else if (thisMonth == "august") {
    thisMonthNum = "8";
  } else if (thisMonth == "september") {
    thisMonthNum = "9";
  } else if (thisMonth == "october") {
    thisMonthNum = "10";
  } else if (thisMonth == "november") {
    thisMonthNum = "11";
  } else if (thisMonth == "december") {
    thisMonthNum = "12";
  }
  //Serial.println("__________________________");
  //Serial.println(thisMonthNum);
  return thisMonthNum;
}
//////////////////////////////////////////////////////////////
//                                                          //
//   changeMonthNumToStr                                    //
//                                                          //
//   input: int                                            //
//                                                          //
//   output: string                                         //
//                                                          //
//   description:  Need month string to number string       //
//                  for Highchart array                     //
//                                                          //
//////////////////////////////////////////////////////////////
String AtoAwcUtil::changeMonthNubToStr(int thisMonth) {
  String thisMonthNum = "0";
  if (thisMonth == 1) {
    thisMonthNum = "january";
  } else if (thisMonth == 2) {
    thisMonthNum = "february";
  } else if (thisMonth == 3) {
    thisMonthNum = "march";
  } else if (thisMonth == 4) {
    thisMonthNum = "april";
  } else if (thisMonth == 5) {
    thisMonthNum = "may";
  } else if (thisMonth == 6) {
    thisMonthNum = "june";
  } else if (thisMonth == 7) {
    thisMonthNum = "july";
  } else if (thisMonth == 8) {
    thisMonthNum = "august";
  } else if (thisMonth == 9) {
    thisMonthNum = "september";
  } else if (thisMonth == 10) {
    thisMonthNum = "october";
  } else if (thisMonth == 11) {
    thisMonthNum = "november";
  } else if (thisMonth == 12) {
    thisMonthNum = "december";
  }
  //Serial.println("__________________________");
  //Serial.println(thisMonthNum);
  return thisMonthNum;
}

//////////////////////////////////////////
//
/////////////////////////////////////////////
int* AtoAwcUtil::setCurrentTime() {
  _datetime->updateTime();
  //y,mo,day,hour,min
  currentTimeArr[0] = _datetime->getCurrentYear();
  currentTimeArr[1] = _datetime->getCurrentMonth();
  currentTimeArr[2] = _datetime->getCurrentDay();
  currentTimeArr[3] = _datetime->getCurrentHour();
  currentTimeArr[4] = _datetime->getCurrentMinute();
  return currentTimeArr;
}

//////////////////////////////////////////////////////////////
//                                                            //
//   setTimeZone                                            //
//                                                            //
//   input: char the val from web dropdown                  //
//                                                            //
//   output: none                                           //
//                                                            //
//   description:  sets utcOffsetInSeconds for date         //
//                    timezone for your area                  //
//                                                            //
//////////////////////////////////////////////////////////////
void AtoAwcUtil::setTimeZone(String tz) {
  /// void setTimeZone(String tz){
  Serial.print("web timezone text is ");
  Serial.println(tz);
  if (tz.equals(String("Etc/GMT+12"))) {
    utcOffsetInSeconds = -43200;
  } else if (tz.equals(String("Pacific/Midway"))) {
    utcOffsetInSeconds = -39600;
  } else if (tz.equals(String("Pacific/Honolulu"))) {
    utcOffsetInSeconds = -36000;
  } else if (tz.equals(String("US/Alaska"))) {
    utcOffsetInSeconds = -32400;
  } else if (tz.equals(String("America/Los_Angeles"))) {
    utcOffsetInSeconds = -28800;
  } else if (tz.equals(String("America/Tijuana"))) {
    utcOffsetInSeconds = -28800;
  } else if (tz.equals(String("US/Arizona"))) {
    utcOffsetInSeconds = -25200;
    // _timeClient->setTimeOffset(-25200);
  } else if (tz.equals(String("America/Chihuahua"))) {
    utcOffsetInSeconds = -25200;
  } else if (tz.equals(String("US/Mountain"))) {
    utcOffsetInSeconds = -25200;
  } else if (tz.equals(String("America/Managua"))) {
    utcOffsetInSeconds = -21600;
  } else if (tz.equals(String("US/Central"))) {
    utcOffsetInSeconds = -21600;
    Serial.println("WE HIT CST");
  } else if (tz.equals(String("America/Mexico_City"))) {
    utcOffsetInSeconds = -21600;
  } else if (tz.equals(String("Canada/Saskatchewan"))) {
    utcOffsetInSeconds = -21600;
  } else if (tz.equals(String("America/Bogota"))) {
    utcOffsetInSeconds = -18000;
  } else if (tz.equals(String("US/Eastern"))) {
    utcOffsetInSeconds = -18000;
    Serial.println("WE HIT EST");
  } else if (tz.equals(String("US/East-Indiana"))) {
    utcOffsetInSeconds = -18000;
  } else if (tz.equals(String("Canada/Atlantic"))) {
    utcOffsetInSeconds = -14400;
  } else if (tz.equals(String("America/Caracas"))) {
    utcOffsetInSeconds = -14400;
  } else if (tz.equals(String("America/Manaus"))) {
    utcOffsetInSeconds = -14400;
  } else if (tz.equals(String("America/Santiago"))) {
    utcOffsetInSeconds = -14400;
  } else if (tz.equals(String("Canada/Newfoundland"))) {
    utcOffsetInSeconds = -10800;
  } else if (tz.equals(String("America/Sao_Paulo"))) {
    utcOffsetInSeconds = -10800;
  } else if (tz.equals(String("America/Argentina/Buenos_Aires"))) {
    utcOffsetInSeconds = -10800;
  } else if (tz.equals(String("America/Godthab"))) {
    utcOffsetInSeconds = -10800;
  } else if (tz.equals(String("America/Montevideo"))) {
    utcOffsetInSeconds = -10800;
  } else if (tz.equals(String("America/Noronha"))) {
    utcOffsetInSeconds = -7200;
  } else if (tz.equals(String("Atlantic/Cape_Verde"))) {
    utcOffsetInSeconds = -3600;
  } else if (tz.equals(String("Atlantic/Azores"))) {
    utcOffsetInSeconds = -3600;
  } else if (tz.equals(String("Africa/Casablanca"))) {
    utcOffsetInSeconds = 0;
  } else if (tz.equals(String("Etc/Greenwich"))) {
    utcOffsetInSeconds = 0;
  } else if (tz.equals(String("Europe/Amsterdam"))) {
    utcOffsetInSeconds = 3600;
  } else if (tz.equals(String("Europe/Belgrade"))) {
    utcOffsetInSeconds = 3600;
  } else if (tz.equals(String("Europe/Belgrade"))) {
    utcOffsetInSeconds = 3600;
  } else if (tz.equals(String("Europe/Brussels"))) {
    utcOffsetInSeconds = 3600;
  } else if (tz.equals(String("Europe/Sarajevo"))) {
    utcOffsetInSeconds = 3600;
  } else if (tz.equals(String("Africa/Lagos"))) {
    utcOffsetInSeconds = 3600;
  } else if (tz.equals(String("Asia/Amman"))) {
    utcOffsetInSeconds = 7200;
  } else if (tz.equals(String("Europe/Athens"))) {
    utcOffsetInSeconds = 7200;
  } else if (tz.equals(String("Asia/Beirut"))) {
    utcOffsetInSeconds = 7200;
  } else if (tz.equals(String("Africa/Cairo"))) {
    utcOffsetInSeconds = 7200;
  } else if (tz.equals(String("Africa/Harare"))) {
    utcOffsetInSeconds = 7200;
  } else if (tz.equals(String("Europe/Helsinki"))) {
    utcOffsetInSeconds = 7200;
  } else if (tz.equals(String("Asia/Jerusalem"))) {
    utcOffsetInSeconds = 7200;
  } else if (tz.equals(String("Europe/Minsk"))) {
    utcOffsetInSeconds = 7200;
  } else if (tz.equals(String("Africa/Windhoek"))) {
    utcOffsetInSeconds = 7200;
  } else if (tz.equals(String("Asia/Kuwait"))) {
    utcOffsetInSeconds = 10800;
  } else if (tz.equals(String("Europe/Moscow"))) {
    utcOffsetInSeconds = 10800;
  } else if (tz.equals(String("Africa/Nairobi"))) {
    utcOffsetInSeconds = 10800;
  } else if (tz.equals(String("Asia/Tbilisi"))) {
    utcOffsetInSeconds = 10800;
  } else if (tz.equals(String("Asia/Tehran"))) {
    utcOffsetInSeconds = 10800;
  } else if (tz.equals(String("Asia/Muscat"))) {
    utcOffsetInSeconds = 14400;
  } else if (tz.equals(String("Asia/Baku"))) {
    utcOffsetInSeconds = 14400;
  } else if (tz.equals(String("Asia/Yerevan"))) {
    utcOffsetInSeconds = 14400;
  } else if (tz.equals(String("Asia/Kabul"))) {
    utcOffsetInSeconds = 16200;
  } else if (tz.equals(String("Asia/Yekaterinburg"))) {
    utcOffsetInSeconds = 18000;
  } else if (tz.equals(String("Asia/Karachi"))) {
    utcOffsetInSeconds = 18000;
  } else if (tz.equals(String("Asia/Calcutta"))) {
    utcOffsetInSeconds = 19800;
  } else if (tz.equals(String("Asia/Calcutta"))) {
    utcOffsetInSeconds = 19800;
  } else if (tz.equals(String("Asia/Katmandu"))) {
    utcOffsetInSeconds = 20700;
  } else if (tz.equals(String("Asia/Almaty"))) {
    utcOffsetInSeconds = 21600;
  } else if (tz.equals(String("Asia/Dhaka"))) {
    utcOffsetInSeconds = 21600;
  } else if (tz.equals(String("Asia/Rangoon"))) {
    utcOffsetInSeconds = 23400;
  } else if (tz.equals(String("Asia/Bangkok"))) {
    utcOffsetInSeconds = 25200;
  } else if (tz.equals(String("Asia/Krasnoyarsk"))) {
    utcOffsetInSeconds = 25200;
  } else if (tz.equals(String("Asia/Hong_Kong"))) {
    utcOffsetInSeconds = 28800;
  } else if (tz.equals(String("Asia/Kuala_Lumpur"))) {
    utcOffsetInSeconds = 28800;
  } else if (tz.equals(String("Asia/Irkutsk"))) {
    utcOffsetInSeconds = 28800;
  } else if (tz.equals(String("Australia/Perth"))) {
    utcOffsetInSeconds = 28800;
  } else if (tz.equals(String("Asia/Taipei"))) {
    utcOffsetInSeconds = 28800;
  } else if (tz.equals(String("Asia/Tokyo"))) {
    utcOffsetInSeconds = 32400;
  } else if (tz.equals(String("Asia/Seoul"))) {
    utcOffsetInSeconds = 28800;
  } else if (tz.equals(String("Asia/Yakutsk"))) {
    utcOffsetInSeconds = 28800;
  } else if (tz.equals(String("Australia/Adelaide"))) {
    utcOffsetInSeconds = 34200;
  } else if (tz.equals(String("Australia/Darwin"))) {
    utcOffsetInSeconds = 34200;
  } else if (tz.equals(String("Australia/Brisbane"))) {
    utcOffsetInSeconds = 36000;
  } else if (tz.equals(String("Australia/Canberra"))) {
    utcOffsetInSeconds = 36000;
  } else if (tz.equals(String("Australia/Hobart"))) {
    utcOffsetInSeconds = 36000;
  } else if (tz.equals(String("Pacific/Guam"))) {
    utcOffsetInSeconds = 36000;
  } else if (tz.equals(String("Asia/Vladivostok"))) {
    utcOffsetInSeconds = 36000;
  } else if (tz.equals(String("Asia/Magadan"))) {
    utcOffsetInSeconds = 39600;
  } else if (tz.equals(String("Pacific/Auckland"))) {
    utcOffsetInSeconds = 43200;
  } else if (tz.equals(String("Pacific/Fiji"))) {
    utcOffsetInSeconds = 43200;
  } else if (tz.equals(String("Pacific/Tongatapu"))) {
    utcOffsetInSeconds = 46800;
  }
  _datetime = new Datetime(utcOffsetInSeconds, false); //need this to calc dst
  int total = utcOffsetInSeconds;
  String str = String(total);
  writeFile(SPIFFS, "/offset.txt", str.c_str());

  if (isDaylightSavingsTime()) {
    Serial.print("IT IS DAYLIGHT SAVINGS TIME");
    int total = utcOffsetInSeconds + 3600;
    String str = String(total);
    writeFile(SPIFFS, "/offset.txt", str.c_str());
    delete _datetime;
    _datetime = new Datetime(utcOffsetInSeconds, true);
  }
  setCurrentTime();// go ahead and set currentTimeArr[5]
}

bool AtoAwcUtil::isDaylightSavingsTime() {
  curTimeArr = setCurrentTime();
  int yr = curTimeArr[0];
  //Serial.print(yr);
  int dayOfMonth = curTimeArr[2];
  //Serial.print(dayOfMonth);
  int hr = curTimeArr[3];
  //Serial.print(hr);
  int mo = curTimeArr[1];
  //Serial.print(mo);
  int DST = 0;

  // ********************* Calculate offset for Sunday *********************
  int y = yr;                          // DS3231 uses two digit year (required here)
  int x = (y + y / 4 + 2) % 7;    // remainder will identify which day of month
  // is Sunday by subtracting x from the one
  // or two week window.  First two weeks for March
  // and first week for November
  // *********** Test DST: BEGINS on 2nd Sunday of March @ 2:00 AM *********
  if (mo == 3 && dayOfMonth == (14 - x) && hr >= 2)
  {
    DST = 1;                           // Daylight Savings Time is TRUE (add one hour)
  }
  if ((mo == 3 && dayOfMonth > (14 - x)) || mo > 3)
  {
    DST = 1;
  }
  // ************* Test DST: ENDS on 1st Sunday of Nov @ 2:00 AM ************
  if (mo == 11 && dayOfMonth == (7 - x) && hr >= 2)
  {
    DST = 0;                            // daylight savings time is FALSE (Standard time)
  }
  if ((mo == 11 && dayOfMonth > (7 - x)) || mo > 11 || mo < 3)
  {
    DST = 0;
  }
  if (DST == 1)                       // Test DST and add one hour if = 1 (TRUE)
  {
    hr = hr + 1;
  }
  if (DST == 1) {
    return true;
  } else {
    return false;
  }
}


////////////////////////////////////////
//    bool checkStoreRun(String function, float elapse);

/////////////////////////////////////
bool AtoAwcUtil::checkStoreRun(String function, float elapse) {
  return true; //TODO write this fuction lost it
}

/////////////////////////////////////
//
////////////////////////////////////////
void AtoAwcUtil::webSerialWrite(String txt) {
  WebSerial.print(txt);
}

/////////////////////////////////////
//
////////////////////////////////////////
void AtoAwcUtil::webSerialWriteLine(String txt) {
  WebSerial.println(txt);
  
}

////////////////////////////
//
/////////////////////////////
void AtoAwcUtil::beginUtil() {
  WebSerial.begin(_server);
  //WebSerial.msgCallback(recvMsg);
}


/////////////////////////////////////////////
//
//  clearSpiffs
/////////////////////////////////////////
void AtoAwcUtil::clearSpiffs() {


  Serial.println("\n\n---BEFORE REMOVING---");
  listAllFiles();


  SPIFFS.remove("/9atoArr.txt");
  SPIFFS.remove("/awcdailyfill.txt");
  SPIFFS.remove("/9awcArr.txt");
  SPIFFS.remove("/bddailyfill.txt");
  SPIFFS.remove("/9blueArr.txt");
  SPIFFS.remove("/gddailyfill.txt");
  SPIFFS.remove("/9greenArr.txt");
  SPIFFS.remove("/yddailyfill.txt");
  SPIFFS.remove("/9yellowArr.txt");
  SPIFFS.remove("/pddailyfill.txt");
  SPIFFS.remove("/9purpleArr.txt");
  SPIFFS.remove("/atodailyfill.txt");
    SPIFFS.remove("/timezone.txt");
   delay(100);
    SPIFFS.remove("/leaknotfreq.txt");
   delay(100);
    SPIFFS.remove("/hwnotfreq.txt");
   delay(100);
    SPIFFS.remove("/lwnotfreq.txt");
   delay(100);
    SPIFFS.remove("/atofreq.txt");
   delay(100);
    SPIFFS.remove("/awcfreq.txt");
   delay(100);
    SPIFFS.remove("/hbnotfreq.txt");
   delay(100);
SPIFFS.remove("/bdnotfreq.txt");
   delay(100);
    SPIFFS.remove("/gdnotfreq.txt");
   delay(100);
    SPIFFS.remove("/ydnotfreq.txt");
   delay(100);
    SPIFFS.remove("/pdnotfreq.txt");
   delay(100);

    SPIFFS.remove("/leakNot.txt");
   delay(100);
    SPIFFS.remove("/hwNot.txt");
   delay(100);
    SPIFFS.remove("/lwNot.txt");
   delay(100);
    SPIFFS.remove("/atoNot.txt");
   delay(100);
    SPIFFS.remove("/awcNot.txt");
   delay(100);
    SPIFFS.remove("/hbNot.txt");
   delay(100);
    SPIFFS.remove("/bdnot.txt");
   delay(100);
    SPIFFS.remove("/gdnot.txt");
   delay(100);
    SPIFFS.remove("/ydnot.txt");
   delay(100);
    SPIFFS.remove("/pdnot.txt");
   delay(100);
     SPIFFS.remove("/atorunduration.txt");
   delay(100);
    SPIFFS.remove("/awcemptyduration.txt");
   delay(100);
    SPIFFS.remove("/awcfillduration.txt");
   delay(100);
 Serial.println("\n\n---AFTER REMOVING---");
  listAllFiles();

}

/////////////////////////////
//
//  listSiff
/////////////////////
void AtoAwcUtil::listAllFiles() {

  File root = SPIFFS.open("/");

  File file = root.openNextFile();

  while (file) {

    Serial.print("FILE: ");
    Serial.println(file.name());

    file = root.openNextFile();
  }
  
}



void AtoAwcUtil::setAtoRunning(bool val) {
  atoRunning = val;
}

bool AtoAwcUtil::getAtoRunning() {
  return atoRunning;
}

/*AsyncWebServer AtoAwcUtil::getServer(){
  return *_server;
  }*/

void AtoAwcUtil::setUtcOffsetInSeconds(int utcOffsetInSec) {
  utcOffsetInSeconds = utcOffsetInSec;
}

/*NTPClient*  AtoAwcUtil::getTc() {
  return _timeClient;
}*/

void AtoAwcUtil::setCalibrating(bool cal) {
  calibrating = cal;
}

int* AtoAwcUtil::getCurrentTimeArray() {
  return currentTimeArr;
}
