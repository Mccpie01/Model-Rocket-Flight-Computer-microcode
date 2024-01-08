# Model-Rocket-Flight-Computer-microcode
Based on Longhorn-rocketry / sac2020-fsw, An updated version to fit with the UNC 1.0 flight computer board

# sac2020-computer

2024-01-03 Fork Created and looking to Fix compile error and Move the code to a Teensy 4.1 (Unified into 1 Teensy 4.1)
- Todo
  - Update to lates version of Photic (1st Priority)
  - Unified code of Main and Auxiliary Computer to run on 1 teensy 4.1
  - (1st alpha commit done) Add support for AltIMU-10 v6 Gyro, Accelerometer, Compass, and Altimeter (LSM6DSO, LIS3MDL, and LPS22DF Carrier)
  - Add support for more GPS (ATGM336H-5N31, Adafruit Ultimate GPS, NMEA)
  - Add config file for Hardware available (select hardware to use)
  - Maybe add a Web page to see the rocket status
  - Add support to display rocket status on an Oled or ePaper ( When running Bench test )
  - Add support for 6 pyros (Primer and backup)
  - Add support for 10 Servos (For Canard fins, Moterize Parashut deployment, Air break Deployment, and more.)
  - Add More Documentation
  - Add support for the UNC RC 1.1 Flight computer (or Newer)
  - Add a Deploy Package ( Future Project to deploy an edf rocket in the form of a MARS or MOON Lander)
  - Update Data logger to support 50000 transactions per sec
  - Add support for Battery voltage test
  - Add support for the main Arming Security Switch
  - Add support for Pyro Arming Security Switch in series with Main Arming
  - Add support for Continuity testing with 3 states ( Fault No Pyro, Good Pyro Available, Shorted )
<<<<<<< HEAD
  - A file base configuration
  - Validation of Hardwere configuration 
  - Option to same log in flight to flash and copy to SD  when landed or only SD
=======
  - A file base configuration (PIN definition, Module on or off, Bench test mode, or Flight mode)
  - Option to save logs in flight to flash and then copy to SD  when landed or only to SD
  - Smaller Microcode footprint
  - Add fault tolerance
  - Add an error logging to a file with detailed info(option on or off)
  - Add a descriptive header to each function/Procedure with a description of what it does. Detailed detail on the variable, the Version number of the function/Procedure
  - each procedure must return a status and or error code with details of the problem
>>>>>>> 61ee29794a19279d170c4faeed8e506ce71b5626
  - add an option to display the rocket status on the launchpad and after a flight on the I2C Oled screen 
  - find and cleanup bottleneck
  


> To boldly go where no Model Rocket has gone before?

---

## Computer Schema

**Main Computer**

* Teensy 4.1
* Tracks vehicle state with BNO055 IMU and BMP085 barometer or Altimu10-v6 
* Deploys forward canards
* Fires recovery pyros
* Operates GPS
<<<<<<< HEAD
* Operates RadioHead RF95 module communicating with the ground station
* Stores telemetry on the onboard SD card and in flash

=======
* Operates Radiohead RF95 module communicating with the ground station
* Stores telemetry on the onboard SD card
* Send telemetry to the base station with Lora
>>>>>>> 61ee29794a19279d170c4faeed8e506ce71b5626
  

## Arduino Dependencies

* [Adafruit BNO055 driver](https://github.com/adafruit/Adafruit_BNO055)
* [Adafruit BMP085 driver](https://github.com/adafruit/Adafruit-BMP085-Library)
* [Adafruit GPS driver](https://github.com/adafruit/Adafruit_GPS)
* [Adafruit Unified Sensor driver](https://github.com/adafruit/Adafruit_Sensor)
* TinyGPS++ libs
* LSM6DSO(LSM6), LIS3MDL, and LPS22DF(LPS) Drivers for Pololu ALTIMU-10 v6 (https://www.pololu.com/product/2863)
* sac2020_lib (included in the repository)
* [Photic](https://github.com/longhorn-rocketry/photic)
