// THIS IS THE ONE!
// 11/15/16


#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <AscendLib.h>
#include <IntersemaBaro.h>

File dataFile;
int sdPin = 10;
ALTemp temp1;
ALTemp temp2;
ALTemp temp3;
Intersema::BaroPressure_MS5607B baro(true); // Basic setup from lib

int logNumber = 1;

// ***** Keep track of time!
unsigned long previousTime = 0;
long interval = 1000; // in ms

void setup() {
    Serial.begin(9600);
    Wire.begin();
    SPI.begin();

    baro.init();
    temp1.setPin(0);
    temp2.setPin(1);
    temp3.setPin(2);

    Serial.println("Hold up...");
    delay(3000);

    pinMode(sdPin, OUTPUT);

    int tries = 5;
    bool usingSD = false;
    while (tries > 1) {
        if (SD.begin()) {
            tries = -1;
            Serial.println("Attempting to use SD...");
            usingSD = true;
        }
        tries--;
        delay(1000);
    } // Should be set up now... if not, then problems.
    delay(500);
    if (usingSD) {
        Serial.println("Using SD");
    } else {
        Serial.println("void setup(): sd check failed!");
    }

    // **** Create file headers
    dataFile = SD.open(F("test3.csv"), FILE_WRITE); // later, just call with filename
    if (dataFile) {
        Serial.println("File opened successfully");
        dataFile.println("Col1, Col2, Col3, Col4, Col5");
        dataFile.close();
    } else {
        Serial.println("void setup(): datFile returned false!");
        delay(1000);
        dataFile.close();
    }
}



void loop() {
    // Check if interval of time has passed
    unsigned long currentTime = millis();

    if (currentTime - previousTime >= interval) { // If it's time, take data
        // Reset time
        previousTime = currentTime;

        // Do stuff
        Serial.print("Looping test3.csv: ");

        Serial.println(logNumber);

        dataFile = SD.open(F("test3.csv"), FILE_WRITE); // ***** Open file, with da options

        if (dataFile) {
            Serial.println("loop(): Accessing card...");
        } else {
            Serial.println("loop(): dataFile returned false");
        }

        double data;

        dataFile.print(logNumber);
        dataFile.print(",");

        data = temp1.read();
        dataFile.print(data);
        Serial.println(data);
        dataFile.print(",");

        data = temp2.read();
        dataFile.print(data);
        Serial.println(data);
        dataFile.print(",");

        data = temp3.read();
        dataFile.print(data);
        Serial.println(data);
        dataFile.print(",");

        data = baro.getHeightCentiMeters();
        dataFile.print(data);
        Serial.println(data);
        dataFile.print(",");

        dataFile.println();
        dataFile.close();


        logNumber++;
    } else {
        Serial.println("Potatoes!");
    }
}

//3628ft bottom outside

//3683ft 6th floor elevator

// diff: 55ft
