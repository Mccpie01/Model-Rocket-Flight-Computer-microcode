/**
 *             
 *
 * Flight software The configurability of the
 * software--combined with modularity and all-in-one-package style of the
 * Generation 2 flight computer--means the software can be easily adapted to run
 * in any high-power rocket.
 *
 * @file      sac2020_pins.h
 * @purpose   Pinouts for the flight computer node.
 * @author    Stefan deBruyn
 * @updated   2/23/2020
 * @modified  Pierre McCarragher 2024/01/04
 */

#ifndef SAC2020_PINS_H
#define SAC2020_PINS_H

/**
 * BNO055 IMU.
 */
#define PIN_BNO055_RESET 2
#define PIN_BNO055_ADDR  3
#define PIN_BNO055_INTR  16

/**
 * Servos for Canard fin, Parachute Deployment and Other device

 */
#define PIN_SERVO1_PWM 8
#define PIN_SERVO2_PWM 5
#define PIN_SERVO3_PWM 4
#define PIN_SERVO4_PWM 3
#define PIN_SERVO5_PWM 6
#define PIN_SERVO6_PWM 7
#define PIN_SERVO7_PWM 2
#define PIN_SERVO8_PWM 25
// to controle a Drone Speed and or EDF rocket
#define PIN_SERVO9_PWM 31  //IN1
#define PIN_SERVO10_PWM 40 //IN2
#define PIN_SERVO11_PWM 41 //IN3
#define PIN_SERVO12_PWM 5  //ESC



/**
 * Fault LEDs.
 */
#define PIN_LED_SYS_FAULT   9
#define PIN_LED_IMU_FAULT   10
#define PIN_LED_BLE_FAULT   13
#define PIN_LED_PYRO2_FAULT 12
#define PIN_LED_PYRO1_FAULT 17
#define PIN_LED_FNW_FAULT   20
#define PIN_LED_BARO_FAULT  21
#define PIN_LED_SD_FAULT  21 // SD card.
#define PIN_LED_RFM_FAULT 22 // RF module.
#define PIN_LED_GPS_FAULT 23 // GPS.


/**
 * Bluetooth module. Used on Launch pas for Diagnostic and Status display
 */
#define PIN_BLE_RTS 6
#define PIN_BLE_CTS 14
#define PIN_BLE_MOD 22
#define PIN_BLE_DFU 23

/**
 * WIFI module. Used on Launch pas for Diagnostic and Status display
 */
#define PIN_WIFI_RXD 28   //RX7
#define PIN_WIFI_TXD 29   //TX7


/**
 * Continuity checking.
 */
#define PIN_CONT_ENABLE    24    //TESTONINPUT
#define PIN_CONT_POWER     -1
#define PIN_CONT_PYRO1_CHK 15   //ReadTESTPyro1
#define PIN_CONT_PYRO2_CHK 16   //ReadTESTPyro2
#define PIN_CONT_PYRO3_CHK 17   //ReadTESTPyro3
#define PIN_CONT_PYRO4_CHK 20   //ReadTESTPyro4
#define PIN_CONT_PYRO5_CHK 21   //ReadTESTPyro5
#define PIN_CONT_PYRO6_CHK 22   //ReadTESTPyro5

/**
 * GPS.
 */
#define PIN_GPS_FIX 3 // High when GPS lock is held.

/**
 * RF module Lora 915MHZ
 */
#define PIN_RFM_ENABLE  4    
#define PIN_RFM_RESET   5    //RESET
#define PIN_RFM_INTR    16   //GPIO0
#define PIN_RFM_CHIPSEL 20   //



/**
 * SD card.
 */
#define PIN_SDCARD_CHIPSEL 15


/**
 * Miscellaneous.
 */
#define PIN_LLC_OUT_ENABLE 11
#define PIN_BUZZER         15

#endif
