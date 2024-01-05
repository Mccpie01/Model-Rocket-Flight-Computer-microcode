/**
 *
 * Flight software The configurability of the
 * software--combined with modularity and all-in-one-package style of the
 * Generation 2 flight computer--means the software can be easily adapted to run
 * in any high-power rocket.
 *
 * @file      sac2020_gps.ino
 * @purpose   Arduino sketch for the flight computer gps
 * @author    Pierre McCarragher
 * @updated   2024/01/04
 * */

//todo find a way to used 1 or 2 gps at the same time


//For GPS ATGM336H-5N31
#if 1     // 1 used 0 do not used
#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#endif

//Adafruit_GPS
#if 1     // 1 used 0 do not used
#include <Adafruit_GPS.h>
#include <SPI.h>
#endif

#include "sac2020_pins.h"
#include "sac2020_lib.h"

//*********************************** GLOBALS **********************************/

static const uint32_t GPSBaud = 9600; // default baudrate of GPS device
/**
 * Hardware wrappers, configured during startup.
 */
#if 1     // 1 used 0 do not used
Adafruit_GPS g_gps(&Serial3);
#endif

//For GPS ATGM336H-5N31
#if 1     // 1 used 0 do not used/
static const int RXPin = 0, TXPin = 1; // pins for ATGM336H GPS device
//static const uint32_t GPSBaud = 9600; // default baudrate of ATGM336H GPS device

TinyGPSPlus gps;
SoftwareSerial ss(TXPin, RXPin);
//start gps setup
//ss.begin(GPSBaud);
#endif


/**
 * Component statuses, set during startup.
 */

Status_t g_gps_status = Status_t::OFFLINE; // GPS.




/**
 * Controller for status LEDs.
 */
LEDController* g_ledc = nullptr;

/********************************* FUNCTIONS **********************************/


/**
 * Initializes the GPS and updates the global status accordingly.
 */
void init_gps()
{
 //Adafruit_GPS 
 #if 1   
    if (!g_gps.begin(GPSBaud))
    {
        fault(PIN_LED_GPS_FAULT, "ERROR :: GPS INIT FAILED", g_gps_status,
              g_ledc);
        return;
    }

    g_gps.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
    g_gps.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);

    g_gps_status = Status_t::ONLINE;
#endif
//For GPS ATGM336H-5N31
#if 1 
    if (ss.begin(GPSBaud))  // start gps serial communication
    {
        g_gps_status = Status_t::ONLINE;
    }
    else
    {
        fault(PIN_LED_GPS_FAULT, "ERROR :: GPS INIT FAILED", g_gps_status,
              g_ledc);
        return;
    }   
#endif   
}



/*********************************** SETUP ************************************/

void setup()
{

    // Set up status LED controller.
    g_ledc = new LEDController({
                                PIN_LED_GPS_FAULT});
    g_ledc->flash(PIN_LED_FNW_FAULT);

    // Initialize subsystems.
    init_gps();
 

    // Determine if everything initialized correctly.
    bool ok = g_gps_status == Status_t::ONLINE;
}
/************************************ LOOP ************************************/

String GetNmeaAdafruitGPS()
{

    // Check for new NMEA sentence from GPS.
    if (g_gps.newNMEAreceived())
    {
        // Read new sentence.
        char* nmea = g_gps.lastNMEA();

        // Compute sentence length in a memory-safe way.
        // Note: 120 is max sentence length according to GPS library.
        uint32_t len = 0;
        for (; len < 120 && nmea[len] != 0; len++);

              
       return(nmea);
    }
    
}
// GPS ATGM336H-5N31
String GetATGM336HGpsData(){
  while (ss.available() > 0){
    if (gps.encode(ss.read()) && gps.location.isValid() && gps.date.isValid() && gps.time.isValid()){
      String data_to_save = ""; // data string for saving
      data_to_save += String(gps.date.month())+"/"+String(gps.date.day())+"/"+
                      String(gps.date.year())+",";
      data_to_save += String(gps.time.hour())+":"+String(gps.time.minute())+":"+
                      String(gps.time.second())+"."+String(gps.time.centisecond())+",";
      data_to_save += String(gps.location.lat(),8)+","; // latitude
      data_to_save += String(gps.location.lng(),8); // longitude
      return data_to_save;
      //data_saver(data_to_save); // save new data points

   //      Serial.println(data_to_save); // uncomment to print GPS data
    }
  }
}

