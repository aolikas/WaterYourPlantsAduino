# WaterYourPlantsAduino
This folder contains the Arduino source code and schemas for the Water Your Plants App.

# Introduction
These Arduino sketches are a part of the IoT project - Water Your Plants. This project consists of three parts:
1. Water your Plants - an Android App based on Realtime Firebase that can receive sensor signals and control to a level of watering.  
2. SoilMoistureTemperatureSensors - Arduino sketch which sends data from Soil Moisture and Temperature Sensor to the Realtime Firebase DB.
3. WaterPumpSensors - Arduino sketch, which is controlled by Android App. 

# Get Started
1. Before your start, be sure that you already have Water Your Plants App on your phone. If no, [check this.](https://github.com/aolikas/WaterYourPlants)
2. Install Arduino IDE, if you don't already have it.
3. Download the sample.
4. Install ESP8266 Add-on in Arduino IDE. [This is an example.](https://randomnerdtutorials.com/how-to-install-esp8266-board-arduino-ide/)

# SoilMoistureTemperatureSensors
  ### Hardware:
  1. Capacitive moisture sensor. It's better to calibrate it before use. For this purpose, you can use this [sketch.](https://github.com/aolikas/WaterYourPlantsAduino/blob/main/SoilMoistureTemperatureSensors/sketch_moisture_sensor_calibration.ino) 
  Remember two values: sensor value in dry condition and sensor value in wet condition (put the sensor in a glass of water).
  2. NodeMcu esp8266 
  3. Optional DS18b20 with pluggable terminal.
  4. Power supply module.
  5. The sensors circuit is [here.](https://github.com/aolikas/WaterYourPlantsAduino/blob/main/SoilMoistureTemperatureSensors/sensors_circuit.jpg)
  ### Software:
  1. In Arduino IDE download further libraries:
      - ESP8266WiFi.h
      - FirebaseESP8266.h
      - OneWire.h
      - DallasTemperature.h
   2. In a sketch fill int AirValue and int WaterValue with your values from calibration.  
   3. Fill with your values Wifi auth.
   4. Fill with your values Firebase Host and Auth. More detailed [here.](https://create.arduino.cc/projecthub/pulasthi-nanayakkara/connecting-esp8266-to-firebase-to-send-receive-data-4adf66)
   5. Next fill userId. In your WaterYourPlants App, in a menu click on Account Details. There you can find user id.
      <p align="left">
      <img width="300" height="200" src="https://github.com/aolikas/WaterYourPlantsAduino/blob/main/Screenshot/Screenshot_user_id.png">
      </p>
   
   6. Next fill sensorId. In your WaterYourPlants App create a Sensor, then click on a Sensor card.
       <p align="left">
      <img width="300" height="200" src="https://github.com/aolikas/WaterYourPlantsAduino/blob/main/Screenshot/Screenshot_sensor_id.png">
      </p>
    
   7. NodeMcu will be in sleep mode every 10 sec. Choose your own time and rewrite ESp.deepSleep function.  
   8. Compile the sketch, be sure that you don't have mistakes.
   9. In Arduino IDE Tools, click Board and choose NodeMCU in ESP8266 Board.
   10. Connect NodeMcu via USB, in Tools choose Port.
   11. Before upload a sketch, ensure that pins D0 and RST are not connected to each other. 
   12. Upload a sketch. Check all logs in Serial Monitor - Connecting to WiFi, Firebase, and transferred data.
   13. After that connect D0 and RST.
  
