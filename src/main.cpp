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

    /* Read user entered parameters &
     * set golobal parameters suck as
     * serial port name, baudrate and
     * TCP/IP address
     */
    const char *commandline_usage = "\tusage: %s -d <serial devicename> -b <baudrate> -t <IP address >[-v/--verbose] [--debug]\n\t\tdefault: -d %s -b %i\n";

	/* read program arguments */
	int i;

	for (i = 1; i < argc; i++) { /* argv[0] is "mavlink" */
		if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
			printf(commandline_usage, argv[0], uart_name, baudrate);
			return 0;
		}

		/* UART device ID */
		if (strcmp(argv[i], "-d") == 0 || strcmp(argv[i], "--device") == 0) {
			if (argc > i + 1) {
				uart_name = argv[i + 1];

			} else {
				printf(commandline_usage, argv[0], uart_name, baudrate);
				return 0;
			}
		}

		/* baud rate */
		if (strcmp(argv[i], "-b") == 0 || strcmp(argv[i], "--baud") == 0) {
			if (argc > i + 1) {
				baudrate = atoi(argv[i + 1]);

			} else {
				printf(commandline_usage, argv[0], uart_name, baudrate);
				return 0;
			}
		}

		/* terminating MAVLink is allowed - yes/no */
		if (strcmp(argv[i], "-v") == 0 || strcmp(argv[i], "--verbose") == 0) {
			verbose = true;
		}

		if (strcmp(argv[i], "--debug") == 0) {
			debug = true;
		}
	}

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
