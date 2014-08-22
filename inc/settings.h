#ifndef _SETTINGS_H_
#define _SETTINGS_H_

/* settings for the serial port which connects to the autopilot */
#define SERIAL_PORT_BAUD_AUTOPILOT_SIDE 115200
char global_serial_port_name_autopilot_side[] = "/dev/ttyUSB0";

/* settings for udp broadcast destination IP address
 * namely the ground station IP address
 */
const char ip_groundstation[] = "192.168.10.102";

#endif
