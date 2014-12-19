#ifndef _MAVLINK_UDP_H_
#define _MAVLINK_UDP_H_

/* init function of udp link */
void udp_init(void);

/* the thread routine of Thread "mavlink_udp_x_thread" */
void *mavlink_udp_send_thread_func(void *arg);
/* the thread routine of Thread "mavlink_udp_r_thread" */
void *mavlink_udp_receive_thread_func(void *arg);
#endif
