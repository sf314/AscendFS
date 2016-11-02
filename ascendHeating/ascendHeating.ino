
#include <AscendLib.h>

// Test latest components!

ALHeater heatPad;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  heatPad.setPin(3);
}

void loop() {
  // put your main code here, to run repeatedly:
  
  Serial.println("Cooling");
  heatPad.setHeat(0);
  
  for (int i = 0; i <5; i++) {
    Serial.print(i);
    Serial.print(" ");
    delay(1000);
  }
  Serial.println();



  
  Serial.println("Heating");
  
  heatPad.setHeat(255);
  for (int i = 0; i <20; i++) {
    Serial.print(i);
    Serial.print(" ");
    delay(1000);
  }
  Serial.println();
}
