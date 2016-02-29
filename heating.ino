#include <Bridge.h>
#include <stdio.h>
#include <FileIO.h>

char D13value[2];
 
void setup() {
  // Zero out the memory we're using for the Bridge.
  memset(D13value, 0, 2);
  
  pinMode(13, OUTPUT);  
  
  Bridge.begin();
  Serial.begin(9600);

  while(!Serial);  
  Serial.println("Filesystem datalogger\n");
  
  randomSeed(analogRead(0));
}
 
void loop() {
  Bridge.get("D13", D13value, 2);
  int D13int = atoi(D13value);
  String valueStr(D13value);
  digitalWrite(13, D13int);

  String dataString;
  dataString += getTimeStamp();
  dataString += ",";
  dataString += String(random(15,25));
  dataString += ",";
  dataString += valueStr;
  
  Bridge.put("CHECK", dataString);

  //TODO guardar log cada hora sólo
  File dataFile = FileSystem.open("/mnt/sda1/heating-logs/datalog.log", FILE_APPEND);
  if (dataFile) {
    dataFile.println(dataString);
    dataFile.close();
    Serial.println(dataString);
  }
  
  // An arbitrary amount of delay to make the whole thing more reliable. YMMV
  delay(1000);  
}

String getTimeStamp() {
  String result;
  Process time;
  time.begin("date");
  time.addParameter("+%D-%T");  
  time.run(); 

  while(time.available()>0) {
    char c = time.read();
    if(c != '\n')
      result += c;
  }

  return result;
}
