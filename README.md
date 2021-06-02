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
   4. fill with your values Firebase Host and Auth. More detailed [here.](https://create.arduino.cc/projecthub/pulasthi-nanayakkara/connecting-esp8266-to-firebase-to-send-receive-data-4adf66)
   5. 
  
