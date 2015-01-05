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
static const int	readDelayUSecs	= 10;

static const unsigned char	cmdSync		= 0xFC;
static const unsigned char	cmdWakeUp	= 0x00;
static const unsigned char	cmdStandBy	= 0xFD;
static const unsigned char	cmdReset	= 0xFE;

static const unsigned char	regStatus	= 0x00;
static const unsigned char	regMux		= 0x01;
static const unsigned char	regADCon	= 0x02;
static const unsigned char	regDRate	= 0x03;
static const unsigned char	regIO		= 0x04;

#define	DRATE_30KSPS	0xF0
#define	DRATE_15KSPS	0xE0
#define	DRATE_7500SPS	0xD0
#define	DRATE_3750SPS	0xC0
#define	DRATE_50SPS		0x63

#define	PGA_64			0x06
#define	PGA_32			0x05
#define	PGA_16			0x04
#define	PGA_8			0x03
#define	PGA_4			0x02
#define	PGA_2			0x01
#define	PGA_1			0x00

// define commands 
#define ADS1256_CMD_WAKEUP   0x00 
#define ADS1256_CMD_RDATA    0x01 
#define ADS1256_CMD_RDATAC   0x03 
#define ADS1256_CMD_SDATAC   0x0f 
#define ADS1256_CMD_RREG     0x10 
#define ADS1256_CMD_WREG     0x50 
#define ADS1256_CMD_SELFCAL  0xf0 
#define ADS1256_CMD_SELFOCAL 0xf1 
#define ADS1256_CMD_SELFGCAL 0xf2 
#define ADS1256_CMD_SYSOCAL  0xf3 
#define ADS1256_CMD_SYSGCAL  0xf4 
#define ADS1256_CMD_SYNC     0xfc 
#define ADS1256_CMD_STANDBY  0xfd 
#define ADS1256_CMD_REST    0xfe 
 
// define the ADS1256 register values 
#define ADS1256_STATUS       0x00   
#define ADS1256_MUX          0x01   
#define ADS1256_ADCON        0x02   
#define ADS1256_DRATE        0x03   
#define ADS1256_IO           0x04   
#define ADS1256_OFC0         0x05   
#define ADS1256_OFC1         0x06   
#define ADS1256_OFC2         0x07   
#define ADS1256_FSC0         0x08   
#define ADS1256_FSC1         0x09   
#define ADS1256_FSC2         0x0A 
 
 
// define multiplexer codes 
#define ADS1256_MUXP_AIN0   0x00 
#define ADS1256_MUXP_AIN1   0x10 
#define ADS1256_MUXP_AIN2   0x20 
#define ADS1256_MUXP_AIN3   0x30 
#define ADS1256_MUXP_AIN4   0x40 
#define ADS1256_MUXP_AIN5   0x50 
#define ADS1256_MUXP_AIN6   0x60 
#define ADS1256_MUXP_AIN7   0x70 
#define ADS1256_MUXP_AINCOM 0x80 
 
#define ADS1256_MUXN_AIN0   0x00 
#define ADS1256_MUXN_AIN1   0x01 
#define ADS1256_MUXN_AIN2   0x02 
#define ADS1256_MUXN_AIN3   0x03 
#define ADS1256_MUXN_AIN4   0x04 
#define ADS1256_MUXN_AIN5   0x05 
#define ADS1256_MUXN_AIN6   0x06 
#define ADS1256_MUXN_AIN7   0x07 
#define ADS1256_MUXN_AINCOM 0x08   
 
 
// define gain codes 
#define ADS1256_GAIN_1      0x00 
#define ADS1256_GAIN_2      0x01 
#define ADS1256_GAIN_4      0x02 
#define ADS1256_GAIN_8      0x03 
#define ADS1256_GAIN_16     0x04 
#define ADS1256_GAIN_32     0x05 
#define ADS1256_GAIN_64     0x06 
//#define ADS1256_GAIN_64     0x07 
 
//define drate codes 
#define ADS1256_DRATE_30000SPS   0xF0 
#define ADS1256_DRATE_15000SPS   0xE0 
#define ADS1256_DRATE_7500SPS   0xD0 
#define ADS1256_DRATE_3750SPS   0xC0 
#define ADS1256_DRATE_2000SPS   0xB0 
#define ADS1256_DRATE_1000SPS   0xA1 
#define ADS1256_DRATE_500SPS    0x92 
#define ADS1256_DRATE_100SPS    0x82 
#define ADS1256_DRATE_60SPS     0x72 
#define ADS1256_DRATE_50SPS     0x63 
#define ADS1256_DRATE_30SPS     0x53 
#define ADS1256_DRATE_25SPS     0x43 
#define ADS1256_DRATE_15SPS     0x33 
#define ADS1256_DRATE_10SPS     0x23 
#define ADS1256_DRATE_5SPS      0x13 
#define ADS1256_DRATE_2_5SPS    0x03

