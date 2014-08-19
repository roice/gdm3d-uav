/** @file
 *	@brief MAVLink comm protocol testsuite generated from gas_sensor.xml
 *	@see http://qgroundcontrol.org/mavlink/
 */
#ifndef GAS_SENSOR_TESTSUITE_H
#define GAS_SENSOR_TESTSUITE_H

#ifdef __cplusplus
extern "C" {
#endif

#ifndef MAVLINK_TEST_ALL
#define MAVLINK_TEST_ALL
static void mavlink_test_common(uint8_t, uint8_t, mavlink_message_t *last_msg);
static void mavlink_test_gas_sensor(uint8_t, uint8_t, mavlink_message_t *last_msg);

static void mavlink_test_all(uint8_t system_id, uint8_t component_id, mavlink_message_t *last_msg)
{
	mavlink_test_common(system_id, component_id, last_msg);
	mavlink_test_gas_sensor(system_id, component_id, last_msg);
}
#endif

#include "../common/testsuite.h"


static void mavlink_test_gas_sensor_raw(uint8_t system_id, uint8_t component_id, mavlink_message_t *last_msg)
{
	mavlink_message_t msg;
        uint8_t buffer[MAVLINK_MAX_PACKET_LEN];
        uint16_t i;
	mavlink_gas_sensor_raw_t packet_in = {
		963497464,
	}963497672,
	}963497880,
	}963498088,
	}963498296,
	};
	mavlink_gas_sensor_raw_t packet1, packet2;
        memset(&packet1, 0, sizeof(packet1));
        	packet1.sensor_1 = packet_in.sensor_1;
        	packet1.sensor_2 = packet_in.sensor_2;
        	packet1.sensor_3 = packet_in.sensor_3;
        	packet1.sensor_4 = packet_in.sensor_4;
        	packet1.time_boot_ms = packet_in.time_boot_ms;
        
        

        memset(&packet2, 0, sizeof(packet2));
	mavlink_msg_gas_sensor_raw_encode(system_id, component_id, &msg, &packet1);
	mavlink_msg_gas_sensor_raw_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
	mavlink_msg_gas_sensor_raw_pack(system_id, component_id, &msg , packet1.sensor_1 , packet1.sensor_2 , packet1.sensor_3 , packet1.sensor_4 , packet1.time_boot_ms );
	mavlink_msg_gas_sensor_raw_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
	mavlink_msg_gas_sensor_raw_pack_chan(system_id, component_id, MAVLINK_COMM_0, &msg , packet1.sensor_1 , packet1.sensor_2 , packet1.sensor_3 , packet1.sensor_4 , packet1.time_boot_ms );
	mavlink_msg_gas_sensor_raw_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
        mavlink_msg_to_send_buffer(buffer, &msg);
        for (i=0; i<mavlink_msg_get_send_buffer_length(&msg); i++) {
        	comm_send_ch(MAVLINK_COMM_0, buffer[i]);
        }
	mavlink_msg_gas_sensor_raw_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);
        
        memset(&packet2, 0, sizeof(packet2));
	mavlink_msg_gas_sensor_raw_send(MAVLINK_COMM_1 , packet1.sensor_1 , packet1.sensor_2 , packet1.sensor_3 , packet1.sensor_4 , packet1.time_boot_ms );
	mavlink_msg_gas_sensor_raw_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);
}

static void mavlink_test_gas_sensor(uint8_t system_id, uint8_t component_id, mavlink_message_t *last_msg)
{
	mavlink_message_t msg;
        uint8_t buffer[MAVLINK_MAX_PACKET_LEN];
        uint16_t i;
	mavlink_gas_sensor_t packet_in = {
		17.0,
	}45.0,
	}73.0,
	}101.0,
	}963498296,
	};
	mavlink_gas_sensor_t packet1, packet2;
        memset(&packet1, 0, sizeof(packet1));
        	packet1.sensor_1 = packet_in.sensor_1;
        	packet1.sensor_2 = packet_in.sensor_2;
        	packet1.sensor_3 = packet_in.sensor_3;
        	packet1.sensor_4 = packet_in.sensor_4;
        	packet1.time_boot_ms = packet_in.time_boot_ms;
        
        

        memset(&packet2, 0, sizeof(packet2));
	mavlink_msg_gas_sensor_encode(system_id, component_id, &msg, &packet1);
	mavlink_msg_gas_sensor_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
	mavlink_msg_gas_sensor_pack(system_id, component_id, &msg , packet1.sensor_1 , packet1.sensor_2 , packet1.sensor_3 , packet1.sensor_4 , packet1.time_boot_ms );
	mavlink_msg_gas_sensor_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
	mavlink_msg_gas_sensor_pack_chan(system_id, component_id, MAVLINK_COMM_0, &msg , packet1.sensor_1 , packet1.sensor_2 , packet1.sensor_3 , packet1.sensor_4 , packet1.time_boot_ms );
	mavlink_msg_gas_sensor_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
        mavlink_msg_to_send_buffer(buffer, &msg);
        for (i=0; i<mavlink_msg_get_send_buffer_length(&msg); i++) {
        	comm_send_ch(MAVLINK_COMM_0, buffer[i]);
        }
	mavlink_msg_gas_sensor_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);
        
        memset(&packet2, 0, sizeof(packet2));
	mavlink_msg_gas_sensor_send(MAVLINK_COMM_1 , packet1.sensor_1 , packet1.sensor_2 , packet1.sensor_3 , packet1.sensor_4 , packet1.time_boot_ms );
	mavlink_msg_gas_sensor_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);
}

static void mavlink_test_gas_sensor(uint8_t system_id, uint8_t component_id, mavlink_message_t *last_msg)
{
	mavlink_test_gas_sensor_raw(system_id, component_id, last_msg);
	mavlink_test_gas_sensor(system_id, component_id, last_msg);
}

#ifdef __cplusplus
}
#endif // __cplusplus
#endif // GAS_SENSOR_TESTSUITE_H
