/* Serial includes */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>  /* UNIX standard function definitions */
#include <fcntl.h>   /* File control definitions */
#include <errno.h>   /* Error number definitions */
/* Network includes */
#include <sys/socket.h> /* socket functions and data structures */
#include <netinet/in.h>
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

// minimum buffer size that can be used with qnx (I don't know why)
#define BUFFER_LENGTH 2041
// Declaration of global parameter -- ip address of ground station
extern char ip_groundstation[100];
// Global mavlink message buffer
extern mavlink_message_t message_mavlink_uart_received;
mavlink_message_t message_mavlink_udp_received;
// Semaphores
extern sem_t sem_mavlink_serial_message_received;
extern sem_t sem_mavlink_udp_message_received;
// micro seconds since epoch
uint64_t microsSinceEpoch();


/* parameters of this file */
/* target ip for udp link, stored as char string */
char target_ip[100];
float position[6] = {};
int sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
struct sockaddr_in gcAddr; 
struct sockaddr_in locAddr;
//struct sockaddr_in fromAddr;
uint8_t buf[BUFFER_LENGTH]; // udp send buf
uint8_t buf_r[BUFFER_LENGTH]; // udp receive buf
ssize_t recsize;                // recsize
socklen_t fromlen;              // receivelength

/* UDP initialize */
void udp_init(void)
{
    /* set the ip address, ip_groundstation is a global
     * const char string defined in settings.h */
    strcpy(target_ip, ip_groundstation);
#ifdef DEBUG
    printf("DEBUG: In mavlink_udp_send_thread_func() function of mavlink_udp.cpp, target_ip init as: %s\n", target_ip);
#endif

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
}

void *mavlink_udp_send_thread_func(void *arg)
{    
	uint16_t len;
    int bytes_sent;
    
    for (;;) 
    {
        sem_wait(&sem_mavlink_serial_message_received);
        //printf("Acquired sem and message, message id = %d, micros =\\ %"PRIu64"\n", message_mavlink_uart_received.msgid, microsSinceEpoch());
        len = mavlink_msg_to_send_buffer(buf, &message_mavlink_uart_received);
        bytes_sent = sendto(sock, buf, len, 0, (struct sockaddr*)&gcAddr, sizeof(struct sockaddr_in));
    }

    pthread_exit(NULL);
}

/* the thread routine of Thread "mavlink_udp_r_thread" */
void *mavlink_udp_receive_thread_func(void *arg)
{
    int i = 0;
    unsigned int temp = 0;
    mavlink_message_t message;
    mavlink_status_t status;
   
    for (;;) 
    {
        memset(buf_r, 0, BUFFER_LENGTH);
        recsize = recvfrom(sock, (void *)buf_r, BUFFER_LENGTH, 0, (struct sockaddr *)&gcAddr, &fromlen);
		if (recsize > 0)
      	{
			// Something received - print out all bytes and parse packet
            for (i=0; i<recsize; ++i)
            {
                temp = buf_r[i];
                /* If a message could be decoded, handle it */
                if (mavlink_parse_char(MAVLINK_COMM_1, buf_r[i], &message, &status))
                {/* packet received */
#ifdef DEBUG
                    printf("Received packet: SYS: %d, COMP: %d, LEN: %d, MSG ID: %d\n", message.sysid, message.compid, message.len, message.msgid);
#endif
                    
                   /* copy message to global message buffer and 
                    * post the semaphore to inform the udp mavlink thread to 
                    * send message
                    */
                    message_mavlink_udp_received = message;
                    sem_post(&sem_mavlink_udp_message_received);  
                }
            }
        }
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
