/*
MPL115A1 sparkfun breakout baropressure meter
 SDN       : pin 7
 CSN       : pin 10
 SDI/MOSI  : pin 11
 SDO/MISO  : pin 12
 SCK       : pin 13
*/

#include <SPI.h>
#include <SD.h>
#include <Wire.h>
#include <ezButton.h>
#include "SparkFun_SCD30_Arduino_Library.h"
SCD30 airSensor;

#define PRESH  0x80
#define PRESL 0x82
#define TEMPH 0x84
#define TEMPL 0x86

#define A0MSB 0x88
#define A0LSB 0x8A
#define B1MSB 0x8C
#define B1LSB 0x8E
#define B2MSB 0x90
#define B2LSB 0x92
#define C12MSB  0x94
#define C12LSB  0x96

#define CONVERT 0x24  

#define chipSelectPin 10
#define shutDown 7

#define buttonPin  0
#define ledPinGreen  1
#define ledPinRed 2
#define ledPinBlue 3

const int chipSelect = BUILTIN_SDCARD;

ezButton button(buttonPin);

int ledState = LOW;
int lastButtonState;
int currentButtonState;


float A0_;
float B1_;
float B2_;
float C12_;

unsigned long previousTime = 0;

void setup() 
{
  Serial.begin(115200);
  SPI.begin(); 
  Wire.begin();
  SD.begin(chipSelect);
  airSensor.begin();

  pinMode(chipSelect, OUTPUT);
  
  // initalize the data ready and chip select pins:
  pinMode(shutDown, OUTPUT);
  digitalWrite(shutDown, HIGH);
  pinMode(chipSelectPin, OUTPUT);
  digitalWrite(chipSelectPin, HIGH);

  //initializing LED pins
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(ledPinGreen, OUTPUT);
  pinMode(ledPinRed, OUTPUT);
  pinMode(ledPinBlue, OUTPUT);
  digitalWrite(ledPinRed, HIGH);

  button.setDebounceTime(50);

  currentButtonState = LOW;

  airSensor.setMeasurementInterval(2); //in seconds
  airSensor.setAltitudeCompensation(30); //in metres
  airSensor.setAmbientPressure(835); //in mBar
  
  // read registers that contain the chip-unique parameters to do the math
  unsigned int A0H = readRegister(A0MSB);
  unsigned int A0L = readRegister(A0LSB);
         A0_ = (A0H << 5) + (A0L >> 3) + (A0L & 0x07) / 8.0;
  
  unsigned int B1H = readRegister(B1MSB);
  unsigned int B1L = readRegister(B1LSB);
          B1_ = ( ( ( (B1H & 0x1F) * 0x100)+B1L) / 8192.0) - 3 ;
  
  unsigned int B2H = readRegister(B2MSB);
  unsigned int B2L = readRegister(B2LSB);
          B2_ = ( ( ( (B2H - 0x80) << 8) + B2L) / 16384.0 ) - 2 ;
  
  unsigned int C12H = readRegister(C12MSB);
  unsigned int C12L = readRegister(C12LSB);
          C12_ = ( ( ( C12H * 0x100 ) + C12L) / 16777216.0 )  ;
}

void loop() 
{
  button.loop();
  
  lastButtonState = currentButtonState;
  currentButtonState = digitalRead(buttonPin);

  unsigned long currentTime = millis();

  if (button.isPressed())
  {
    ledState = !ledState;
    digitalWrite(ledPinGreen, ledState);
    digitalWrite(ledPinRed, !ledState);
  }

  if ((currentTime - previousTime) == 2000 || (currentTime - previousTime) > 2000)
  {
  if (ledState)
  {
     File dataFile = SD.open("baro.txt", FILE_WRITE);

     if (dataFile)
     {
          digitalWrite(ledPinBlue,HIGH);
          Serial.print(baropPessure());
          Serial.print(",");
          Serial.print(airSensor.getCO2());
          Serial.print(",");
          Serial.print(airSensor.getTemperature());
          Serial.print(",");
          Serial.print(airSensor.getHumidity());
          Serial.println(",");
          dataFile.print(baropPessure());
          dataFile.print(",");
          dataFile.print(airSensor.getCO2());
          dataFile.print(",");
          dataFile.print(airSensor.getTemperature());
          dataFile.print(",");
          dataFile.print(airSensor.getHumidity());
          dataFile.println(",");
          dataFile.close();
          digitalWrite(ledPinBlue,LOW);
         }

         else{
          digitalWrite(ledPinGreen,LOW);
          while(!dataFile)
          {
            digitalWrite(ledPinRed,HIGH);
            digitalWrite(ledPinGreen,HIGH);
            digitalWrite(ledPinBlue,HIGH);
            delay(200);
            digitalWrite(ledPinRed,LOW);
            digitalWrite(ledPinGreen,LOW);
            digitalWrite(ledPinBlue,LOW);
            delay(200);
          }
         }
            
            
            
      }
      previousTime = currentTime;
  }
}  

//Read registers
unsigned int readRegister(byte thisRegister ) 
{
  unsigned int result = 0;   // result to return
  digitalWrite(chipSelectPin, LOW);
    delay(10);
    SPI.transfer(thisRegister);
    result = SPI.transfer(0x00);
  digitalWrite(chipSelectPin, HIGH);
  return(result);
}

//read pressure
float baropPessure()
{
  digitalWrite(chipSelectPin, LOW);
  delay(3);
    SPI.transfer(0x24);
    SPI.transfer(0x00);
    digitalWrite(chipSelectPin, HIGH);
    delay(3);
  digitalWrite(chipSelectPin, LOW);
    SPI.transfer(PRESH);
    unsigned int presH = SPI.transfer(0x00);
        delay(3);
    SPI.transfer(PRESL);
    unsigned int presL = SPI.transfer(0x00);
        delay(3);
    SPI.transfer(TEMPH);
    unsigned int tempH = SPI.transfer(0x00);
        delay(3);
    SPI.transfer(TEMPL);
    unsigned int tempL = SPI.transfer(0x00);
        delay(3);
    SPI.transfer(0x00);
      delay(3);
  digitalWrite(chipSelectPin, HIGH);

  unsigned long press = ((presH *256) + presL)/64;
  unsigned long temp  = ((tempH *256) + tempL)/64;

  float pressure = A0_+(B1_+C12_*temp)*press+B2_*temp;
  float preskPa = pressure*  (65.0/1023.0)+50.0;

  return(preskPa);
}
