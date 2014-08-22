/* Serial includes */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/* pThread and sem related includes */
#include <pthread.h>
#include <semaphore.h>
/* User defined functions (most of them are thread routines) */
#include "mavlink_serial.h"
#include "mavlink_udp.h"

// Semaphores, global
sem_t sem_mavlink_serial_message_received;

int main(int argc, char **argv)
{
    int res;
    void *thread_result;
    /* thread handlers */
    pthread_t mavlink_serial_r_thread;
    pthread_t mavlink_udp_x_thread;

    /* semaphore init
     * local sem for current process
     * initial value for this sem is 0
     */
    res = sem_init(&sem_mavlink_serial_message_received, 0, 0);
    if(res != 0)
    {
        perror("Sem 'sem_mavlink_message_need_send' init failed");
        exit(EXIT_FAILURE);
    }

    /* create thread for mavlink send via udp */
    res = pthread_create(&mavlink_udp_x_thread, NULL, mavlink_udp_send_thread_func, NULL);
    if(res != 0)
    {
        perror("Thread 'mavlink_udp_x_thread' create failed");
        exit(EXIT_FAILURE);
    }

    /* create thread for mavlink receive via serial */
    res = pthread_create(&mavlink_serial_r_thread, NULL, mavlink_serial_receive_thread_func, NULL);
    if(res != 0)
    {
        perror("Thread 'mavlink_udp_x_thread' create failed");
        exit(EXIT_FAILURE);
    }

    /* block here, the thread to be waited can be anyone, since none of
     * the threads would exit or be canceled.
     */
    res = pthread_join(mavlink_serial_r_thread, &thread_result);
    if(res != 0)
    {
        perror("Thread join failed!/n");
        exit(EXIT_FAILURE);
    }

    exit(EXIT_FAILURE);
}
