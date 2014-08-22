/* Serial includes */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>  /* UNIX standard function definitions */
#include <fcntl.h>   /* File control definitions */
#include <errno.h>   /* Error number definitions */
/* Network includes */
#include <sys/socket.h> /* socket functions and data structures */
/* Linux POSIX timer headers */
#include <sys/time.h>   /* headers for date and time of linux system */
#include <time.h>       /* headers for date and time */
#include <arpa/inet.h>  /* provide conversion function of IP address */
/* headers for data types */
#include <inttypes.h>   /* types for integers, especially for uint64_t print support */
#include <sys/types.h>  /* basic system data types */
/* pThread and sem related includes */
#include <pthread.h>
#include <semaphore.h>
/* MAVLINK PROTOCAL includes
 * should change to custom mavlink.h if we have
 * customed message
 */
#include <common/mavlink.h>
/* User settings, IP address of udp broadcasting in this case */
#include "settings.h"

// minimum buffer size that can be used with qnx (I don't know why)
#define BUFFER_LENGTH 2041 
// Global mavlink message buffer
extern mavlink_message_t message_mavlink_uart_received;
// Semaphores
extern sem_t sem_mavlink_serial_message_received;
// micro seconds since epoch
uint64_t microsSinceEpoch();

void *mavlink_udp_send_thread_func(void *arg)
{
    /* target ip for udp link, stored as char string */
    char target_ip[100];
 
	float position[6] = {};
	int sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
	struct sockaddr_in gcAddr; 
	struct sockaddr_in locAddr;
    //struct sockaddr_in fromAddr;
	uint8_t buf[BUFFER_LENGTH];
	ssize_t recsize;
	socklen_t fromlen;
	int bytes_sent;
	mavlink_message_t msg;
	uint16_t len;
	int i = 0;
	//int success = 0;
	unsigned int temp = 0;

    /* set the ip address, ip_groundstation is a global
     * const char string defined in settings.h */
    strcpy(target_ip, ip_groundstation);

    memset(&locAddr, 0, sizeof(locAddr));
	locAddr.sin_family = AF_INET;
	locAddr.sin_addr.s_addr = INADDR_ANY;
	locAddr.sin_port = htons(14551);
 
	/* Bind the socket to port 14551 - necessary to receive packets from qgroundcontrol */ 
	if (-1 == bind(sock,(struct sockaddr *)&locAddr, sizeof(struct sockaddr)))
    {
		perror("error bind failed");
		close(sock);
		exit(EXIT_FAILURE);
    }

    /* Attempt to make it non blocking */
	if (fcntl(sock, F_SETFL, O_NONBLOCK | FASYNC) < 0)
    {
		fprintf(stderr, "error setting nonblocking: %s\n", strerror(errno));
		close(sock);
		exit(EXIT_FAILURE);
    }

    memset(&gcAddr, 0, sizeof(gcAddr));
	gcAddr.sin_family = AF_INET;
	gcAddr.sin_addr.s_addr = inet_addr(target_ip);
	gcAddr.sin_port = htons(14550);

    for (;;) 
    {
        sem_wait(&sem_mavlink_serial_message_received);
        printf("Acquired sem and message, message id = %d, micros = %"PRIu64"\n", message_mavlink_uart_received.msgid, microsSinceEpoch());
        len = mavlink_msg_to_send_buffer(buf, &message_mavlink_uart_received);
        bytes_sent = sendto(sock, buf, len, 0, (struct sockaddr*)&gcAddr, sizeof(struct sockaddr_in));
    }

    while(true)
    {
        

        /*Send Heartbeat */
		mavlink_msg_heartbeat_pack(1, 200, &msg, MAV_TYPE_HELICOPTER, MAV_AUTOPILOT_GENERIC, MAV_MODE_GUIDED_ARMED, 0, MAV_STATE_ACTIVE);
		len = mavlink_msg_to_send_buffer(buf, &msg);
		bytes_sent = sendto(sock, buf, len, 0, (struct sockaddr*)&gcAddr, sizeof(struct sockaddr_in));
 
		/* Send Status */
		mavlink_msg_sys_status_pack(1, 200, &msg, 0, 0, 0, 500, 11000, -1, -1, 0, 0, 0, 0, 0, 0);
		len = mavlink_msg_to_send_buffer(buf, &msg);
		bytes_sent = sendto(sock, buf, len, 0, (struct sockaddr*)&gcAddr, sizeof (struct sockaddr_in));
 
		/* Send Local Position */
		mavlink_msg_local_position_ned_pack(1, 200, &msg, microsSinceEpoch(), 
										position[0], position[1], position[2],
										position[3], position[4], position[5]);
		len = mavlink_msg_to_send_buffer(buf, &msg);
		bytes_sent = sendto(sock, buf, len, 0, (struct sockaddr*)&gcAddr, sizeof(struct sockaddr_in));
 
		/* Send attitude */
		mavlink_msg_attitude_pack(1, 200, &msg, microsSinceEpoch(), 1.2, 1.7, 3.14, 0.01, 0.02, 0.03);
		len = mavlink_msg_to_send_buffer(buf, &msg);
		bytes_sent = sendto(sock, buf, len, 0, (struct sockaddr*)&gcAddr, sizeof(struct sockaddr_in));
 
 
		memset(buf, 0, BUFFER_LENGTH);
		recsize = recvfrom(sock, (void *)buf, BUFFER_LENGTH, 0, (struct sockaddr *)&gcAddr, &fromlen);
		if (recsize > 0)
      	{
			// Something received - print out all bytes and parse packet
			mavlink_message_t msg;
			mavlink_status_t status;
 
			printf("Bytes Received: %d\nDatagram: ", (int)recsize);
			for (i = 0; i < recsize; ++i)
			{
				temp = buf[i];
				printf("%02x ", (unsigned char)temp);
				if (mavlink_parse_char(MAVLINK_COMM_0, buf[i], &msg, &status))
				{
					// Packet received
					printf("\nReceived packet: SYS: %d, COMP: %d, LEN: %d, MSG ID: %d\n", msg.sysid, msg.compid, msg.len, msg.msgid);
				}
			}
			printf("\n");
		}
		memset(buf, 0, BUFFER_LENGTH);
		sleep(1); // Sleep one second
    }
    pthread_exit(NULL);
}

/* QNX timer version */
#if (defined __QNX__) | (defined __QNXNTO__)
uint64_t microsSinceEpoch()
{
 
	struct timespec time;
 
	uint64_t micros = 0;
 
	clock_gettime(CLOCK_REALTIME, &time);  
	micros = (uint64_t)time.tv_sec * 1000000 + time.tv_nsec/1000;
 
	return micros;
}
#else
uint64_t microsSinceEpoch()
{
 
	struct timeval tv;
 
	uint64_t micros = 0;
 
	gettimeofday(&tv, NULL);  
	micros =  ((uint64_t)tv.tv_sec) * 1000000 + tv.tv_usec;
 
	return micros;
}
#endif