namespace input 
{
	ADS1256::ADS1256( const char *spidev, float vref ) 
	{
		spidevname	= spidev;
		vRef = vref;
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
        /* Modified by Roice, 20150105
        xfer[0].tx_buf = xfer[0].rx_buf = ((unsigned int)txb1[0] << 24) + ((unsigned int)txb1[1] << 16) + ((unsigned int)txb1[2] << 8);
        */

		xfer[0].len = 3;
		
//		printf("Write %X %X\n", txb1[0], txb1[2]);
		status = ioctl( fd, SPI_IOC_MESSAGE(1), xfer );
		if ( status < 0 )
			return false;

		if ( verify ) {
			unsigned char temp;
			if ( !readReg( reg, &temp ) )	return false;
			if ( temp != val )	return false;
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

		txb1[0] = 0x10 | (reg & 0x0F);
		txb1[1] = 0x00;

		memset( xfer,0,sizeof(xfer));

		xfer[0].tx_buf = xfer[0].rx_buf = (unsigned long) txb1;
		xfer[1].tx_buf = xfer[1].rx_buf = (unsigned long) txb2;
        /* Modified by Roice, 20150105
        xfer[0].tx_buf = xfer[0].rx_buf = ((unsigned int)txb1[0] << 24) + ((unsigned int)txb1[1] << 16);
		xfer[1].tx_buf = xfer[1].rx_buf = (unsigned int)txb2[0] << 24;
        */

		xfer[0].len = 2;
		xfer[0].delay_usecs	= readDelayUSecs;
		
		xfer[1].len = 1;
		
		status = ioctl( fd, SPI_IOC_MESSAGE(2), xfer );
		if ( status < 0 )
			return false;

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

		txb1[0] = 0x01;

		xfer[0].tx_buf = xfer[0].rx_buf = (unsigned long) txb1;
		xfer[1].tx_buf = xfer[1].rx_buf = (unsigned long) txb2;
        /* Modified by Roice, 20150105
        xfer[0].tx_buf = xfer[0].rx_buf = (unsigned int) txb1[0] << 24;
		xfer[1].tx_buf = xfer[1].rx_buf = ((unsigned int)txb2[0] << 24) + ((unsigned int)txb2[1] << 16) + ((unsigned int)txb2[2] << 8) + ((unsigned int)txb2[3]);
        */
		
        xfer[0].len = 1;
		xfer[0].delay_usecs	= readDelayUSecs;

		xfer[1].len = 3;
		
		status = ioctl( fd, SPI_IOC_MESSAGE(2), xfer );
		if ( status < 0 )
			return false;

//		printf("DA: %X %X %X\n", txb2[0], txb2[1], txb2[2] );
		*val = (256*256*txb2[0] + 256*txb2[1] + txb2[2])<<8;

		*val >>= 8;	//a 24 bits d enuevo
		return true;
	}

	bool	ADS1256::setMux( int ch1, int ch2 ) {
		if ( !writeReg( regMux, (ch2<<4) | ch1, true ) ) {
				printf("Err set mux\n");
				return false;
			}
		return true;
	}

	bool	ADS1256::setIO( unsigned int pin, int high )
	{
		unsigned char rval = 0;
		if ( pin > 3 )	return false;

		if (!readReg( regIO, &rval ))
			return false;

		rval &= ~(1<<(4+pin));	//as output
		if ( high )
			rval |= (1<<pin);
		else
			rval &= ~(1<<pin);

		return writeReg( regIO, rval, true );	//write and verify
	}
	
	bool ADS1256::test()
	{
		struct spi_ioc_transfer	xfer[2];
		unsigned char txb[5];	//1+2+2
		unsigned char	txb2[5];

		int status;

		uint16_t	ret = 0;
		unsigned char tchar;
		int i;

		memset( xfer,0,sizeof(xfer));

		xfer[0].tx_buf = (unsigned long) txb;
		xfer[0].rx_buf = (unsigned long) txb;	//full duplex
        /* Modified by Roice, 20150105
        xfer[0].tx_buf = ((unsigned int)txb[0] << 24) + ((unsigned int)txb[1] << 16) + ((unsigned int)txb[2] << 8) + ((unsigned int)txb[3]);
		xfer[0].rx_buf = ((unsigned int)txb[0] << 24) + ((unsigned int)txb[1] << 16) + ((unsigned int)txb[2] << 8) + ((unsigned int)txb[3]);
        */

		printf("write..\n");


		getchar();

//		if ( !writeReg( regIO, 0, true ) )
//			printf("Err set IO\n");


			if (!writeReg( regDRate, DRATE_7500SPS, true )) {
				printf("ERr set drate\n");
				return false;
			}
			usleep(100);

			usleep(1000);
			if (!writeReg( regStatus, (1<<1) | (1<<2) ) ) //buffer enabled, autocal
			{
				printf("Err set status\n");
				return false;
			}

			usleep(1000);
			if ( !setMux(1,0) ) {
				printf("Err set mux\n");
				return false;
			}
			
			usleep(1000);
			#define	PGA	64.0
			if ( !writeReg( regADCon, PGA_64, true ) ) {
				printf("Error set PGA\n");
				return false;
			}

		return true;

		while(1)
		{
			//
			//getchar();
			usleep(10e3);
			int data;
			
			writeCmd( cmdWakeUp );
			usleep(20000);

			writeCmd( cmdSync );
			readData( &data );
			usleep(100);

			printf("---> %f mV\n", vRef*(1000.0*data)/(PGA*(1<<24)) );

		}
		return true;
	}

	bool	ADS1256::convert( int ch1, int ch2, float *val ) {
		int data;

		/**
		 * TODO: see order, could be faster, check delays
		 */
		if ( !setMux( ch1, ch2 ) )	return false;
		//usleep(100);
		

			writeCmd( cmdWakeUp );
			usleep(100);
			writeCmd( cmdSync );
			readData( &data );
			usleep(100);

			*val = vRef*(data)/(PGA*(1<<24));

			return true;
	}

	bool ADS1256::init( unsigned int spiclk ) 
	{
		int ret;

		uint8_t mode = SPI_MODE_1;
		uint8_t bits = 8;
		uint32_t speed = spiclk;

		fd = open(spidevname, O_RDWR);
		if (fd < 0) {
			cout << "can't open device" << endl;
			return false;
		}

		/*
		 * spi mode
		 */
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

		/*
		 * bits per word
		 */
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

		/*
		 * max speed hz
		 */
		ret = ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed);
		if (ret == -1) {
			cout << "can't set max speed hz" << endl;
			return false;
		}

		ret = ioctl(fd, SPI_IOC_RD_MAX_SPEED_HZ, &speed);
		if (ret == -1) {
			cout << "can't get max speed hz" << endl;
		}




		return true;	//ok!
	}

