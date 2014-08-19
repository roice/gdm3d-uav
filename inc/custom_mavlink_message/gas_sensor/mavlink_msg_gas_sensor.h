// MESSAGE GAS_SENSOR PACKING

#define MAVLINK_MSG_ID_GAS_SENSOR 201

typedef struct __mavlink_gas_sensor_t
{
 float sensor_1; ///< The scaled values to the described units from ADC channel 1.
 float sensor_2; ///< The scaled values to the described units from ADC channel 2.
 float sensor_3; ///< The scaled values to the described units from ADC channel 3.
 float sensor_4; ///< The scaled values to the described units from ADC channel 4.
 uint32_t time_boot_ms; ///< Time stamp (milliseconds since system boot)
} mavlink_gas_sensor_t;

#define MAVLINK_MSG_ID_GAS_SENSOR_LEN 20
#define MAVLINK_MSG_ID_201_LEN 20

#define MAVLINK_MSG_ID_GAS_SENSOR_CRC 58
#define MAVLINK_MSG_ID_201_CRC 58



#define MAVLINK_MESSAGE_INFO_GAS_SENSOR { \
	"GAS_SENSOR", \
	5, \
	{  { "sensor_1", NULL, MAVLINK_TYPE_FLOAT, 0, 0, offsetof(mavlink_gas_sensor_t, sensor_1) }, \
         { "sensor_2", NULL, MAVLINK_TYPE_FLOAT, 0, 4, offsetof(mavlink_gas_sensor_t, sensor_2) }, \
         { "sensor_3", NULL, MAVLINK_TYPE_FLOAT, 0, 8, offsetof(mavlink_gas_sensor_t, sensor_3) }, \
         { "sensor_4", NULL, MAVLINK_TYPE_FLOAT, 0, 12, offsetof(mavlink_gas_sensor_t, sensor_4) }, \
         { "time_boot_ms", NULL, MAVLINK_TYPE_UINT32_T, 0, 16, offsetof(mavlink_gas_sensor_t, time_boot_ms) }, \
         } \
}


