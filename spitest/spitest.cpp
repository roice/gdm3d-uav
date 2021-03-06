#include <stdio.h>

#include <iostream>

#include <unistd.h>

#include "ads1256.h"

/* device node of SPI for ADS1256 */
#define SPIDEV_NODE_ADC  "/dev/spidev0.0"
/* Clock of the SPI connecting ADS1256 */
#define SPIDEV_CLK_ADC  500000

#define ADC_CH0     (ADS1256_MUXN_AIN0 << 4) | ADS1256_MUXN_AINCOM
#define ADC_CH1     1
#define ADC_CH2     2
#define ADC_CH3     3
#define ADC_CH4     4
#define ADC_CH5     5
#define ADC_CH6     6
#define ADC_CH7     7

int main()
{
    unsigned int val;
    double voltage;

    /* init AD */
    input::ADS1256 devAD(SPIDEV_NODE_ADC, SPIDEV_CLK_ADC);
    if (!devAD.init()) {
        printf("Error: Error init AD\n");
        return -1;
    }


    printf("Init ADC done.\n");

    while(1)
    { 
        if (!devAD.getSample(ADC_CH0, &val))
            printf("Error: Convert ADC channel 0 error\n");
        else
        {
            if (val & 0x800000)
            {
                //val = ~val;
                //val &= 0x7fffff;
                //val += 1;
                //val = -val;
                val = val - 0x800000;
                val = 0x7FFFFF - val + 1;
            }
            printf("Sampled ADC is %d.\n", val);
        }
        usleep(100000);
    }
}
