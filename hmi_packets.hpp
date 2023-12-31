/*
 * hmi_packets.hpp
 *
 *  Created on: Dec 17, 2023
 *      Author: tunah
 */

#ifndef HMI_PACKETS_HPP_
#define HMI_PACKETS_HPP_

#include <hmi_interface.hpp>


class hmi_packets {



public:

    enum class types : uint8_t
    {
        PERIODIC=1,
        CMD,
        SET_SPEED,
    };

    enum class cmd_types : uint8_t
    {
        STOP,
        SCAN_INF,
        SCAN_1_TIME
    };

    enum motor_state{
        Ready,
        Busy
    };
    struct periodic
    {
        uint16_t angle;
        uint16_t distance;
        uint16_t  mot_state;
    };


    static periodic periodic_pack;

	hmi_packets();

    static uart_protocol::packet packet_periodic(uint16_t _distance_cm, float _angle_deg);
    static uart_protocol::packet packet_cmd(cmd_types _cmd_type);
    static uart_protocol::packet packet_set_speed(uint32_t _set_speed);

    static void packet_parse(uart_protocol::packet &packet);

    static uint16_t get_distance();
    static uint16_t get_angle();
private:




    // Add any other necessary member functions or variables
};



#endif /* HMI_PACKETS_HPP_ */
