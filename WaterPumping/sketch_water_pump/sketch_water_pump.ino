
#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>

// WiFi auth
#define WIFI_SSID "" //your WiFi name
#define WIFI_PASSWORD "" // your WiFi password

// Firebase auth
#define FIREBASE_HOST "" //example.firebaseio.com
#define FIREBASE_AUTH "" //token_or_secret
FirebaseData DB;

String nameDB = "/Users";
//in WaterYourPlants App in main menu/Account Details/User Id 
String userId = "/";
String subDB = "/userSensors";
// in WaterPlants App, create a sensor, click on sensor card
String sensorId = "/";
String sensorMoisture = "/userSensorMoistureCondition";
String sensorWateringDuration = "/userSensorPumpWateringDuration";
String sensorAutomaticWatering = "/userSensorPumpWateringAutomatic";
String sensorWatering = "/userSensorPumpWatering";
String sensorSleepModeTime = "/userSensorSleepModeTime";
String sensorAutoSleepMode = "/userSensorSleepModeAutomatic";

String moistureCondition = "";
float wateringDuration;
float sleepModeTime;
boolean isWateringAutomatic;
boolean isWatering;
boolean isAutoSleepMode;


void setup() {
  
  Serial.begin(115200);
  pinMode(D5, OUTPUT);
  digitalWrite(D5, HIGH);
  Serial.println("High");
 
  connectToWiFi();
  
  readFromDB();

  watering();

  sendToSleepMode();
 
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

 void readFromDB() {
  
  //Firebase init
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);

  String path = nameDB + userId + subDB + sensorId;

  if(Firebase.getBool(DB, path + sensorAutomaticWatering)) {
      Serial.println("PASSED");
      Serial.println("------------------------------------");
      Serial.println();
      isWateringAutomatic = DB.boolData();
      Serial.print("Automatic watering: ");
      Serial.println(isWateringAutomatic);    
    } else {
      Serial.println("FAILED");
      Serial.println("REASON: " + DB.errorReason());
      Serial.println("------------------------------------");
      Serial.println();
    }

    if(Firebase.getBool(DB, path + sensorWatering)) {
      Serial.println("PASSED");
      Serial.println("------------------------------------");
      Serial.println();
      isWatering = DB.boolData();
      Serial.print("Automatic watering: ");
      Serial.println(isWatering);    
    } else {
      Serial.println("FAILED");
      Serial.println("REASON: " + DB.errorReason());
      Serial.println("------------------------------------");
      Serial.println();
    }


    if(Firebase.getFloat(DB, path + sensorWateringDuration)) {
      Serial.println("PASSED");
      Serial.println("------------------------------------");
      Serial.println();
      wateringDuration = DB.floatData();
      Serial.print("Watering duration: ");
      Serial.println(wateringDuration);    
    } else {
      Serial.println("FAILED");
      Serial.println("REASON: " + DB.errorReason());
      Serial.println("------------------------------------");
      Serial.println();
    }
  
     if(Firebase.getString(DB, path + sensorMoisture)) {
      Serial.println("PASSED");
      Serial.println("------------------------------------");
      Serial.println();
      moistureCondition = DB.stringData();
      Serial.print("Condition: ");
      Serial.println(moistureCondition);    
    } else {
      Serial.println("FAILED");
      Serial.println("REASON: " + DB.errorReason());
      Serial.println("------------------------------------");
      Serial.println();
    }

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

 void watering() {
  int duration = (int) wateringDuration;
  if(isWateringAutomatic == true) {
    pumpingAutomatic(moistureCondition, duration);
  } else {
    pumping(duration);
  }
 }
 
 
 void pumpingAutomatic(String condition, int duration) {
  Serial.print("Pumping condition ");
  Serial.println(condition);
  if(condition.equals("Dry")){
    Serial.println("Its Dry");
    
      digitalWrite(D5, LOW);
       delay(duration * 1000);
    }
 }

 void pumping(int duration) {
  if(isWatering == true) {
     Serial.print("Watering duration ");
      Serial.println(duration);

      digitalWrite(D5, LOW);
      delay(duration * 1000);
      isWatering = false;
      sendToDB(isWatering);
  }
  
}

void sendToDB(boolean condition) {
  //Firebase init
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);

  String path = nameDB + userId + subDB + sensorId;

  if (Firebase.setBool(DB, path + sensorWatering, condition)) {
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

void sendToSleepMode() {

   if(isAutoSleepMode == true) {
      if(moistureCondition.equals("Dry")) {
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

void loop() {}