/**
 * @brief Pack a gas_sensor message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param sensor_1 The scaled values to the described units from ADC channel 1.
 * @param sensor_2 The scaled values to the described units from ADC channel 2.
 * @param sensor_3 The scaled values to the described units from ADC channel 3.
 * @param sensor_4 The scaled values to the described units from ADC channel 4.
 * @param time_boot_ms Time stamp (milliseconds since system boot)
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_gas_sensor_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
						       float sensor_1, float sensor_2, float sensor_3, float sensor_4, uint32_t time_boot_ms)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
	char buf[MAVLINK_MSG_ID_GAS_SENSOR_LEN];
	_mav_put_float(buf, 0, sensor_1);
	_mav_put_float(buf, 4, sensor_2);
	_mav_put_float(buf, 8, sensor_3);
	_mav_put_float(buf, 12, sensor_4);
	_mav_put_uint32_t(buf, 16, time_boot_ms);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_GAS_SENSOR_LEN);
#else
	mavlink_gas_sensor_t packet;
	packet.sensor_1 = sensor_1;
	packet.sensor_2 = sensor_2;
	packet.sensor_3 = sensor_3;
	packet.sensor_4 = sensor_4;
	packet.time_boot_ms = time_boot_ms;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_GAS_SENSOR_LEN);
#endif

	msg->msgid = MAVLINK_MSG_ID_GAS_SENSOR;
#if MAVLINK_CRC_EXTRA
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_GAS_SENSOR_LEN, MAVLINK_MSG_ID_GAS_SENSOR_CRC);
#else
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_GAS_SENSOR_LEN);
#endif
}

/**
 * @brief Pack a gas_sensor message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param sensor_1 The scaled values to the described units from ADC channel 1.
 * @param sensor_2 The scaled values to the described units from ADC channel 2.
 * @param sensor_3 The scaled values to the described units from ADC channel 3.
 * @param sensor_4 The scaled values to the described units from ADC channel 4.
 * @param time_boot_ms Time stamp (milliseconds since system boot)
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_gas_sensor_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
							   mavlink_message_t* msg,
						           float sensor_1,float sensor_2,float sensor_3,float sensor_4,uint32_t time_boot_ms)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
	char buf[MAVLINK_MSG_ID_GAS_SENSOR_LEN];
	_mav_put_float(buf, 0, sensor_1);
	_mav_put_float(buf, 4, sensor_2);
	_mav_put_float(buf, 8, sensor_3);
	_mav_put_float(buf, 12, sensor_4);
	_mav_put_uint32_t(buf, 16, time_boot_ms);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_GAS_SENSOR_LEN);
#else
	mavlink_gas_sensor_t packet;
	packet.sensor_1 = sensor_1;
	packet.sensor_2 = sensor_2;
	packet.sensor_3 = sensor_3;
	packet.sensor_4 = sensor_4;
	packet.time_boot_ms = time_boot_ms;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_GAS_SENSOR_LEN);
#endif

	msg->msgid = MAVLINK_MSG_ID_GAS_SENSOR;
#if MAVLINK_CRC_EXTRA
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_GAS_SENSOR_LEN, MAVLINK_MSG_ID_GAS_SENSOR_CRC);
#else
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_GAS_SENSOR_LEN);
#endif
}

/**
 * @brief Encode a gas_sensor struct
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param gas_sensor C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_gas_sensor_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_gas_sensor_t* gas_sensor)
{
	return mavlink_msg_gas_sensor_pack(system_id, component_id, msg, gas_sensor->sensor_1, gas_sensor->sensor_2, gas_sensor->sensor_3, gas_sensor->sensor_4, gas_sensor->time_boot_ms);
}

/**
 * @brief Encode a gas_sensor struct on a channel
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param gas_sensor C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_gas_sensor_encode_chan(uint8_t system_id, uint8_t component_id, uint8_t chan, mavlink_message_t* msg, const mavlink_gas_sensor_t* gas_sensor)
{
	return mavlink_msg_gas_sensor_pack_chan(system_id, component_id, chan, msg, gas_sensor->sensor_1, gas_sensor->sensor_2, gas_sensor->sensor_3, gas_sensor->sensor_4, gas_sensor->time_boot_ms);
}

/**
 * @brief Send a gas_sensor message
 * @param chan MAVLink channel to send the message
 *
 * @param sensor_1 The scaled values to the described units from ADC channel 1.
 * @param sensor_2 The scaled values to the described units from ADC channel 2.
 * @param sensor_3 The scaled values to the described units from ADC channel 3.
 * @param sensor_4 The scaled values to the described units from ADC channel 4.
 * @param time_boot_ms Time stamp (milliseconds since system boot)
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_gas_sensor_send(mavlink_channel_t chan, float sensor_1, float sensor_2, float sensor_3, float sensor_4, uint32_t time_boot_ms)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
	char buf[MAVLINK_MSG_ID_GAS_SENSOR_LEN];
	_mav_put_float(buf, 0, sensor_1);
	_mav_put_float(buf, 4, sensor_2);
	_mav_put_float(buf, 8, sensor_3);
	_mav_put_float(buf, 12, sensor_4);
	_mav_put_uint32_t(buf, 16, time_boot_ms);

#if MAVLINK_CRC_EXTRA
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_GAS_SENSOR, buf, MAVLINK_MSG_ID_GAS_SENSOR_LEN, MAVLINK_MSG_ID_GAS_SENSOR_CRC);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_GAS_SENSOR, buf, MAVLINK_MSG_ID_GAS_SENSOR_LEN);
#endif
#else
	mavlink_gas_sensor_t packet;
	packet.sensor_1 = sensor_1;
	packet.sensor_2 = sensor_2;
	packet.sensor_3 = sensor_3;
	packet.sensor_4 = sensor_4;
	packet.time_boot_ms = time_boot_ms;

#if MAVLINK_CRC_EXTRA
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_GAS_SENSOR, (const char *)&packet, MAVLINK_MSG_ID_GAS_SENSOR_LEN, MAVLINK_MSG_ID_GAS_SENSOR_CRC);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_GAS_SENSOR, (const char *)&packet, MAVLINK_MSG_ID_GAS_SENSOR_LEN);
#endif
#endif
}

#if MAVLINK_MSG_ID_GAS_SENSOR_LEN <= MAVLINK_MAX_PAYLOAD_LEN
/*
  This varient of _send() can be used to save stack space by re-using
  memory from the receive buffer.  The caller provides a
  mavlink_message_t which is the size of a full mavlink message. This
  is usually the receive buffer for the channel, and allows a reply to an
  incoming message with minimum stack space usage.
 */
