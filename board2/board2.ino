// Code for Board # 2:
// Heat pad and altimeter

#include <Wire.h>
#include <IntersemaBaro.h>
Intersema::BaroPressure_MS5607B baro(true);


void setup() {
    Wire.begin();
    Serial.begin(9600);
    delay(500);
    Serial.println("Holdup...");
    delay(3000);
    baro.init();
    Serial.println("Running...");
}

void loop() {
    double alt = (double)baro.getHeightCentiMeters();
    Serial.println(alt);

}
