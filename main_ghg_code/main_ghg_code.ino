/*
MPL115A1
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

//***************************************************************
//MPL115A1 Arduino Code by huinink
//https://forum.sparkfun.com/viewtopic.php?t=23965
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
#define shutDown 9
//***************************************************************

#define buttonPin  7
#define ledPinGreen  2
#define ledPinRed 4
#define ledPinBlue 0

#define mq9bpin 6
#define mq137pin 8
#define mq4pin 9
#define mq131pin 7

//               n--m--sensorRes--totalRes--terminalRes   
const float mq4[] = {1/0.621,8.0933,35.0,15.17,10.0};
const float mq137[] = {1/0.0265,0.5845,38.9,4.97,3.3};
const float mq9b[] = {1/0.542,0.8239,27.0,4.97,3.3};
const float mq131[] = {1/0.4493,0.3579,4195,1530,1000};

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

  pinMode(mq9bpin, INPUT);
  pinMode(mq137pin, INPUT);
  pinMode(mq4pin, INPUT);
  pinMode(mq131pin, INPUT);

  analogReadResolution(12);

  pinMode(chipSelect, OUTPUT);
  
  //initializing CS and SDN pins
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
  airSensor.setAltitudeCompensation(1045); //in metres
  airSensor.setAmbientPressure(900); //in mBar
  airSensor.setTemperatureOffset(2);

//***************************************************************
//MPL115A1 Arduino Code by huinink
//https://forum.sparkfun.com/viewtopic.php?t=23965
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
//***************************************************************
}

void loop() 
{
  button.loop();
  
  lastButtonState = currentButtonState;
  currentButtonState = digitalRead(buttonPin);

  unsigned long currentTime = millis(); //lasts up to 50 days until overflow occurs 

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

       if ((int)ozone() == 0)
       {
          digitalWrite(ledPinRed, HIGH);
       }
  
       if (dataFile)
       {
            //Print to serial display
            digitalWrite(ledPinBlue,HIGH);
            Serial.print(baroPressure());
            Serial.print(",");
            Serial.print(airSensor.getCO2(),1);
            Serial.print(",");
            Serial.print(airSensor.getTemperature(),1);
            Serial.print(",");
            Serial.print(airSensor.getHumidity(),1);
            Serial.print(",");
            Serial.print(methane());
            Serial.print(",");
            Serial.print(monoxide());
            Serial.print(",");
            Serial.print(ammonia());
            Serial.print(",");
            Serial.print(ozone());
            Serial.println(",");
  
            //Write to SD Card
            dataFile.print(baroPressure());
            dataFile.print(",");
            dataFile.print(airSensor.getCO2(),1);
            dataFile.print(",");
            dataFile.print(airSensor.getTemperature(),1);
            dataFile.print(",");
            dataFile.print(airSensor.getHumidity(),1);
            dataFile.print(",");
            dataFile.print(methane());
            dataFile.print(",");
            dataFile.print(monoxide());
            dataFile.print(",");
            dataFile.print(ammonia());
            dataFile.print(",");
            dataFile.print(ozone());
            dataFile.println(",");
            dataFile.close();
            digitalWrite(ledPinBlue,LOW);
           }
  
           else
           {
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

double methane()
{
  const float n = 1/0.621; //power
  const float m = 8.0933; //numerator
  float sensor_analog = analogRead(mq4pin); //reading analog value from pin 0
  float sensor_voltage = sensor_analog*(3.3/4095.0); //ADC conversion
  float rs_target_gas = (5.0*10/sensor_voltage)-15.17; //calculation of resistance using voltage divider
  float ratio = rs_target_gas/35.0; //ratio rs/r0

  double ppm = pow((m/ratio),n);
  return ppm;
}

double monoxide()
{
  const float n = 1/0.542; //power
  const float m = 0.8239; //numerator
  
  float sensor_analog = analogRead(mq9bpin); //reading analog value from pin 0
  float sensor_voltage = sensor_analog*(3.3/4095.0); //ADC conversion
  float rs_target_gas = (5.0*3.3/sensor_voltage)-4.97; //calculation of resistance using voltage divider
  float ratio = rs_target_gas/27.0; //ratio rs/r0

  double ppm = pow((m/ratio),n);
  return ppm;
  
}

double ammonia()
{
  const float n = 1/0.265; //power
  const float m = 0.5845; //numerator
  
  float sensor_analog = analogRead(mq137pin); //reading analog value from pin 0
  float sensor_voltage = sensor_analog*(3.3/4095.0); //ADC conversion
  float rs_target_gas = (5.0*3.3/sensor_voltage)-4.97; //calculation of resistance using voltage divider
  float ratio = rs_target_gas/38.9; //ratio rs/r0

  double ppm = pow((m/ratio),n);
  return ppm;
}

double ozone()
{
  const float n = 1/0.4493; //power
  const float m = 0.3579; //denominator
  
  float sensor_analog = analogRead(mq131pin); //reading analog value from pin 0
  float sensor_voltage = sensor_analog*(3.3/4095.0); //ADC conversion
  float rs_target_gas = (5.0*1000/sensor_voltage)-1530; //calculation of resistance using voltage divider
  float ratio = rs_target_gas/4195; //ratio rs/r0

  double ppb = pow((ratio/m),n);
  return ppb;
}

//***************************************************************
//MPL115A1 Arduino Code by huinink
//https://forum.sparkfun.com/viewtopic.php?t=23965
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

float baroPressure()
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

  unsigned long pre = ((presH *256) + presL)/64;
  unsigned long temp  = ((tempH *256) + tempL)/64;

  float pressure = A0_+(B1_+C12_*temp)*pre+B2_*temp;
  float baro = pressure*  (65.0/1023.0)+50.0;

  return(baro);
}

//***************************************************************
