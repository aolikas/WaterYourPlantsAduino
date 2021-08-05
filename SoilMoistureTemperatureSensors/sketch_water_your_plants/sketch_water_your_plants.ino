#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>
#include <OneWire.h>
#include <DallasTemperature.h>

// DS18B20
#define ONE_WIRE_BUS 4  //D2 pin of nodemcu
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensorDS(&oneWire);
//var for Temperature value
float soilTempValue = 0;

// Soil Humidity
#define MOISTURE_PIN A0
//var for Moisture int value 
int soilMoistureValue = 0;
//var for Mousture condition
String soilMoistureCondition = "";

// moisture sensor dry value
const int AirValue = 644;  //put your own value
// moisture sensor wet value
const int WaterValue = 282; //put your own value
int intervals = (AirValue - WaterValue) / 3;

// WiFi auth
#define WIFI_SSID ""   //your Wifi name
#define WIFI_PASSWORD ""  // your password

// Firebase auth
#define FIREBASE_HOST ""
#define FIREBASE_AUTH "" //token_or_secret

FirebaseData DB;

//Firebase path
String nameDB = "/Users";
String userId = "/";    //in WaterYourPlants App in main menu/Account Details/User Id  
String subDB = "/userSensors";
String sensorId = "/";   // in WaterPlants App, create a sensor, click on sensor card
String sensorMoisture = "/userSensorMoistureCondition";
String sensorTemperarure = "/userSensorTemperature";
String sensorSleepModeTime = "/userSensorSleepModeTime";
String sensorAutoSleepMode = "/userSensorSleepModeAutomatic";

//vars
float sleepModeTime;
boolean isAutoSleepMode;


void setup() {
  
  Serial.begin(115200);
  sensorDS.begin();

  connectToWiFi();

  //Firebase init
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);
  
  readFromDB();
  
  sendToRealTime();

  sendToSleepMode();
}

void loop() {}

// read data from DB for AutoSleepMode and SleepMode Time
void readFromDB() {

  Serial.println("Starting to read from DB");

  String path = nameDB + userId + subDB + sensorId;

  if(Firebase.getBool(DB, path + sensorAutoSleepMode)) {
      Serial.println("PASSED");
      Serial.println("------------------------------------");
      Serial.println();
      isAutoSleepMode = DB.boolData();
      Serial.print("Auto Sleep Mode is: ");
      Serial.println(isAutoSleepMode);    
    } else {
      Serial.println("FAILED");
      Serial.println("REASON: " + DB.errorReason());
      Serial.println("------------------------------------");
      Serial.println();
    }

    if(Firebase.getFloat(DB, path + sensorSleepModeTime)) {
      Serial.println("PASSED");
      Serial.println("------------------------------------");
      Serial.println();
      sleepModeTime = DB.floatData();
      Serial.print("Sleep Mode Time is: ");
      Serial.println(sleepModeTime);    
    } else {
      Serial.println("FAILED");
      Serial.println("REASON: " + DB.errorReason());
      Serial.println("------------------------------------");
      Serial.println();
    }

  
}

void sendToRealTime() {

  Serial.println("Starting to send to DB");

   String path = nameDB + userId + subDB + sensorId;
 
  //DS sensor init
  sensorDS.requestTemperatures();
  soilTempValue = sensorDS.getTempCByIndex(0);
  Serial.print(soilTempValue);
  Serial.println(" C");

  //Soil sensor init
  soilMoistureValue = analogRead(MOISTURE_PIN);
  Serial.print("Moisture Value is: ");
  Serial.println(soilMoistureValue);
 
  soilMoistureCondition = getSoilSensorReadings(soilMoistureValue);
  Serial.print("Moisture Condition is: ");
  Serial.println(soilMoistureCondition);


  if (Firebase.setString(DB, path + sensorMoisture, soilMoistureCondition)) {
      Serial.println("PASSED");
      Serial.println("------------------------------------");
      Serial.println();
    }
    else
    {
      Serial.println("FAILED");
      Serial.println("REASON: " + DB.errorReason());
      Serial.println("------------------------------------");
      Serial.println();
    }

    if (Firebase.setFloat(DB, path + sensorTemperarure, soilTempValue)) {
      Serial.println("PASSED");
      Serial.println("------------------------------------");
      Serial.println();
    }
    else
    {
      Serial.println("FAILED");
      Serial.println("REASON: " + DB.errorReason());
      Serial.println("------------------------------------");
      Serial.println();
    }  
}

void connectToWiFi() {

  //set WiFi to station mode and disconnect from AP if 
  // it was previously connected
   WiFi.mode(WIFI_STA);
   WiFi.disconnect();
   delay(100);

   // attempt to connect to WIFI network
   Serial.print("Connected WiFi: ");
   Serial.println(WIFI_SSID);
   WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
   while(WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
   }
   Serial.println("");
   Serial.println("WiFi connected");
   Serial.println("IP Address: ");
   // Print ESP32 Local IP Address
  Serial.println(WiFi.localIP()); 
 }

 String getSoilSensorReadings(int sensorData) {

 String condition = "";

  if(sensorData > WaterValue && 
  sensorData < (WaterValue + intervals)) {
    condition = "Very Wet";
  } else if(sensorData > (WaterValue + intervals) &&
  sensorData < (AirValue - intervals)) {
    condition = "Wet"; 
  } else if(sensorData < AirValue && 
  sensorData > (AirValue - intervals)) {
    condition = "Dry";
  }

  return condition;
}

void sendToSleepMode() {

   if(isAutoSleepMode == true) {
      if(soilMoistureCondition.equals("Dry") || soilTempValue > 36.0) {
      Serial.println("AutoSleep Mode going to sleep for 20 sec");
      ESP.deepSleep(20e6);
      } else {
      //ESP.deepSleep(ESP.deepSleepMax())
      Serial.println("AutoSleep Mode going to sleep for 1 hour");
      ESP.deepSleep(3600e6);
      }
   } else {
     int sleepTime = (int) sleepModeTime;
   
      int sleepSec = sleepTime * 60000000; 

     Serial.print("Sleep Mode going to sleep for ");
     Serial.print(sleepTime);
     Serial.println(" min");
     ESP.deepSleep(sleepSec);
  }
}
