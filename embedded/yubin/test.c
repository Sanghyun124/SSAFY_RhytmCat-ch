#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <wiringPiSPI.h>

#define SPI_CHANNEL 0
#define SPI_SPEED 1000000

int read_mcp3008_adc(unsigned char adcChannel)
{
    unsigned char buff[3];
    int adcValue = 0;

    buff[0] = 0x06 | ((adcChannel & 0x07) >> 2);
    buff[1] = ((adcChannel & 0x07) << 6);
    buff[2] = 0x00;

    wiringPiSPIDataRW(SPI_CHANNEL, buff, 3);

    adcValue = ((buff[1] & 0x0F) << 8) | buff[2];

    return adcValue;
}

int main(void)
{
    int flexPin = 0; // MCP3008 ADC 채널 설정
    int flexValue = 0;

    if (wiringPiSetup() == -1)
    {
        printf("wiringPiSetup failed.\n");
        return -1;
    }

    if (wiringPiSPISetup(SPI_CHANNEL, SPI_SPEED) == -1)
    {
        printf("wiringPiSPISetup failed.\n");
        return -1;
    }

    while (1)
    {
        flexValue = read_mcp3008_adc(flexPin); // MCP3008 ADC를 사용하여 Flex Sensor 값을 읽음
        printf("Flex Sensor value: %d\n", flexValue);
        delay(500);
    }

    return 0;
}