// Integrating barometer and SD functionality
// NOTES: 11/8/16

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
ALTemp temp1;
ALTemp temp2;
ALTemp temp3;
int logNumber = 1;

void setup() {
  // put your setup code here, to run once:
  sd.changeSDPin(10);
  Wire.begin();
  SPI.begin();
  Serial.begin(9600);
  sd.initSD();
  //char thing[20] = "dataFile.csv";
  sd.setupFile("dataFile.csv", "1_Altitude, 2_Temp1, 3_Temp2, 4_Temp3");
  baro.init();
  heatPad.setPin(3);

  temp1.setPin(0);
  temp2.setPin(1);
  temp3.setPin(2);
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
  sd.writeDouble(temp1.read());
  sd.writeDouble(temp2.read());
  sd.writeDouble(temp3.read());


  logNumber++;
  sd.close();
  delay(1000);
}
