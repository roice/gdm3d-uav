/* Serial includes */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>  /* UNIX standard function definitions */
#include <fcntl.h>   /* File control definitions */
#include <errno.h>   /* Error number definitions */
/* Network includes */
#include <sys/socket.h> /* socket functions and data structures */
#include <sys/types.h>  /* basic system data types */
/* Linux POSIX timer headers */
#include <sys/time.h>   /* headers for date and time of linux system */
#include <time.h>       /* headers for date and time */
#include <arpa/inet.h>  /* provide conversion function of IP address */
/* pThread and sem related includes */
#include <pthread.h>
#include <semaphore.h>
/* MAVLINK PROTOCAL includes
 * should change to custom mavlink.h if we have
 * customed message
 */
#include <common/mavlink.h>

// Global mavlink message buffer
extern mavlink_message_t message_mavlink_uart_received;
// Semaphores
extern sem_t sem_mavlink_serial_message_received;
// micro seconds since epoch
uint64_t microsSinceEpoch();

void *mavlink_udp_send_thread_func(void *arg)
{
    while(true)
    {
        sem_wait(&sem_mavlink_serial_message_received);
        printf("Acquired sem and message, message id = %d\n", message_mavlink_uart_received.msgid);
    }
    pthread_exit(NULL);
}
