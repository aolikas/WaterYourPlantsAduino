
#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>
#include <OneWire.h>
#include <DallasTemperature.h>

// DS18B20
#define ONE_WIRE_BUS 4  //D2 pin of nodemcu
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensorDS(&oneWire);
float soilTempValue = 0;

// Soil Humidity
#define MOISTURE_PIN A0
int soilMoistureValue = 0;

// moisture sensor dry value
const int AirValue = 627;  //put your own value
// moisture sensor wet value
const int WaterValue = 292; //put your own value
int intervals = (AirValue - WaterValue) / 3;

// WiFi auth
#define WIFI_SSID ""   //your WiFi name
#define WIFI_PASSWORD ""  // your WiFi password

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
String sensorTemperarure = "/userSensorTemperature";


void setup() {
  Serial.begin(115200);
  sensorDS.begin();

   connectToWiFi();
   sentToRealTime();

  // sleep mode for 10 secs
  Serial.println("Going to sleep");
  ESP.deepSleep(10e6);
}

void loop() {}

void sentToRealTime() {
 
  //Firebase init
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);

  //DS sensor init
  sensorDS.requestTemperatures();
  soilTempValue = sensorDS.getTempCByIndex(0);
  Serial.print(soilTempValue);
  Serial.println(" C");

  //Soil sensor init
  soilMoistureValue = analogRead(MOISTURE_PIN);
  Serial.print(soilMoistureValue);
  Serial.println(" Moisture Value");

  String moistureCondition = getSoilSensorReadings(soilMoistureValue);
  Serial.print("Condition: ");
  Serial.println(moistureCondition);


    if (Firebase.setString(DB, nameDB + userId + subDB + sensorId + sensorMoisture, moistureCondition)) {
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

    if (Firebase.setFloat(DB, nameDB + userId + subDB + sensorId + sensorTemperarure, soilTempValue)) {
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
