// Integrating barometer and SD functionality

// Libraries
#include <Wire.h>
#include <SPI.h> // Required!
#include <SD.h> // Required!
#include <SDWrite.h>
#include "IntersemaBaro.h"

// Setup
SDWrite sd;

Intersema::BaroPressure_MS5607B baro(true); // Basic setup from lib
int logNumber = 1;

void setup() {
  // put your setup code here, to run once:
  sd.changeSDPin(10);
  Wire.begin();
  SPI.begin();
  Serial.begin(9600);
  sd.initSD();
  sd.setupFile("dataFile.csv", "1_Altitude");
  baro.init();
}

void loop() {
  // put your main code here, to run repeatedly:
  sd.startNewLog();
  Serial.println(logNumber);
  
  // Get altitude
  double altM = (double)baro.getHeightCentiMeters(); // in metres
  Serial.println(altM);
  
  // Write to 
  sd.writeInt(logNumber);
  sd.writeDouble(altM);
  
  logNumber++;
  sd.close();
  delay(1000);
}
