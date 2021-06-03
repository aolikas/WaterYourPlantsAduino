
#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>

// WiFi auth
#define WIFI_SSID ""
#define WIFI_PASSWORD ""

// Firebase auth
#define FIREBASE_HOST ""
#define FIREBASE_AUTH ""
FirebaseData DB;

String nameDB = "/Users";
String userId = "/";
String subDB = "/userSensors";
String sensorId = "/";
String sensorMoisture = "/userSensorMoistureCondition";
String sensorWateringDuration = "/userSensorPumpWateringDuration";
String sensorAutomaticWatering = "/userSensorPumpWateringAutomatic";

String moistureCondition = "";
float wateringDuration;
boolean wateringAutomatic;


void setup() {
  
  Serial.begin(115200);
  pinMode(D5, OUTPUT);
  digitalWrite(D5, HIGH);
  Serial.println("High");
 
  connectToWiFi();
  
  readFromDB();

  watering();

 

  // sleep mode for 40 secs
  Serial.println("Going to sleep");
  ESP.deepSleep(40e6);
 
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
      wateringAutomatic = DB.boolData();
      Serial.print("Automatic watering: ");
      Serial.println(wateringAutomatic);    
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
 }

 void watering() {
  if(wateringAutomatic == true) {
    pumpingAutomatic(moistureCondition);
  } else {
    pumpingDuration();
  }
 }
 
 
 void pumpingAutomatic(String condition) {
  Serial.print("Pumping condition ");
  Serial.println(condition);
  if(condition.equals("Dry")){
    Serial.println("Its Dry");
    
      digitalWrite(D5, LOW);
      delay(10000);
    }
 }

 void pumpingDuration() {
  int duration = (int) wateringDuration;

  Serial.print("Watering duration ");
  Serial.println(duration);

  digitalWrite(D5, LOW);
  delay(duration * 1000);
}

void loop() {}
