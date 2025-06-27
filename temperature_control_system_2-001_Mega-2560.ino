// temperature_control_system_2-001_Mega-2560
// en Hanna P 2025

/********************************************/

/*-----( Import needed libraries )-----*/
#include <Wire.h>  // Comes with Arduino IDE

// Get the LCD I2C Library here:
// https://bitbucket.org/fmalpartida/new-liquidcrystal/downloads
// Move any other LCD libraries to another folder or delete them
// See Library "Docs" folder for possible commands etc.
#include <LiquidCrystal_I2C.h>

/*************************************************** 
  This is a library for the Adafruit PT100/P1000 RTD Sensor w/MAX31865
  Designed specifically to work with the Adafruit RTD Sensor
  ----> https://www.adafruit.com/products/3328
  This sensor uses SPI to communicate, 4 pins are required to  
  interface
  Adafruit invests time and resources providing this open source code, 
  please support Adafruit and open-source hardware by purchasing 
  products from Adafruit!
  Written by Limor Fried/Ladyada for Adafruit Industries.  
  BSD license, all text above must be included in any redistribution
 ****************************************************/
#include <Adafruit_MAX31865.h>

#include <SoftwareSerial.h> // Comes with Arduino IDE

/*-----( Declare objects )-----*/
// set the LCD address to 0x20 for a 20 chars 4 line display
// Set the pins on the I2C chip used for LCD connections:
//                    addr, en,rw,rs,d4,d5,d6,d7,bl,blpol
// Set the LCD I2C address 0x20, 0x3f or something else,
// depending on the I2C circuit

LiquidCrystal_I2C lcd(0x3f, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

// Use software SPI: CS, DI, DO, CLK
Adafruit_MAX31865 temperature_sensor_heating_burner = Adafruit_MAX31865(10, 11, 12, 13);
Adafruit_MAX31865 temperature_sensor_heating_tanktop = Adafruit_MAX31865(9, 11, 12, 13);
Adafruit_MAX31865 temperature_sensor_heating_inline = Adafruit_MAX31865(8, 11, 12, 13);
Adafruit_MAX31865 temperature_sensor_heating_hotwater = Adafruit_MAX31865(7, 11, 12, 13);
Adafruit_MAX31865 temperature_sensor_outdoor = Adafruit_MAX31865(6, 11, 12, 13);

// use hardware SPI, just pass in the CS pin
//Adafruit_MAX31865 max = Adafruit_MAX31865(10);

// The value of the Rref resistor. Use 430.0 for PT100 and 4300.0 for PT1000
#define RREF_burner      425.0
#define RREF_tanktop      425.0
#define RREF_inline      425.0
#define RREF_hotwater      425.0
#define RREF_outdoor      425.0
// The 'nominal' 0-degrees-C resistance of the sensor
// 100.0 for PT100, 1000.0 for PT1000
#define RNOMINAL  100.0

void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

}
