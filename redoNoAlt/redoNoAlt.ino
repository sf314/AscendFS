// THIS IS THE ONE!
// 11/15/16

// Redo, but without intersema stuff :(


#include <Wire.h>
#include <SPI.h>
#include <SD.h>

File dataFile;
int heatPin = 3;
int logNumber = 1;

// ***** Keep track of time!
unsigned long previousTime = 0;
long interval = 1000; // in ms

void setup() {
    Serial.begin(9600);
    Wire.begin();
    SPI.begin();
    Serial.println("Starting setup");

    Serial.println("Holdup");
    delay(3000);

    pinMode(10, OUTPUT);
    pinMode(heatPin, OUTPUT);

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
    if (usingSD) {
        Serial.println("Using SD");
    } else {
        Serial.println("Not using SD");
    }

    // **** Create file headers
    dataFile = SD.open("sdt7.csv", FILE_WRITE); // later, just call with filename
    if (dataFile) {
        Serial.println("File opened successfully");
        dataFile.println("Log, Time, T0, T1, T2");
        dataFile.close();
    } else {
        Serial.println("setup(): dataFile is false");
    }

    Serial.println("Beginning loop");
}



void loop() {
    // Check if interval of time has passed
    unsigned long currentTime = millis();

    if (currentTime - previousTime >= interval) { // If it's time, take data
        // Reset time
        previousTime = currentTime;

        dataFile = SD.open("sdt7.csv", FILE_WRITE); // ***** Open file, with options
        dataFile.print(logNumber);
        dataFile.print(",");

        dataFile.print(currentTime);
        dataFile.print(",");

        dataFile.print(readTemp(0));
        dataFile.print(",");

        dataFile.print(readTemp(1));
        dataFile.print(",");

        dataFile.println(readTemp(2));


        dataFile.close();
        logNumber++;
        analogWrite(heatPin, 125); // 50 out of 255
    }

}




// Read temperature
double readTemp(int tempPin) {
    // IF 5V, use 5000.0
    // If 3.3V, use 3300.0

    double pinMillivolts = analogRead(tempPin) * (5000.0 / 1024.0);
    double celsius = (pinMillivolts - 500.0) / 10.0; // in celsius

    // perform other calculations, or switch on a thing to use Fahrenheit
    //double fahrenheit =  celsius * 1.8 + 32;
    return celsius;
}
