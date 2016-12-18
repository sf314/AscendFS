// THIS IS THE ONE!
// 11/15/16

// Writing out everything
// Printing directly from read


#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <IntersemaBaro.h>

File dataFile;
int sdPin = 4;
int baroPin = 2;
int heatPin = 3;
int logNumber = 1;

Intersema::BaroPressure_MS5607B baro(true);


// ***** Keep track of time!
unsigned long previousTime = 0;
long interval = 1000; // in ms

void setup() {

    pinMode(10, OUTPUT);
    pinMode(4, OUTPUT);
    pinMode(baroPin, OUTPUT);
    pinMode(heatPin, OUTPUT);

    Serial.begin(9600);
    delay(3000);
    Serial.println("Starting setup");
    Wire.begin();
    SPI.begin();


    digitalWrite(sdPin, LOW);
    digitalWrite(baroPin, HIGH);
    delay(50);
    baro.init();
    delay(50);
    digitalWrite(baroPin, LOW);
    digitalWrite(sdPin, HIGH);
    delay(50);


    Serial.println("Holdup");
    delay(3000);




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
        dataFile.println("Col1, Col2, Col3, Col4, Col5");
        dataFile.close();
    } else {
        Serial.println("setup(): dataFile is false");
    }

    Serial.println("Beginning loop");
}


long alt = 0;

void loop() {
    // Check if interval of time has passed
    unsigned long currentTime = millis();

    if (currentTime - previousTime >= interval) { // If it's time, take data
        // Reset time
        previousTime = currentTime;


        dataFile = SD.open("sdt7.csv", FILE_WRITE); // ***** Open file, with options
        dataFile.print(logNumber);
        dataFile.print(",");

        dataFile.print(readTemp(0));
        dataFile.print(",");

        dataFile.print(readTemp(1));
        dataFile.print(",");

        dataFile.print(readTemp(2));
        dataFile.print(",");

        Serial.println(alt);
        dataFile.println(alt);

        dataFile.close();
        logNumber++;
        //analogWrite(heatPin, 255);


        digitalWrite(sdPin, LOW);
        digitalWrite(baroPin, HIGH);
        delay(50);

        alt = (double)baro.getHeightCentiMeters();

        delay(50);
        digitalWrite(baroPin, LOW);
        digitalWrite(sdPin, HIGH);
        delay(50);
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