void SetAdafruitGPSOption(){
   // uncomment this line to turn on RMC (recommended minimum) and GGA (fix data) including altitude
  g_gps.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
  // uncomment this line to turn on only the "minimum recommended" data
  //GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCONLY);
  // For parsing data, we don't suggest using anything but either RMC only or RMC+GGA since
  // the parser doesn't care about other sentences at this time

  // Set the update rate
  g_gps.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);   // 1 Hz update rate
  // For the parsing code to work nicely and have time to sort thru the data, and
  // print it out we don't suggest using anything higher than 1 Hz

  // Request updates on antenna status, comment out to keep quiet
 // g_gps.sendCommand(PGCMD_ANTENNA);

}

void GetAdafruitGPSAntenaStatus(){
 // Request updates on antenna status, comment out to keep quiet
  g_gps.sendCommand(PGCMD_ANTENNA);
} 

void GetandPraseAdafruitNmea(){
  // read data from the GPS in the 'main loop'
  char c = g_gps.read();
  // if you want to debug, this is a good time to do it!
  if (g_gps.newNMEAreceived()) {
    // a tricky thing here is if we print the NMEA sentence, or data
    // we end up not listening and catching other sentences!
    // so be very wary if using OUTPUT_ALLDATA and trying to print out data
    //Serial.println(GPS.lastNMEA());   // this also sets the newNMEAreceived() flag to false
    if (!g_gps.parse(g_gps.lastNMEA()))   // this also sets the newNMEAreceived() flag to false
      return;  // we can fail to parse a sentence in which case we should just wait for another
  }
 
    String data_to_save = ""; // data string for saving
    data_to_save += String("\nTime: ");
    if (g_gps.hour < 10) { data_to_save += String('0'); }
    data_to_save += String(g_gps.hour, DEC); data_to_save += String(':');
    if (g_gps.minute < 10) { data_to_save += String('0'); }
    data_to_save += String(g_gps.minute, DEC); data_to_save += String(':');
    if (g_gps.seconds < 10) { data_to_save += String('0'); }
    data_to_save += String(g_gps.seconds, DEC); data_to_save += String('.');
    if (g_gps.milliseconds < 10) {
      data_to_save += String("00");
    } else if (g_gps.milliseconds > 9 && g_gps.milliseconds < 100) {
      data_to_save += String("0");
    }
    data_to_save += String(g_gps.milliseconds);
    data_to_save += String(" , ");
    data_to_save += String("Date: ");
    data_to_save += String(g_gps.day, DEC); data_to_save += String('/');
    data_to_save += String(g_gps.month, DEC); data_to_save += String("/20");
    data_to_save += String(g_gps.year, DEC);
    data_to_save += String(" , ");
    data_to_save += String("Fix: "); data_to_save += String((int)g_gps.fix);
    data_to_save += String(" quality: "); data_to_save += String((int)g_gps.fixquality);
    if (g_gps.fix) {
      data_to_save += String("Location: ");
      data_to_save += String(g_gps.latitude, 4); data_to_save += String(g_gps.lat);
      data_to_save += String(", ");
      data_to_save += String(g_gps.longitude, 4); data_to_save += String(g_gps.lon);
      data_to_save += String(" , ");
      data_to_save += String("Speed (knots): "); data_to_save += String(g_gps.speed);
      data_to_save += String(" , ");
      data_to_save += String("Angle: "); data_to_save += String(g_gps.angle);
      data_to_save += String(" , ");
      data_to_save += String("Altitude: "); data_to_save += String(g_gps.altitude);
      data_to_save += String(" , ");
      data_to_save += String("Satellites: "); data_to_save += String((int)g_gps.satellites);
      data_to_save += String(" , ");
      data_to_save += String("Antenna status: "); data_to_save += String((int)g_gps.antenna);
    }
  
}