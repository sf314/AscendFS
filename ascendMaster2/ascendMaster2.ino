// Retry of master code, but with SD built in (rather than external library)

// **** Inclusions test
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <IntersemaBaro.h>
#include <AscendLib.h>

// **** Var setup
Intersema::BaroPressure_MS5607B baro(true); // Basic setup from lib
ALHeater heatPad;
ALTemp temp1;
ALTemp temp2;
ALTemp temp3;
int logNumber = 1;

// Data logging! (try formatting SD card to FAT16)
File dataFile;
String fileName = "thing.csv";
int sdPin = 10;

void setup() {
    Serial.begin(9600);
    Wire.begin();
    SPI.begin();

    baro.init();

    heatPad.setPin(3);

    temp1.setPin(0);
    temp1.setPin(1);
    temp1.setPin(2);

    // ***** Data logging setup
    pinMode(sdPin, OUTPUT);

    int tries = 5;
    bool usingSD = false;
    while (tries > 1) {
        if (SD.begin()) {
            tries = -1;
            Serial.println("Attempting to use SD...");
            bool usingSD = true;
            break;
        }
        tries--;
        delay(1000);
    } // Should be set up now... if not, then problems.
    if (usingSD) {Serial.println("Using SD!!!!!");}

    // **** Create file headers
    dataFile = SD.open(fileName, FILE_WRITE); // later, just call with filename
    if (dataFile) {
        Serial.println("File opened successfully");
        dataFile.println("Col1, Col2, Col3, Col4, Col5");
        dataFile.close();
    }
}
// ***** Keep track of time!
unsigned long previousTime = 0;
long interval = 1000; // in ms

void loop() {

    // Check if interval of time has passed
    unsigned long currentTime = millis();

    if (currentTime - previousTime >= interval) { // If it's time, take data
        // Reset time
        previousTime = currentTime;

        Serial.println(String(logNumber));
        dataFile = SD.open(fileName, FILE_WRITE); // ***** Open file with options!

        // Altitude
        double altCM = (double)baro.getHeightCentiMeters();
        Serial.println("Alt:");
        Serial.println(altCM);

        // Temperature
        double t1 = temp1.read();
        double t2 = temp2.read();
        double t3 = temp3.read();
        Serial.println("Temps:");
        Serial.println(t1);
        Serial.println(t2);
        Serial.println(t3);

        // Heater
        heatPad.setHeat(255);

        //dataFile.println(logNumber + ", " + altCM + ", " + t1 + ", " + t2 + ", " + t3);
        String dataString = String(logNumber) + "," + String(altCM) + "," + String(t1) + "," + String(t2) + "," + String(t3);

        dataFile.println(dataString);
        // Hey there!

        dataFile.close(); // Close file
        logNumber++;
        Serial.print("Hello ");

    }



}
