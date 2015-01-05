/*
 *  ADS1256 A/D 'driver' through spidev
 *
 *  Copyright (c) by Carlos Becker	http://github.com/cbecker 
 *
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
 *
 */


#include "./timer_this.h"

#include "ads1256.h"

#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <asm/types.h>
#include <linux/spi/spidev.h>

#include <stdio.h>

#include <string.h>


#include <iostream>

#define	testbit(x,bit)	( (x) & (1<<(bit)) )

using namespace std;

/* microseconds between last written bit and bit read */
/* According to Timing Characteristics of ADS1256 Manual, 
 * this delay is t_6, which is 50/f_clkin. So if SPI Master
 * clock is 50kHz, this delay is 1ms, which is 1000 micro seconds*/
static const int	readDelayUSecs	= 50*1000*1000/ADS1256_SPI_CLK;

namespace input 
{
	ADS1256::ADS1256( const char *spidev) 
	{
		spidevname	= spidev;
	}

	bool	ADS1256::writeCmd( unsigned char cmd )
	{
		int status;

		struct spi_ioc_transfer	xfer[1];

		unsigned char txb1[1];

		txb1[0] = cmd;

		memset( xfer,0,sizeof(xfer));

        
		xfer[0].tx_buf = xfer[0].rx_buf = (unsigned long) txb1;
        /* Modified by Roice, 20150105
        xfer[0].tx_buf = xfer[0].rx_buf = (unsigned int)txb1[0] << 24;
        */

		xfer[0].len = 1;
		
		status = ioctl( fd, SPI_IOC_MESSAGE(1), xfer );
		if ( status < 0 )
			return false;

		return true;

	}

	bool	ADS1256::writeReg( unsigned char reg, unsigned char val, bool verify )
	{
		int status;

		struct spi_ioc_transfer	xfer[1];

		unsigned char txb1[3];

		txb1[0] = ADS1256_CMD_WREG | (reg & 0x0F);
		txb1[1] = 0x00;
		txb1[2] = val;

		memset( xfer,0,sizeof(xfer));

		xfer[0].tx_buf = xfer[0].rx_buf = (unsigned long) txb1;

		xfer[0].len = 3;
		
//		printf("Write %X %X\n", txb1[0], txb1[2]);
		status = ioctl( fd, SPI_IOC_MESSAGE(1), xfer );
		if ( status < 0 )
        {
			printf("Error: ADS1256::writeReg-->write failed.\n");
            return false;
        }

		if ( verify ) {
			unsigned char temp;
			if ( !readReg( reg, &temp ) )
            {
                printf("Error: ADS1256::writeReg-->verify failed, can't read Reg.\n");
                return false;
            }
			if ( temp != val )
            {
                printf("Error: ADS1256::writeReg-->verify failed, value written is %x, value read is %x, not equal.\n", val, temp);
                return false;
            }
		}

		return true;

	}

	bool	ADS1256::readReg( unsigned char reg, unsigned char *val )
	{
		int status;

		/* 2 transfers- read reg needs a delay before reading data */
		struct spi_ioc_transfer	xfer[2];

		unsigned char txb1[2];
		unsigned char txb2[1];

		txb1[0] = ADS1256_CMD_RREG | (reg & 0x0F);
		txb1[1] = 0x00;

		memset( xfer,0,sizeof(xfer));

		xfer[0].tx_buf = xfer[0].rx_buf = (unsigned long) txb1;
		xfer[1].tx_buf = xfer[1].rx_buf = (unsigned long) txb2;

		xfer[0].len = 2;
		xfer[0].delay_usecs	= readDelayUSecs;
		
		xfer[1].len = 1;
		
		status = ioctl( fd, SPI_IOC_MESSAGE(2), xfer );
        
        if ( status < 0 )
        {
            printf("Error: ADS1256::readReg-->read failed.\n");
			return false;
        }

		*val = txb2[0];
		return true;
	}

	bool	ADS1256::readData( int *val )
	{
		int status;

		/* 2 transfers- read reg needs a delay before reading data */
		struct spi_ioc_transfer	xfer[2];

		unsigned char txb1[1];
		unsigned char txb2[4];

		memset( xfer,0,sizeof(xfer));

		txb1[0] = ADS1256_CMD_RDATA;

		xfer[0].tx_buf = xfer[0].rx_buf = (unsigned long) txb1;
		xfer[1].tx_buf = xfer[1].rx_buf = (unsigned long) txb2;
		
        xfer[0].len = 1;
		xfer[0].delay_usecs	= readDelayUSecs;

		xfer[1].len = 3;
		
		status = ioctl( fd, SPI_IOC_MESSAGE(2), xfer );
		if ( status < 0 )
			return false;

		printf("DA: %X %X %X\n", txb2[0], txb2[1], txb2[2] );
        /* 24bit */
		*val = (256*256*txb2[0] + 256*txb2[1] + txb2[2]);

		return true;
	}

	bool ADS1256::init( void ) 
	{
		int ret;

        /* CPOL=low, CPHA=second edge, MSB first, 8bits */
		uint8_t mode = SPI_MODE_1;
		uint8_t bits = 8;
		uint32_t speed = ADS1256_SPI_CLK;

		fd = open(spidevname, O_RDWR);
		if (fd < 0) {
			cout << "can't open device" << endl;
			return false;
		}

        /* Settings of Master SPI interface */
		/* spi mode */
		ret = ioctl(fd, SPI_IOC_WR_MODE, &mode);
		if (ret == -1) {
			cout << "can't set spi mode" << endl;
			return false;
		}
		ret = ioctl(fd, SPI_IOC_RD_MODE, &mode);
		if (ret == -1) {
			cout << "can't get spi mode" << endl;
			return false;
		}
		/* bits per word */
		ret = ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &bits);
		if (ret == -1) {
			cout << "can't set bits per word" << endl;
			return false;
		}
		ret = ioctl(fd, SPI_IOC_RD_BITS_PER_WORD, &bits);
		if (ret == -1) {
			cout << "can't get bits per word";
			return false;
		}
        /* max speed hz */
		ret = ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed);
		if (ret == -1) {
			cout << "can't set max speed hz" << endl;
			return false;
		}
		ret = ioctl(fd, SPI_IOC_RD_MAX_SPEED_HZ, &speed);
		if (ret == -1) {
			cout << "can't get max speed hz" << endl;
		}

        /* Settings of ADS1256 */
        /* status */
        writeReg(ADS1256_STATUS, 0x06, true);
        usleep(1000);
        /* A0:'+' AINCOM:'-' */
        writeReg(ADS1256_MUX, 0x08, true);
        usleep(1000);
        /* Amp 1 */
        writeReg(ADS1256_ADCON, 0x00, true);
        usleep(1000);
        /* data 5sps */
        writeReg(ADS1256_DRATE, ADS1256_DRATE_10SPS, true);
        usleep(1000);
        writeReg(ADS1256_IO, 0x00, true);
        usleep(1000);
		return true;	//ok!
	}

	bool ADS1256::getSample( uint8_t channel, int *result ) 
	{
        int sample_val;

        /* set channel */
        writeReg(ADS1256_MUX, channel);
        writeCmd(ADS1256_CMD_SYNC);
        writeCmd(ADS1256_CMD_WAKEUP);

        if(!readData(&sample_val))
        {
            printf("Error: ADS1256::getSample--readData failed\n");
            return false;
        }
        else
        {
            printf("Sample OK.\t");
            *result = sample_val;
            return true;
        }
    }

};