	bool ADS1256::getSample( uint8_t channel, float *result ) 
	{
		struct spi_ioc_transfer	xfer[2];
		unsigned char txb[5];	//1+2+2
		int status;

		uint16_t	ret = 0;
		int i;


		memset( xfer,0,sizeof(xfer));

		txb[0] = /*start*/ (1<<2) | /*single*/(1<<1) | (channel>>2);
		txb[1] = (channel & 0x3) << 6;
		txb[2] = 0;

		xfer[0].tx_buf = (unsigned long) txb;
		xfer[0].rx_buf = (unsigned long) txb;	//full duplex
        /* Modified by Roice, 20150105
        xfer[0].tx_buf = ((unsigned int)txb[0] << 24) + ((unsigned int)txb[1] << 16) + ((unsigned int)txb[2] << 8) + ((unsigned int)txb[3]);
		xfer[0].rx_buf = ((unsigned int)txb[0] << 24) + ((unsigned int)txb[1] << 16) + ((unsigned int)txb[2] << 8) + ((unsigned int)txb[3]);
        */
		
		xfer[0].len = sizeof(txb);
		

		status = ioctl( fd, SPI_IOC_MESSAGE(1), xfer );

		/** now:
		 *	txb[1,2] => MSB a LSB
		 *	txb[3,4] => LSB a MSB (repeated, util for verification)
		 */

		if ( status < 0 )
		{
			cout << "Error at ioctl" << endl;
			return false;
		}

		txb[1] &= 0xFF >> 3;	//para sacar los MSB que no van
		
		//txb[3] <<= 1;	//el B0 no está en la retransmisión
		txb[4] &= 0xFF << 3;	//también.

		//check LSB
		for (i=1; i < 8; i++ )	//el primero no se chequea
		{
			if ( testbit( txb[2], i ) && ( !testbit( txb[3], 8-i ) ) )
				return false;
			if ( (!testbit( txb[2], i )) &&  testbit( txb[3], 8-i ) )
				return false;
		}


		//check MSB
		for (i=0; i < 12-8-1; i++ )
		{
			if ( testbit( txb[1], i+1 ) && ( !testbit( txb[4], 7-i ) ) )
				return false;
			if ( (!testbit( txb[1], i+1 )) &&  testbit( txb[4], 7-i ) )
				return false;
		}

		//queda 1 que es re loco..
		if ( testbit( txb[1], 0 ) !=  testbit( txb[3], 0 ) )
			return false;

		ret = txb[1]*256 + txb[2];

		if ( ret > 4095 )	//error!
		{
			cout << "Error on data from AD" << endl;
			return false;
		}

		if ( result != NULL ) {
			*result = 5.0*ret/4096;
		}

		return true;
	}

};

