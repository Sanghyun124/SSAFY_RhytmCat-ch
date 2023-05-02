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
    
    int flexValue0 = 0;
    int flexValue1 = 0;
    int flexValue2 = 0;
    int flexValue3 = 0;
    int flexValue4 = 0;

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
        flexValue0 = read_mcp3008_adc(0); // MCP3008 ADC의 0번 채널을 사용하여 Flex Sensor 값을 읽음
        flexValue1= read_mcp3008_adc(1); // MCP3008 ADC의 1번 채널을 사용하여 Flex Sensor 값을 읽음
        flexValue2= read_mcp3008_adc(2); // MCP3008 ADC의 2번 채널을 사용하여 Flex Sensor 값을 읽음
        flexValue3= read_mcp3008_adc(3); // MCP3008 ADC의 3번 채널을 사용하여 Flex Sensor 값을 읽음
        flexValue4= read_mcp3008_adc(4); // MCP3008 ADC의 4번 채널을 사용하여 Flex Sensor 값을 읽음
	printf("Flex Sensor value \n0 : %d \n1 : %d \n2 : %d \n3 : %d \n4 : %d \n", flexValue0,flexValue1,flexValue2,flexValue3,flexValue4);
        delay(500);
    }

    return 0;
}

