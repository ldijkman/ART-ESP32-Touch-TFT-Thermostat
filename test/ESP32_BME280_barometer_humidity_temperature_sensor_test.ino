/*********

  Complete project details at https://randomnerdtutorials.com 

changes made by luberth dijkman
For Art Thermostat, a better NEST
   
BME 280 Baromter Humidity and temperature sensor
ESP32 arduino ide example

https://oshwlab.com/l.dijkman
https://github.com/ldijkman/ART-ESP32-Touch-TFT-Thermostat

i2c SDA SCL 
gpio 21, gpio 22
VCC 3.3V 
GND

*********/


#include <Wire.h>
#include <Adafruit_Sensor.h>  // i luberth used https://github.com/adafruit/Adafruit_Sensor
#include <Adafruit_BME280.h>  // i luberth used https://github.com/adafruit/Adafruit_BME280_Library


#define SEALEVELPRESSURE_HPA (1013.25)

Adafruit_BME280 bme; // I2C

void setup() {
  Serial.begin(115200);
  delay(500);
  Serial.println("BME280 test");

  bool status;

  // default settings
  // (you can also pass in a Wire library object like &Wire2)
  status = bme.begin(0x76);  
  if (!status) {
    Serial.println("Could not find a valid BME280 sensor, check wiring!");
    while (1);
  }

  Serial.println("-- Default Test --");
  

  Serial.println();
}


void loop() { 
  printValues();
  delay(1000);
}

void printValues() {
  Serial.print("Temperature = ");
  Serial.print(bme.readTemperature());
  Serial.println(" *C");
  
  // Convert temperature to Fahrenheit
  /*Serial.print("Temperature = ");
  Serial.print(1.8 * bme.readTemperature() + 32);
  Serial.println(" *F");*/
  
  Serial.print("Pressure = ");
  Serial.print(bme.readPressure() / 100.0F);
  Serial.println(" hPa");

  Serial.print("Approx. Altitude = ");
  Serial.print(bme.readAltitude(SEALEVELPRESSURE_HPA));
  Serial.println(" m");

  Serial.print("Humidity = ");
  Serial.print(bme.readHumidity());
  Serial.println(" %");

  Serial.println();
}
