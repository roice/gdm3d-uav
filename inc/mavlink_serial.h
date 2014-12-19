#ifndef _MAVLINK_SERIAL_H_
#define _MAVLINK_SERIAL_H_

/* init function of serial port */
void serial_port_init(void);

/* the thread routine of Thread "mavlink_serial_r_thread" */
void *mavlink_serial_receive_thread_func(void *arg);
/* the thread routine of Thread "mavlink_serial_x_thread" */
void *mavlink_serial_send_thread_func(void *arg);
#endif
