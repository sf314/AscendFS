#include <Wire.h>
#include <DHT.h>
#include <SPI.h>
#include <SD.h>

#define DHTPIN 2     // what pin we're connected to

// Uncomment whatever type you're using!
//#define DHTTYPE DHT11   // DHT 11
#define DHTTYPE DHT22   // DHT 22  (AM2302)
//#define DHTTYPE DHT21   // DHT 21 (AM2301)

// Connect pin 1 (on the left) of the sensor to +5V
// Connect pin 2 of the sensor to whatever your DHTPIN is
// Connect pin 4 (on the right) of the sensor to GROUND
// Connect a 10K resistor from pin 2 (data) to pin 1 (power) of the sensor

DHT dht(DHTPIN, DHTTYPE);

#define ADDRESS 0x76


uint32_t D1 = 0;
uint32_t D2 = 0;
int64_t dT = 0;
int32_t TEMP = 0;
int64_t OFF = 0;
int64_t SENS = 0;
int32_t P = 0;
uint16_t C[7];
const int chipSelect = 4; // not 10?

float Temperature;
float Pressure;
#define visualPin 6
int ledState = LOW;             // ledState used to set the LED
long previousMillis = 0;
long interval = 10000;
long intervalShort = 150;



void setup() {
// Disable internal pullups, 10Kohms are on the breakout
PORTC |= (1 << 4);
PORTC |= (1 << 5);

 Wire.begin();
 Serial.begin(9600); //9600 changed 'cos of timing?
 delay(100);
  Serial.println("DHT22 with SD Logging!");
  pinMode(10, OUTPUT);
  if (!SD.begin(chipSelect)) {
  Serial.println("Card failed, or not present");
  return;
  }
    Serial.println("card initialized.");

  delay(2000);
  }

void loop()
{
   unsigned long currentMillis = millis();
   File dataFile = SD.open("datalog.csv", FILE_WRITE);
  // Wait a few seconds between measurements.
  delay(4850);

 D1 = getVal(ADDRESS, 0x48); // Pressure raw
 D2 = getVal(ADDRESS, 0x58);// Temperature raw

 dT   = D2 - (uint32_t)C[5] *pow(2,8);
 OFF  = (int64_t)C[2] * pow(2,17) + ((dT * C[4])/pow(2,6));
 SENS = (int32_t)C[1] *pow(2,16) + ((dT * C[3])/pow(2,7));

 TEMP = ((int64_t)dT * (int64_t)C[6] / 8388608) + 2000;

 if(TEMP < 2000) // if temperature lower than 20 Celsius
 {
   int32_t T1    = 0;
   int64_t OFF1  = 0;
   int64_t SENS1 = 0;

   T1    = pow(dT, 2) / 2147483648;
   OFF1  = 61 * pow((TEMP - 2000), 2) / 16;
   SENS1 = 2 * pow((TEMP - 2000), 2);

   if(TEMP < -1500) // if temperature lower than -15 Celsius
   {
     OFF1  = OFF1 + 15 * pow((TEMP + 1500), 2);
     SENS1 = SENS1 + 8 * pow((TEMP + 1500), 2);
   }

   TEMP -= T1;
   OFF -= OFF1;
   SENS -= SENS1;
 }


 Temperature = (float)TEMP / 100;

 P  = ((int64_t)D1 * SENS / 2097152 - OFF) / 32768;

 Pressure = (float)P / 100;

 Serial.print("     Actual TEMP= ");
 Serial.print(Temperature);
 Serial.print("      Actual PRESSURE= ");
 Serial.print(Pressure);

 Serial.println();
 Serial.print(" RAW Temp D2=  ");
 Serial.print(D2);
 Serial.print(" RAW Pressure D1=  ");
 Serial.println(D1);
 Serial.println();

//  Serial.print(" dT=  ");
//  Serial.println(dT); can't print int64_t size values
 Serial.println();
 Serial.print(" C1 = ");
 Serial.println(C[1]);
 Serial.print(" C2 = ");
 Serial.println(C[2]);
 Serial.print(" C3 = ");
 Serial.println(C[3]);
 Serial.print(" C4 = ");
 Serial.println(C[4]);
 Serial.print(" C5 = ");
 Serial.println(C[5]);
 Serial.print(" C6 = ");
 Serial.println(C[6]);
//  Serial.print(" C7 = ");
//  Serial.println(C[7]);
 Serial.println();

  if(dataFile){
  String dataString = String(Pressure) + "," + String(Temperature) + "," + String(D1)+ "," + String (D2);
  dataFile.println(dataString);
  dataFile.close();
  }
  else{Serial.println("Couldn't access file");}

}

long getVal(int address, byte code)
{
 unsigned long ret = 0;
 Wire.beginTransmission(address);
 Wire.write(code);
 Wire.endTransmission();
 delay(10);
 // start read sequence
 Wire.beginTransmission(address);
 Wire.write((byte) 0x00);
 Wire.endTransmission();
 Wire.beginTransmission(address);
 Wire.requestFrom(address, (int)3);
 if (Wire.available() >= 3)
 {
   ret = Wire.read() * (unsigned long)65536 + Wire.read() * (unsigned long)256 + Wire.read();
 }
 else {
   ret = -1;
 }
 Wire.endTransmission();
 return ret;
}

void initial(uint8_t address)
{

 Serial.println();
 Serial.println("PROM COEFFICIENTS");

 Wire.beginTransmission(address);
 Wire.write(0x1E); // reset
 Wire.endTransmission();
 delay(10);


 for (int i=0; i<6  ; i++) {

   Wire.beginTransmission(address);
   Wire.write(0xA2 + (i * 2));
   Wire.endTransmission();

   Wire.beginTransmission(address);
   Wire.requestFrom(address, (uint8_t) 6);
   delay(1);
   if(Wire.available())
   {
      C[i+1] = Wire.read() << 8 | Wire.read();
   }
   else {
     Serial.println("Error reading PROM 1"); // error reading the PROM or communicating with the device
   }
   Serial.println(C[i+1]);
 }
 Serial.println();
}