static inline void mavlink_msg_gas_sensor_send_buf(mavlink_message_t *msgbuf, mavlink_channel_t chan,  float sensor_1, float sensor_2, float sensor_3, float sensor_4, uint32_t time_boot_ms)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
	char *buf = (char *)msgbuf;
	_mav_put_float(buf, 0, sensor_1);
	_mav_put_float(buf, 4, sensor_2);
	_mav_put_float(buf, 8, sensor_3);
	_mav_put_float(buf, 12, sensor_4);
	_mav_put_uint32_t(buf, 16, time_boot_ms);

#if MAVLINK_CRC_EXTRA
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_GAS_SENSOR, buf, MAVLINK_MSG_ID_GAS_SENSOR_LEN, MAVLINK_MSG_ID_GAS_SENSOR_CRC);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_GAS_SENSOR, buf, MAVLINK_MSG_ID_GAS_SENSOR_LEN);
#endif
#else
	mavlink_gas_sensor_t *packet = (mavlink_gas_sensor_t *)msgbuf;
	packet->sensor_1 = sensor_1;
	packet->sensor_2 = sensor_2;
	packet->sensor_3 = sensor_3;
	packet->sensor_4 = sensor_4;
	packet->time_boot_ms = time_boot_ms;

#if MAVLINK_CRC_EXTRA
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_GAS_SENSOR, (const char *)packet, MAVLINK_MSG_ID_GAS_SENSOR_LEN, MAVLINK_MSG_ID_GAS_SENSOR_CRC);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_GAS_SENSOR, (const char *)packet, MAVLINK_MSG_ID_GAS_SENSOR_LEN);
#endif
#endif
}
#endif

#endif

// MESSAGE GAS_SENSOR UNPACKING


/**
 * @brief Get field sensor_1 from gas_sensor message
 *
 * @return The scaled values to the described units from ADC channel 1.
 */
static inline float mavlink_msg_gas_sensor_get_sensor_1(const mavlink_message_t* msg)
{
	return _MAV_RETURN_float(msg,  0);
}

/**
 * @brief Get field sensor_2 from gas_sensor message
 *
 * @return The scaled values to the described units from ADC channel 2.
 */
static inline float mavlink_msg_gas_sensor_get_sensor_2(const mavlink_message_t* msg)
{
	return _MAV_RETURN_float(msg,  4);
}

/**
 * @brief Get field sensor_3 from gas_sensor message
 *
 * @return The scaled values to the described units from ADC channel 3.
 */
static inline float mavlink_msg_gas_sensor_get_sensor_3(const mavlink_message_t* msg)
{
	return _MAV_RETURN_float(msg,  8);
}

/**
 * @brief Get field sensor_4 from gas_sensor message
 *
 * @return The scaled values to the described units from ADC channel 4.
 */
static inline float mavlink_msg_gas_sensor_get_sensor_4(const mavlink_message_t* msg)
{
	return _MAV_RETURN_float(msg,  12);
}

/**
 * @brief Get field time_boot_ms from gas_sensor message
 *
 * @return Time stamp (milliseconds since system boot)
 */
static inline uint32_t mavlink_msg_gas_sensor_get_time_boot_ms(const mavlink_message_t* msg)
{
	return _MAV_RETURN_uint32_t(msg,  16);
}

/**
 * @brief Decode a gas_sensor message into a struct
 *
 * @param msg The message to decode
 * @param gas_sensor C-struct to decode the message contents into
 */
static inline void mavlink_msg_gas_sensor_decode(const mavlink_message_t* msg, mavlink_gas_sensor_t* gas_sensor)
{
#if MAVLINK_NEED_BYTE_SWAP
	gas_sensor->sensor_1 = mavlink_msg_gas_sensor_get_sensor_1(msg);
	gas_sensor->sensor_2 = mavlink_msg_gas_sensor_get_sensor_2(msg);
	gas_sensor->sensor_3 = mavlink_msg_gas_sensor_get_sensor_3(msg);
	gas_sensor->sensor_4 = mavlink_msg_gas_sensor_get_sensor_4(msg);
	gas_sensor->time_boot_ms = mavlink_msg_gas_sensor_get_time_boot_ms(msg);
#else
	memcpy(gas_sensor, _MAV_PAYLOAD(msg), MAVLINK_MSG_ID_GAS_SENSOR_LEN);
#endif
}
