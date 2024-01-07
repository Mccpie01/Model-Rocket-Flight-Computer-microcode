/**
 *             [ANOTHER FINE PRODUCT FROM THE NONSENSE FACTORY]
 *
 * Flight software The configurability of the
 * software--combined with modularity and all-in-one-package style of the
 * Generation 2 flight computer--means the software can be easily adapted to run
 * in any high-power rocket.
 *
 * @file      sac2020_aux.ino
 * @purpose   Arduino sketch flight computer Telemetry comms, 
 *            Send Data to base Station.
 * @author    Pierre McCarragher
 * @updated   2024/01/07
 * @version.  Beta 0.01
 */

#include <RH_RF95.h>
#include <SPI.h>
#include <TeensyThreads.h>

#include "sac2020_aux_pins.h"
#include "sac2020_lib.h"

/**
 * RF module frequency.
 */
#define RFM_FREQ 915


/*********************************** GLOBALS **********************************/

/**
 * Hardware wrappers, configured during startup.
 */
RH_RF95      g_rfm(PIN_RFM_CHIPSEL, PIN_RFM_INTR);

/**
 * Component statuses, set during startup.
 */
Status_t g_rfm_status = Status_t::OFFLINE; // RF module.

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
 * Initializes the RF module and updates the global status accordingly.
 */
void init_rfm()
{
    pinMode(PIN_RFM_RESET, OUTPUT);
    digitalWrite(PIN_RFM_RESET, HIGH);

    digitalWrite(PIN_RFM_RESET, LOW);
    delay(10);
    digitalWrite(PIN_RFM_RESET, HIGH);
    delay(10);

    if (!g_rfm.init())
    {
        fault(PIN_LED_RFM_FAULT, "ERROR :: RFM INIT FAILED", g_rfm_status,
              g_ledc);
        return;
    }

    if (!g_rfm.setFrequency(RFM_FREQ))
    {
        fault(PIN_LED_RFM_FAULT, "ERROR :: FAILED TO SET RFM FREQ",
              g_rfm_status, g_ledc);
        return;
    }

    g_rfm.setTxPower(23, true);

    g_rfm_status = Status_t::ONLINE;
}

/*********************************** SETUP ************************************/

void setupTele()
{
    DEBUG_SERIAL.begin(115200);
    //delay(3000);

    // Set up status LED controller.
    g_ledc = new LEDController({PIN_LED_RFM_FAULT});
    
    // Initialize subsystems.
    init_rfm();

    // Determine if everything initialized correctly.
    bool ok = g_rfm_status == Status_t::ONLINE;
}

/************************************ LOOP ************************************/

void SendTele()
{
    // Flash LEDs while on pad.
    if (!g_liftoff)
    {
        g_ledc->run(time_s());
    }

        // Otherwise, it's a state vector, so we have entered flight.
        if (!g_liftoff)
        {
            g_liftoff = true;
            g_ledc->lower_all();
        }

        
        // If the state was VehicleState_t::CONCLUDE, the flight is over--raise
        // all LEDs as indication to recovery team.
        if (vec.state == VehicleState_t::CONCLUDE)
        {
            g_ledc->raise_all();
        }
        // Otherwise, flash LED to indicate telemetry activity.
        else
        {
            g_telemtx_led = !g_telemtx_led;
            
        }
    
        // Transmit to ground station via RF module.
         g_rfm.send((uint8_t) (&vec), sizeof(vec));
         g_rfm.waitPacketSent();
    }
}
