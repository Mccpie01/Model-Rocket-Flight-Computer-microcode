/**
 *             
 *
 * Flight software The configurability of the
 * software--combined with modularity and all-in-one-package style of the
 * Generation 2 flight computer--means the software can be easily adapted to run
 * in any high-power rocket.
 *
 * @file      sac2020_LIS3MDL_imu.h
 * @purpose   Device wrapper for the flight computer's magnetometer measurement
 *           
 * @author    Pierre McCarragher
 * @updated   2024/01/04
 */

#ifndef SAC2020_LIS3MDL_H
#define SAC2020_LIS3MDL_H

#include <Wire.h>
#include <LIS3MDL.h>
#include "photic.h"

#include "utility/imumaths.h"

LIS3MDL::vector<int16_t> running_min = {32767, 32767, 32767}, running_max = {-32768, -32768, -32768};

class Sac2020lis3mdlImu final : public photic::Imu
{
public:
    /**
     * Initializes the lis3mdl.
     *
     * @ret     True if successful, false otherwise.
     */
    bool init()
    {
        bool success = m_lis3mdl.init();

        if (success)
        {
            m_lis3mdl.enableDefault();
            return success;
        }else { 
            return false;
        }

    }

    /**
     * Updates orientation and acceleration data with the lis3mdl's Euler angle
     * and linear accleration readings. Magnetometer data is disregarded.
     *
     * @ret     Always returns true.
     */
    bool update()
    {
        // Read in sensor events from device.
        //sensors_event_t orient, accel;
        m_lis3mdl.read();
       
        // Populate orientation data. Currently this maps x, y, z to roll,
        // pitch, yaw, but I have no idea if this is correct.
        m_data.gyro_x = m_lis3mdl.m.x;
        m_data.gyro_y = m_lis3mdl.m.y;
        m_data.gyro_z = m_lis3mdl.m.z;


        return true;
    }

   bool calibrate()
    {
        // Read in sensor events from device.
        //sensors_event_t orient, accel;
        m_lis3mdl.read();
       
        // Calibrating magnetometer
        for (uint16_t i = 0; i < 327; ++i) {
            running_min.x = min(running_min.x, m_lis3mdl.m.x);
            running_min.y = min(running_min.y, m_lis3mdl.m.y);
            running_min.z = min(running_min.z, m_lis3mdl.m.z);

            running_max.x = max(running_max.x, m_lis3mdl.m.x);
            running_max.y = max(running_max.y, m_lis3mdl.m.y);
            running_max.z = max(running_max.z, m_lis3mdl.m.z);
        }

        return true;
    }

  
private:
    /**
     * lis3mdl driver.
     */
    LIS3MDL m_lis3mdl;

};

#endif
