#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h> /* File control definitions */
#include <errno.h>
#include <termios.h> /* POSIX terminal control definitions */

/*
 *  * 'serialport_open()' - Open serial port 1
 *   * Returns the file descriptor on success or -1 on error.
 *    */

int serialport_open(void)
{
    int fd; /* File descriptor for the port */
    struct termios options; // POSIX terminal control struct

    fd = open("/dev/ttyS0", O_RDWR | O_NOCTTY | O_NDELAY);
    if (fd == -1)
    {
        /*
         * Could not open the port.
         *               */
        perror("open_port: Unable to open /dev/ttyS0 -");
    }
    else
    {
        fcntl(fd, F_SETFL, 0);// read in normal(blocking) mode

        /* ============ SETTINGS for SERIAL PORT ==============
         * Get the current configuration for the port, modify it to
         * 57600 baud 8N1 mode, and configure the port */
        /* +++++ Get current configuration +++++ */
        tcgetattr(fd, &options); 
        /* +++++ Set baudrate +++++ */
        /* set input/output baudrate to 57600 baud (autopilot defaut)
         * supported baudrate: B50 B75 B110 B134 B150 B200 B300 B600 B1200
         * B1800 B2400 B4800 B9600 B19200 B38400 B57600 B76800 B115200
         */
        cfsetispeed(&options, B57600); // set input baudrate to 57600
        cfsetospeed(&options, B57600); // set output baudrate to 57600
        /* +++++++ Control options c_cflag struct ++++++++ */
        /* enable the receiver and set local mode */
        options.c_cflag |= (CLOCAL | CREAD);
        /* set parity and character size to 8N1*/
        options.c_cflag &= ~PARENB // no parity
        options.c_cflag &= ~CSTOPB // one stop bit
        options.c_cflag &= ~CSIZE; /* Mask the character size bits */
        options.c_cflag |= CS8; // character size is 8 bit
        /* set hard flow control to OFF
         * Enable hardflow control --> options.c_cflag |= CNEW_RTSCTS;
         * Disable hardflow control--> options.c_cflag &= ~CNEW_RTSCTS;
         */ 
        options.c_cflag &= ~CNEW_RTSCTS;
        /*+++++ Local options c_lflag struct +++++*/
        /* set to raw input
         * raw input --> options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
         * classic input --> options.c_lflag |= (ICANON | ECHO | ECHOE);
         * */
        options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
        /*+++++ Input options c_iflag struct +++++*/
        /* set input soft flow control to OFF
         * Enable  options.c_iflag |= (IXON | IXOFF | IXANY);
         * Disable options.c_iflag &= ~(IXON | IXOFF | IXANY);
         * */
        options.c_iflag &= ~(IXON | IXOFF | IXANY);
        /* +++++ Output options c_oflag struct +++++ */
        /* set raw output 
         * raw output --> options.c_oflag &= ~OPOST;
         * processed output --> options.c_oflag |= OPOST;*/
        options.c_oflag &= ~OPOST;
        /* +++++ Control Characters c_cc struct +++++ */
        /* temporary blank */
        /* set the new options for the port
         * TCSANOW      Make changes now without waiting for data to complete
         * TCSADRAIN    Wait until everything has been transmitted
         * TCSAFLUSH    Flush input and output buffers and make the change
         */
        tcsetattr(fd, TCSANOW, &options);
        close(fd);
        return (fd);
    }
}

void main(void)
{
    open_port();
}
