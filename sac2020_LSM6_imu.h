/**
 *             
 *
 * Flight software The configurability of the
 * software--combined with modularity and all-in-one-package style of the
 * Generation 2 flight computer--means the software can be easily adapted to run
 * in any high-power rocket.
 *
 * @file      sac2020_lsm6_imu.h
 * @purpose   Device wrapper for the flight computer's inertial measurement
 *            unit.
 * @author    Pierre McCarragher
 * @updated   2024/01/04
 */

#ifndef SAC2020_LSM6_IMU_H
#define SAC2020_LSM6_IMU_H

#include <Wire.h>
#include <LSM6.h>
#include "photic.h"

#include "utility/imumaths.h"

class Sac2020LSM6Imu final : public photic::Imu
{
public:
    /**
     * Initializes the lsm6.
     *
     * @ret     True if successful, false otherwise.
     */
    bool init()
    {
        bool success = m_lsm6.init();

        if (success)
        {
            m_lsm6.enableDefault();
            return success;
        }else { 
            return false;
        }

    }

    /**
     * Updates orientation and acceleration data with the lsm6's Euler angle
     * and linear accleration readings. Magnetometer data is disregarded.
     *
     * @ret     Always returns true.
     */
    bool update()
    {
        // Read in sensor events from device.
        //sensors_event_t orient, accel;
        m_lsm6.read();
       
        // Populate orientation data. Currently this maps x, y, z to roll,
        // pitch, yaw, but I have no idea if this is correct.
        m_data.gyro_x = m_lsm6.g.x;
        m_data.gyro_y = m_lsm6.g.y;
        m_data.gyro_z = m_lsm6.g.z;
        // Populate acceleration data.
        m_data.accel_x = m_lsm6.a.x;
        m_data.accel_y = m_lsm6.a.y;
        m_data.accel_z = m_lsm6.a.z;

        return true;
    }

  
private:
    /**
     * lsm6 driver.
     */
    LSM6 m_lsm6;

};

#endif
