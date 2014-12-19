/* Serial includes */
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
//#include <ctype.h>      // isspace()
/* pThread and sem related includes */
#include <pthread.h>
#include <semaphore.h>
/* User defined functions (most of them are thread routines) */
#include "mavlink_serial.h"
#include "mavlink_udp.h"

// Global parameters, passed by the arguments of main function
bool silent = false;              ///< Wether console output should be enabled
bool verbose = false;             ///< Enable verbose output
bool debug = false;               ///< Enable debug functions and output
/* settings for the serial port which connects to the autopilot */
char global_serial_port_name_autopilot_side[100];
int serial_port_baud_autopilot_side;
/* settings for udp broadcast destination IP address
 * namely the ground station IP address
 */
char ip_groundstation[100];

// Semaphores, global
sem_t sem_mavlink_serial_message_received;
sem_t sem_mavlink_udp_message_received;

int main(int argc, char **argv)
{
    int res;
    void *thread_result;
    /* thread handlers */
    pthread_t mavlink_serial_r_thread;
    pthread_t mavlink_udp_x_thread;
    pthread_t mavlink_serial_x_thread;
    pthread_t mavlink_udp_r_thread;


    /* initialize global parameters */
    strcpy(global_serial_port_name_autopilot_side, "/dev/ttyS1");
    serial_port_baud_autopilot_side = 115200;
    strcpy(ip_groundstation, "192.168.10.101");

    /* Read user entered parameters &
     * set golobal parameters suck as
     * serial port name, baudrate and
     * TCP/IP address
     */
    const char *commandline_usage = "usage: %s -d <serial devicename> -b <baudrate> -i <IP address >[-v/--verbose] [--debug]\nargument:\n\t-d/--device: the serial device connected to the autopilot board.\n\t-b/--baud: baudrate of the serial device mentioned above.\n\t-i/--ip: ip address of ground station or the device receiving message via udp link.\ndefault: -d %s -b %i -i %s\n";

	/* read program arguments */
	int i;

	for (i = 1; i < argc; i++) { /* argv[0] is the name of this program, namely some thing like "gdm3d-uav" */
		if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
			printf(commandline_usage, argv[0], global_serial_port_name_autopilot_side, serial_port_baud_autopilot_side, ip_groundstation);
			return 0;
		}

		/* UART device ID */
		if (strcmp(argv[i], "-d") == 0 || strcmp(argv[i], "--device") == 0) {
			if (argc > i + 1) { // to ensure there exists parameter followed "-d" or "--device"
				strcpy(global_serial_port_name_autopilot_side, argv[i+1]);
#ifdef DEBUG
                printf("DEBUG: UART device is %s\n", global_serial_port_name_autopilot_side);
#endif
			} else {
				printf(commandline_usage, argv[0], global_serial_port_name_autopilot_side, serial_port_baud_autopilot_side, ip_groundstation);
				return 0;
			}
		}

		/* baud rate */
		if (strcmp(argv[i], "-b") == 0 || strcmp(argv[i], "--baud") == 0) {
			if (argc > i + 1) {
				serial_port_baud_autopilot_side = atoi(argv[i+1]);
#ifdef DEBUG
                printf("DEBUG: Baud rate is %d\n", serial_port_baud_autopilot_side);
#endif
			} else {
				printf(commandline_usage, argv[0], global_serial_port_name_autopilot_side, serial_port_baud_autopilot_side, ip_groundstation);
				return 0;
			}
		}

        /* ip address of ground station or other devices */
		if (strcmp(argv[i], "-i") == 0 || strcmp(argv[i], "--ip") == 0) {
			if (argc > i + 1) {
				strcpy(ip_groundstation, argv[i+1]);
#ifdef DEBUG
                printf("DEBUG: IP address of GS is %s\n", ip_groundstation);
#endif
			} else {
				printf(commandline_usage, argv[0], global_serial_port_name_autopilot_side, serial_port_baud_autopilot_side, ip_groundstation);
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
#ifdef DEBUG
    printf("Args are parsed and saved\n");
#endif


    /* Open serial Port */
    serial_port_init();
    /* Initialize UDP link */
    udp_init();

    /* semaphore init
     * local sem for current process
     * initial value for this sem is 0
     *
     * Note: sem_init function prototype:
     * #include <semaphore.h>
     * int sem_init(sem_t *sem, int pshared, unsigned int value);
     * if pshared == 0, this sem is a local sem for current process
     * if pshared !=0, this sem can be shared with other processes
     */
    res = sem_init(&sem_mavlink_serial_message_received, 0, 0);
    if(res != 0)
    {
        perror("Sem 'sem_mavlink_serial_message_received' init failed");
        exit(EXIT_FAILURE);
    }
    res = sem_init(&sem_mavlink_udp_message_received, 0, 0);
    if(res != 0)
    {
        perror("Sem 'sem_mavlink_udp_message_received' init failed");
        exit(EXIT_FAILURE);
    }
#ifdef DEBUG
    printf("Semaphore init successfully\n");
#endif

    /* create thread for mavlink send via udp */
    res = pthread_create(&mavlink_udp_x_thread, NULL, mavlink_udp_send_thread_func, NULL);
    if(res != 0)
    {
        perror("Thread 'mavlink_udp_x_thread' create failed");
        exit(EXIT_FAILURE);
    }

    /* create thread for mavlink send via serial */
    res = pthread_create(&mavlink_serial_x_thread, NULL, mavlink_serial_send_thread_func, NULL);
    if(res != 0)
    {
        perror("Thread 'mavlink_serial_x_thread' create failed");
        exit(EXIT_FAILURE);
    }

    /* create thread for mavlink receive via serial */
    res = pthread_create(&mavlink_serial_r_thread, NULL, mavlink_serial_receive_thread_func, NULL);
    if(res != 0)
    {
        perror("Thread 'mavlink_serial_r_thread' create failed");
        exit(EXIT_FAILURE);
    }
    
    /* create thread for mavlink receive via udp */
    res = pthread_create(&mavlink_udp_r_thread, NULL, mavlink_udp_receive_thread_func, NULL);
    if(res != 0)
    {
        perror("Thread 'mavlink_udp_r_thread' create failed");
        exit(EXIT_FAILURE);
    }
#ifdef DEBUG
    printf("Threads created.\n");
#endif

    /* block here, the thread to be waited can be anyone, since none of
     * the threads would exit or be canceled.
     */
    res = pthread_join(mavlink_serial_r_thread, &thread_result);
    if(res != 0)
    {
        perror("Thread join failed!/n");
        exit(EXIT_FAILURE);
    }
#ifdef DEBUG
    printf("Threads Terminated\n");
#endif

    exit(EXIT_FAILURE);
}
