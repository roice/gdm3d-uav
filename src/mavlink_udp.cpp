/* Serial includes */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

void *mavlink_udp_send_thread_func(void *arg)
{
    while(true)
    {
        sem_wait(&sem_mavlink_serial_message_received);
        printf("Acquired sem and message, message id = %d\n", message_mavlink_uart_received.msgid);
    }
    pthread_exit(NULL);
}
