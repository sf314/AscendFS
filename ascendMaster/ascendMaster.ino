// ascendMaster.ino
/* Multiple sensor integration:
 *  Heating pad (ALHeater)
 *  TMP36 (x3) (ALTemp)
 *  Barometer/altimeter (not yet AL-ified)
 *  SD card (ALData)
 *  
*/

// ***** Libraries
#include <Wire.h>
#include <SPI.h> // required
#include <SD.h> // required
#include <SDWrite.h>
#include "IntersemaBaro.h"
#include "AscendLib.h" // certainly required

// ***** Objects
SDWrite sd;
ALHeater heatPad;
ALTemp temp;
Intersema::BaroPressure_MS5607B baro(true); // Basic setup from lib

// ***** Global Vars
int logNumber = 1;

void setup() {
  sd.changeSDPin(10);
  Serial.begin(9600);
  Wire.begin();
  SPI.begin();
  
  sd.initSD();
  sd.setupFile("otherFile.csv", "1_Alt, 2_Temp");
  Serial.println("\tME: Setup complete");
  
  baro.init();
  Serial.println("\tME: baro.init() complete!");

  heatPad.setPin(3);
  Serial.println("\tME: setPin(3) done!");
}


void loop() {
  sd.startNewLog();
  sd.writeInt(logNumber);
  Serial.println(logNumber);

  // Do altimeter stuff within for loops of heatPad
//
//  Serial.println("Cooling");
//  heatPad.setHeat(0);
//
//  for (int i = 0; i <5; i++) {
//    Serial.print(i);
//    Serial.print(" ");
//    delay(1000);
//  }
//  Serial.println();






  Serial.println("Heating");
  
  heatPad.setHeat(255);
//  for (int i = 0; i <20; i++) {
//    Serial.print(i);
//    Serial.print(" ");
//    delay(1000);
    
    // Sense alt and save!
    double altM = (double)baro.getHeightCentiMeters(); // in metres
    sd.writeDouble(altM);
    Serial.println(altM);
    
    // Sense temp and save!
    double t = temp.read();
    sd.writeDouble(t);
    Serial.println(t);
//  }
  Serial.println();

  logNumber++;
  //sd.newLine();
  sd.close();
  
  
  delay(1000);
}

