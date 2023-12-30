/*
 * hmi_packets.cpp
 *
 *  Created on: Dec 14, 2023
 *      Author: tunah
 */

#include <hmi_packets.hpp>
#include <cstring>
hmi_packets::hmi_packets()
{

}


uart_protocol::packet hmi_packets::packet_periodic(uint16_t _distance_cm, float _angle_deg)
{

    periodic payload{};
    payload.distance = _distance_cm;
    payload.angle    = static_cast<uint16_t>(_angle_deg*100.0);

    uint8_t* byte_ptr = reinterpret_cast<uint8_t*>(&payload);

    std::vector<uint8_t> dataToSend{byte_ptr, byte_ptr + sizeof(periodic)};

    return uart_protocol::pack_packet(static_cast<uint8_t>(types::PERIODIC), dataToSend);

}

uart_protocol::packet hmi_packets::packet_cmd(cmd_types _cmd_type)
{

    uint8_t* byte_ptr = reinterpret_cast<uint8_t*>(&_cmd_type);

    std::vector<uint8_t> dataToSend{byte_ptr, byte_ptr + sizeof(periodic)};

    return uart_protocol::pack_packet(static_cast<uint8_t>(types::CMD), dataToSend);
}

uart_protocol::packet hmi_packets::packet_set_speed(uint32_t _set_speed)
{

    uint8_t* byte_ptr = reinterpret_cast<uint8_t*>(&_set_speed);

    std::vector<uint8_t> dataToSend{byte_ptr, byte_ptr + sizeof(periodic)};

    return uart_protocol::pack_packet(static_cast<uint8_t>(types::SET_SPEED), dataToSend);
}

void hmi_packets::packet_parse(uart_protocol::packet &packet)
{
    switch (static_cast<types>(packet.packet_type)) {
    case types::PERIODIC:

        std::memcpy(&periodic_pack, packet.payload.data(), sizeof(periodic));
        break;
    default:
        break;
    }
}

uint16_t hmi_packets::get_distance()
{
    return periodic_pack.distance;
}

uint16_t hmi_packets::get_angle()
{
    return periodic_pack.angle;
}

