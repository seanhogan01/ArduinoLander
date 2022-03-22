#include <Wire.h>
#include <SPI.h>
#include <SoftwareSerial.h>
#include "Adafruit_Sensor.h"
#include "Adafruit_BMP280.h"
#include "ADXL335.h"
// Function Declarations
void sensorSetup();
void hc12Setup();
void adxlFunc(); // Acceleromometer
void bmpFunc();  // Temperature / Pressure (Purple Variant)
void hc12Func(); // Wireless Reciever
// Naming and Variable Declarations
ADXL335 accelerometer;
Adafruit_BMP280 bmp; 
SoftwareSerial HC12(7, 6); // TX Pin, RX Pin
const int xInput = A1, yInput = A2, zInput = A3;
String comms;
void setup() {  // Calls Sensor Setup Functions
  bmpSetup();
  sensorSetup();
} 
void loop() {   // Calls Sensor Loop Functions
  adxlFunc();   // Gathers and Stores accelerometer data
  bmpFunc();    // Gathers and Stores Pressure, Temperature and Altlitude data
  hc12Func();   // Prints wirelessly to Serial plotter
  delay(100);
}
// Sensor Setup Functions
void sensorSetup() {
  Serial.begin(115200);
  HC12.begin(115200);   // Serial port to HC12  
  accelerometer.begin();
}
void bmpSetup() {
  bmp.begin();          //Begin the sensor
  while ( !Serial ) delay(100);   // wait for USB
  bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     // Operating Mode
                  Adafruit_BMP280::SAMPLING_X2,     // Temperature Oversampling
                  Adafruit_BMP280::SAMPLING_X16,    // Pressure Oversampling
                  Adafruit_BMP280::FILTER_X16,      // Filtering
                  Adafruit_BMP280::STANDBY_MS_500); // Standby Time
}
// Sensor Loop Functions
void adxlFunc() {
  float ax,ay,az;
  accelerometer.getAcceleration(&ax,&ay,&az); // Writes Accelerometer data to variables
  comms = comms + String((ax-2.05)*100) + "," + String((ay-2.09)*100) + "," + String ((az-3.32)*100) + ",";
  // XYZ Values are multiplied by 100 to make them more visible on Serial Plotter
}
void bmpFunc() {
  float pressure, temperature, altimeter;
  pressure = bmp.readPressure();          // Reads Pressure
  temperature = bmp.readTemperature();    // Reads Temperature
  altimeter = bmp.readAltitude (1013.25); // Finds Alitude
  comms = comms + String(pressure/1000) + "," + String(temperature) + "," + String (altimeter + 63) + ",";
}
void hc12Func() {
  HC12.println(comms); // Print String Comms.
  comms = "\0";        // Wipes String Comms.
}
