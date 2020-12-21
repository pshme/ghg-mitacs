# **GHG Sensing Project**

## **Introduction and Overview**

The purpose of this research project was to find low-cost and low-power sensors and integrate them into a compact DAQ (Data Acquisition) system for use in UAVs (Unmanned Aerial Vehicles). The sensors measure GHGs (Greenhouse Gasses) in the atmosphere along with complementary ambient readings such as temperature, humidity, and pressure.

The prototype DAQ system uses the following components:

* PJRC Teensy 4.1 Arduino-based microcontroller
* Pololu 5V 2.5A Step-Down Regulator (D24V22F5)
* Pololu 3.3V Step-Up/Step-Down Regulator (S7V8F3)
* Sensirion SCD30 CO2, humidity, and temperature sensor
* Winsen MQ-4 CH4 Sensor
* Winsen MQ-9B CO and CH4 Sensor
* Winsen MQ-131 O3 Sensor
* Winsen MQ-137 NH3 Sensor
* SparkFun Gas Sensor Breakout Boards
* Toggle button, LEDs, and resistors.

Each Winsen sensor was individually calibrated to determine the sensor's resistance and also convert the 5V to 3.3V output logic. The Sensirion SCD30 was calibrated by leaving the sensor powered on in an outdoor environment for 1 hour every day. 

## **Basic Operation**

### **Hardware Controls**

The DAQ has 3 LEDs (one red, one green, and one blue) which represent the state of the system. There are **three** main states and **one** exception state. When the DAQ is first powered on, the system is not logging any data and the red LED is solid. When the button is pressed, then the system can enter one of the following states: 

* *Solid* **RED** - system is not logging to SD card
* *SOLID* **GREEN**, *FLASHING* **BLUE** - system is logging to SD card
* *Flashing* **RED, GREEN, BLUE** - error with SD card or SD card not inserted
* *SOLID* **RED, GREEN**, *FLASHING* **BLUE** - O3 sensor not recording data (exception state).

To exit out of one of the four states, the button must be pressed again; however, if there is an error with the SD card or the SD card is not inserted, then you must reboot the DAQ and check that the error is resolved. 

### **Matlab Plotting**

The Arduino code (*main_ghg_code.ino*) has been modified to first display a title for each column in the .txt file. This helps simplify the Matlab import process. Once a test has been logged to the SD card, the **Import Data** function can be used. It is important to make sure that the information is *delimited* by commas and that each column is imported to Workspace as a *column vector*. After importing the section, *daq_import.m* can be ran, can the graphs will be produced. 




