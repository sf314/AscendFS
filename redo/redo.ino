// THIS IS THE ONE!
// 11/15/16


#include <Wire.h>
#include <SPI.h>
#include <SD.h>

File dataFile;
int sdPin = 10;

int logNumber = 1;

// ***** Keep track of time!
unsigned long previousTime = 0;
long interval = 1000; // in ms

void setup() {
    Serial.begin(9600);
    Wire.begin();
    SPI.begin();

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
    if (usingSD) {Serial.println("Using SD!!!!!");}

    // **** Create file headers
    dataFile = SD.open("sdt.csv", FILE_WRITE); // later, just call with filename
    if (dataFile) {
        Serial.println("File opened successfully");
        dataFile.println("Col1, Col2, Col3, Col4, Col5");
        dataFile.close();
    }
}



void loop() {
    // Check if interval of time has passed
    unsigned long currentTime = millis();

    if (currentTime - previousTime >= interval) { // If it's time, take data
        // Reset time
        previousTime = currentTime;



        dataFile = SD.open("sdt.csv", FILE_WRITE); // ***** Open file, with options
        dataFile.println(logNumber);
        dataFile.print(",");

        dataFile.print(readTemp(0));
        dataFile.print(",");

        dataFile.print(readTemp(1));
        dataFile.close();
        logNumber++;
    }

}




// Read temperature
double readTemp(int tempPin) {
    double voltage = 5000
    double pinMillivolts = analogRead(tempPin) * ((double)voltage / 1024);
    double celsius = (pinMillivolts - 500) / 10; // in celsius

    // perform other calculations, or switch on a thing to use Fahrenheit
    //double fahrenheit =  celsius * 1.8 + 32;
    return celsius;
}
