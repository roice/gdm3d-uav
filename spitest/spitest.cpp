#include <stdio.h>

#include <iostream>

#include "ads1256.h"

/* device node of SPI for ADS1256 */
#define SPIDEV_ADS1256  "/dev/spidev0.0"
/* Clock and reference volt of ADS1256 */
#define ADS1256_SPICLK  400000
#define ADS1256_VREF    5.0
#define ADS1256_CH0     0
#define ADS1256_CH1     1
#define ADS1256_CH2     2
#define ADS1256_CH3     3
#define ADS1256_CH4     4
#define ADS1256_CH5     5
#define ADS1256_CH6     6
#define ADS1256_CH7     7

int main()
{
    /* init AD */
    input::ADS1256 devAD(SPIDEV_ADS1256, ADS1256_VREF);
    if (!devAD.init(ADS1256_SPICLK)) {
        printf("Error: Error init AD\n");
        return -1;
    }

    devAD.test();

    /* set & clear IO */
//    if (!devAD.setIO)
    while(1)
    {
        float val;
        if (!devAD.convert(ADS1256_CH0, &val))
            printf("Error: Convert ADC channel 0 error\n");
        else
            printf("%f\t", val);
}
