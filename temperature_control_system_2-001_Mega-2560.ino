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

/*-----( Declare Constants )-----*/
const int tempValueAdd1 = 52;   // Pin for tempeture value Add1, ESP32
const int tempValueAdd2 = 53;   // Pin for tempeture value Add2, ESP32
const int tempValueAdd3 = 54;   // Pin for tempeture value Add3, ESP32


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

/*-----------------------------------------------------------------
Note for SoftwareSerial:
 Not all pins on the Mega and Mega 2560 support change interrupts,
 so only the following can be used for RX:
 10, 11, 12, 13, 50, 51, 52, 53, 62, 63, 64, 65, 66, 67, 68, 69

 Not all pins on the Leonardo and Micro support change interrupts,
 so only the following can be used for RX:
 8, 9, 10, 11, 14 (MISO), 15 (SCK), 16 (MOSI).
-------------------------------------------------------------------*/

SoftwareSerial mySerial(51, 50); // 51=RX 50=TX

/*-----( Declare Variables )-----*/
float heating_burner;
float heating_tanktop;
float heating_inline;
float heating_hotwater;
float outdoor;
float dataTemp;


void setup() 
{
  /*----( SETUP: RUNS ONCE )----*/
  Serial.begin(9600);  // initialize the hardware UART for speed 9600
  mySerial.begin(9600);   // initialize the baud rate for ESP32 Module
  test_IIC(); //Debug I2C
  lcd.begin(20,4);         // initialize the lcd for 20 chars 4 lines

  digitalWrite(tempValueAdd1, LOW); // Set Add1 pin to Low
  digitalWrite(tempValueAdd2, LOW); // Set Add2 pin to Low

  temperature_sensor_heating_burner.begin(MAX31865_2WIRE);  // Set to 2WIRE or 4WIRE as necessary
  temperature_sensor_heating_tanktop.begin(MAX31865_2WIRE); 
  temperature_sensor_heating_inline.begin(MAX31865_2WIRE);
  temperature_sensor_heating_hotwater.begin(MAX31865_2WIRE);
  temperature_sensor_outdoor.begin(MAX31865_2WIRE);

  lcd.backlight(); // set backlight on

  //-------- Write characters on the display ----------------
  // NOTE: Cursor Position: CHAR, LINE) start at 0 
  lcd.setCursor(0,1); //Start at character 0 on line 2
  lcd.print("Booting up system...");
  delay(1000);
  terminal_display(); // Print some values to terminal
  lcd.clear();
  lcd.setCursor(0,1); //Start at character 0 on line 2
  lcd.print("Temperature control system 2-001_Mega-2560");
  delay(5000);
  lcd.clear();
  /*--(end setup )---*/
}

void loop() 
{
  /*----( LOOP: RUNS CONSTANTLY )----*/

  /*----------Print some values to terminal-----------*/
  //Serial.print("outdoor temperature = "); Serial.println(Temperature_sensor_outdoor.temperature(RNOMINAL, RREF_outdoor));
  /*----------Print to LCD-----------*/
  for(int i = 0; i< 10; i++)
  {
    temperatureRead(); // Reading temperature from sensor)
    if (i < 5) 
      {
        //lcd.clear();
        if ( i == 0)
          {
            lcd.clear();
          }
        lcd.setCursor(0,0);
        lcd.print("Burner = "); lcd.print(heating_burner);
        lcd.setCursor(0, 1);
        lcd.print("Inline = "); lcd.print(heating_inline);
        //lcd.setCursor(10, 1);
        //lcd.print("");
        //lcd.setCursor(12, 2);
        lcd.setCursor(0, 2);
        lcd.print("Hot water = "); lcd.print(heating_hotwater);
        lcd.setCursor(0, 3);
        lcd.print("Out temp = "); lcd.print(outdoor);
        //lcd.print(" = "); lcd.print(max2.temperature(RNOMINAL2, RREF2));   
        delay(1000);                       
      }
    else
      {
        if ( i == 5)
          {
            lcd.clear();
          }      
        //lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Burner = "); lcd.print(heating_burner);
        lcd.setCursor(0,1);
        lcd.print("Tank top = "); lcd.print(heating_tanktop);
        // lcd.setCursor(0,2);
        // lcd.print("P No = "); lcd.print(phoneNro);
        lcd.setCursor(0,3); 
        lcd.print("Up time (min) = "); lcd.print(millis()/60000);
        // lcd.clear(); 
        // lcd.print("str "); lcd.print(inputString.substring(90,130));
        delay(1000);
      }
    }
  }
  /* --(end main loop )-- */
}


void temperatureRead()
{
  /* Reading temperature from sensors
  -------------------------------------------*/
  heating_burner = temperature_sensor_heating_burner.temperature(RNOMINAL, RREF_burner);
  heating_tanktop = temperature_sensor_heating_tanktop.temperature(RNOMINAL, RREF_tanktop);
  heating_inline = temperature_sensor_heating_inline.temperature(RNOMINAL, RREF_inline);
  heating_hotwater = temperature_sensor_heating_hotwater.temperature(RNOMINAL, RREF_hotwater);
  outdoor = temperature_sensor_outdoor.temperature(RNOMINAL, RREF_outdoor);
  //Debug
  /*
  Serial.print("Burner = ");
  Serial.println(heating_burner);
  Serial.print("Tank top = ");
  Serial.println(heating_tanktop);
  Serial.print("Inline = ");
  Serial.println(heating_inline);
  Serial.print("Hot water = ");
  Serial.println(heating_hotwater);
  Serial.print("Out temp = ");
  Serial.println(outdoor);
    */
}


void serialEvent()
{
  while (mySerial.available())
  {
    Serial.print("serial available debug only");
  }
}


void test_IIC() 
//Debug I2C
//Since the seller doesn't provide any info I neede to find the address for the module so I ran an I2C scanner
//Written by Nick Gammon
// Date: 20th April 2011
{
  Serial.println ()
  Serial.println ("I2C scanner. Scanning ...");
  byte count = 0;
  Wire.begin();
  for (byte i = 1; i < 120; i++)
  {
    Wire.beginTransmission (i);
    if (Wire.endTransmission () == 0)
    {
      Serial.print ("Found address: ");
      Serial.print (i, DEC);
      Serial.print (" (0x");
      Serial.print (i, HEX);
      Serial.println (")");
      count++;
      delay (1);  // maybe unneeded?
    } // end of good response
  } // end of for loop
  Serial.println ("Done.");
  Serial.print ("Found ");
  Serial.print (count, DEC);
  Serial.println (" device(s).");
}

void sendData()
{
  // Send temperature data to ESP32 via UART
  dataTemp = heating_burner // For testing
  mySerial.println(dataTemp); // For testing

  for(int i = 0; i < 4; i++)
  {

  }
}