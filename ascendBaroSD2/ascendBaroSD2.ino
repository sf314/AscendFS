// Integrating barometer and SD functionality

// Libraries
#include <Wire.h>
#include <SPI.h> // Required!
#include <SD.h> // Required!
#include <SDWrite.h>
#include "IntersemaBaro.h"
#include "AscendLib.h"

// Setup
SDWrite sd;

Intersema::BaroPressure_MS5607B baro(true); // Basic setup from lib
ALHeater heatPad;
ALTemp temp;
int logNumber = 1;

void setup() {
  // put your setup code here, to run once:
  sd.changeSDPin(10);
  Wire.begin();
  SPI.begin();
  Serial.begin(9600);
  sd.initSD();
  sd.setupFile("dataFile.csv", "1_Altitude, 2_Temp");
  baro.init();
  heatPad.setPin(3);
}


void loop() {
  // put your main code here, to run repeatedly:
  sd.startNewLog();
  sd.writeInt(logNumber);
  Serial.println(logNumber);
  
  // Get altitude
  double altM = (double)baro.getHeightCentiMeters(); // in metres
  Serial.println(altM);

  // HEATING
  heatPad.setHeat(255);

  // Write to 
  sd.writeDouble(altM);
  
  // TEMP
  sd.writeDouble(temp.read());
 
  
  logNumber++;
  sd.close();
  delay(1000);
}
