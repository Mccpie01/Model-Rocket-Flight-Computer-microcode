/**
 *             [ANOTHER FINE PRODUCT FROM THE NONSENSE FACTORY]
 *
 * Flight software The configurability of the
 * software--combined with modularity and all-in-one-package style of the
 * Generation 2 flight computer--means the software can be easily adapted to run
 * in any high-power rocket.
 *
 * @file      sac2020_aux.ino
 * @purpose   Arduino sketch flight computer logging, which
 *            logs data to an SD card.
 * @author    Pierre McCarragher
 * @updated   2024/01/07  
 * @Version   Beta 0.01 
 */



#include <SD.h>
#include <SPI.h>
#include <TeensyThreads.h>

#include "sac2020_aux_pins.h"
#include "sac2020_lib.h"


/**
 * Name of telemetry heap file on SD card. Todo make name unique name+date
 * Todo If file existe rename it to unique name(number) +date (date)
 */
#define TELEM_FNAME "TELEM.DAT"
/**
 * Name of GPS data file on SD card.
 */
#define NMEA_FNAME "NMEA.DAT"

/*********************************** GLOBALS **********************************/


/**
 * Component statuses, set during startup.
 */
Status_t g_sd_status  = Status_t::OFFLINE; // SD card.

/**
 * Whether or not rocket has entered flight. This is flipped on the first
 * receipt of a telemetry packet from main.
 */
bool g_liftoff = false;


/**
 * Controller for status LEDs.
 */
LEDController* g_ledc = nullptr;

/********************************* FUNCTIONS **********************************/

/**
 * Initializes the SD card library and validates file operations. Updates the
 * global status appropriately.
 */
void init_sd()
{
    static const char TEST_FNAME[] = "SDVAL";
    static const char TEST_BUF[]   = "Rage against the dying of the light.";

    // Attempt to initialize library.
    if (!SD.begin(PIN_SDCARD_CHIPSEL))
    {
        fault(PIN_LED_SD_FAULT, "ERROR :: SD INIT FAILED", g_sd_status, g_ledc);
        return;
    }

    // Validate write operation.
    File out = SD.open(TEST_FNAME, FILE_WRITE);
    size_t bytes_written = out.write(TEST_BUF, sizeof TEST_BUF);
    out.close();
    if (bytes_written != sizeof TEST_BUF)
    {
        fault(PIN_LED_SD_FAULT, "ERROR :: SD TEST WRITE FAILED", g_sd_status,
              g_ledc);
        return;
    }

    // Validate read operation.
    File in = SD.open(TEST_FNAME, FILE_READ);
    char buf[sizeof TEST_BUF];
    in.read(buf, sizeof TEST_BUF);
    in.close();
    if (strcmp(buf, TEST_BUF))
    {
        fault(PIN_LED_SD_FAULT, "ERROR :: SD TEST READ FAILED", g_sd_status,
              g_ledc);
        return;
    }

    // Validate remove operation.
    SD.remove(TEST_FNAME);
    if (SD.exists(TEST_FNAME))
    {
        fault(PIN_LED_SD_FAULT, "ERROR :: SD TEST REMOVE FAILED", g_sd_status,
              g_ledc);
        return;
    }

    g_sd_status = Status_t::ONLINE;
}

/*********************************** SETUP ************************************/

void SDsetup()
{
    DEBUG_SERIAL.begin(115200);
    //delay(3000);

       // Set up status LED controller.
    g_ledc = new LEDController({PIN_LED_SD_FAULT,
                                });

    // Initialize subsystems.
    init_sd();

    // Determine if everything initialized correctly.
    bool ok = g_sd_status  == Status_t::ONLINE;
}

/************************************ LOOP ************************************/

void logdata()
{
    // Flash LEDs while on pad.
    if (!g_liftoff)
    {
        g_ledc->run(time_s());
    }

    // Check for state vector receipt from main. Main should only start sending
    // these after liftoff.
    // Otherwise, it's a state vector, so we have entered flight.
        if (!g_liftoff)
        {
            g_liftoff = true;
            g_ledc->lower_all();
        }

        // Copy into struct and write to SD.
        MainStateVector_t vec;
        memcpy(&vec, packet + 1, sizeof(vec));
        File out = SD.open(TELEM_FNAME, FILE_WRITE);
        out.write((uint8_t*) (&vec), sizeof(vec));
        out.close();

        // If the state was VehicleState_t::CONCLUDE, the flight is over--raise
        // all LEDs as indication to recovery team.
        if (vec.state == VehicleState_t::CONCLUDE)
        {
            g_ledc->raise_all();
        }

    

}
