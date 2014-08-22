#ifndef _MAVLINK_SERIAL_H_
#define _MAVLINK_SERIAL_H_

/* the thread routine of Thread "mavlink_serial_r_thread" */
void *mavlink_serial_receive_thread_func(void *arg);

#endif
