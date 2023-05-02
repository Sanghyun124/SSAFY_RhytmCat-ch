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
    int fsrPin = 5; // MCP3008 ADC 채널 5번 설정
    int fsrValue = 0;

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
        fsrValue = read_mcp3008_adc(fsrPin); // MCP3008 ADC를 사용하여 FSR402 센서 값을 읽음
        printf("FSR402 Sensor value: %d\n", fsrValue);
        delay(500);
    }

    return 0;
}

