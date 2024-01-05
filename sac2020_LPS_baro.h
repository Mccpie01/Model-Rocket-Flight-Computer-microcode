/**
 *            *
 * Flight software; The configurability of the
 * software--combined with modularity and all-in-one-package style of the
 * Generation 2 flight computer--means the software can be easily adapted to run
 * in any high-power rocket.
 *
 * @file      sac2020__LPS_baro.h
 * @purpose   Device wrapper for the flight computer's barometer.
 * @author    Pierre McCarragher
 * @updated   2024/01/04
 */

#ifndef SAC2020_LPS_BARO_H
#define SAC2020_LPS_BARO_H

#include <Wire.h>
#include <LPS.h>
#include "photic.h"

class Sac2020LPSBarometer final : public photic::Barometer
{
public:
    /**
     * Initializes the lps.
     *
     * @ret     True if successful, false otherwise.
     */
    bool init()
    {
        return m_lps.init();
    }

    /**
     * Reads in all data from the lps.
     *
     * @ret     Always returns true.
     */
    bool update()
    {
        m_data.pressure = m_lps.readPressureMillibars();
        m_data.temperature = m_lps.readTemperatureC();
        m_data.altitude = m_lps.pressureToAltitudeMeters(m_data.pressure);;

        return true;
    }

private:
    /**
     * lps driver.
     */
    LPS m_lps;
};

#endif
