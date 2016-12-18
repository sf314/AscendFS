
// Alitmeter on I2C address 0x76

#include <Wire.h>
#include <IntersemaBaro.h>


Intersema::BaroPressure_MS5607B baro(true);

void setup() {
    pinMode(10, OUTPUT);
    pinMode(2, OUTPUT);
    // Chipselect sd 4?
    digitalWrite(10, HIGH);

    // Chipselect baro on 5?

    Serial.begin(9600);
    baro.init();
    delay(500);
}

void loop() {
    long alt = baro.getHeightCentiMeters();
    Serial.println(alt);
    delay(1000);
}
