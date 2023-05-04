#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <wiringPi.h>
#include <wiringPiSPI.h>
#include <fcntl.h>
#include <stdint.h>
#include <time.h>
#include "mpu6050_reg.h"

#define SPI_CHANNEL 0
#define SPI_SPEED 1000000

void error_handling(const char *message);

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

char* hand_mode(int f0,int f1,int f2,int f3, int f4, int f5, int f6, int f7){
    return "23";
}

int main(int argc, char *argv[])
{
    // Initialize wiringPi and SPI
    if (wiringPiSetup() == -1 || wiringPiSPISetup(SPI_CHANNEL, SPI_SPEED) == -1)
    {
        printf("Failed to setup wiringPi or SPI\n");
        return -1;
    }

    // MPU6050 initialization
    int mpu6050_fd;
    char mpu6050_data[14];
    int16_t ax, ay, az, gx, gy, gz;
    struct timespec sleep_time;

    mpu6050_fd = open("/dev/mpu6050_device", O_RDONLY);
    if (mpu6050_fd < 0) {
        perror("Failed to open mpu6050_device");
        return 1;
    }

    sleep_time.tv_sec = 0;
    sleep_time.tv_nsec = 100000000; // 100 ms

    // ������ ����
    int serv_sock=socket(PF_INET, SOCK_STREAM, 0);
    if(serv_sock == -1)
        error_handling("socket() error");
    
    // �ּ� ���� �ʱ�ȭ
    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family=AF_INET;
    // INADDR_ANY�� ���� �ڽ��� IP�ּ�
    serv_addr.sin_addr.s_addr=htonl(INADDR_ANY);
    serv_addr.sin_port=htons(30020);
    
    // ���Ͽ� ������ ������ �ּ� ������ �Ҵ�
    if(bind(serv_sock, (struct sockaddr*) &serv_addr, sizeof(serv_addr))==-1 )
        error_handling("bind() error"); 
    
    // Ŭ���̾�Ʈ�� �����û�� �����ϵ��� 5ũ���� ���� ����
    if(listen(serv_sock, 5)==-1)
        error_handling("listen() error");
    
    // accept()�Լ�ȣ���� �ؼ� ���� �����͸� ���� �� �ִ� ������ ����
    socklen_t clnt_addr_size=sizeof(serv_addr);
    int clnt_sock=accept(serv_sock, (struct sockaddr*)&serv_addr,&clnt_addr_size);
    if(clnt_sock==-1)
        error_handling("accept() error");  
    
    // �޽����� ������
    char message[]="Hello UE!";
    write(clnt_sock, message, sizeof(message));
    // ����, Ŭ���̾�Ʈ ������ ������ ����
    char buffer[10]="10";
    char send_buffer[10]="10";
    char* pSend_buffer;
    char end_buffer[10]="00";
    int b=0;
    int flexValue0 = 0;
    int flexValue1 = 0;
    int flexValue2 = 0;
    int flexValue3 = 0;
    int flexValue4 = 0;
    int forceValue5 = 0;
    int forceValue6 = 0;
    int forceValue7 = 0;
    
    while(1){
        flexValue0 = read_mcp3008_adc(0); // MCP3008 ADC�� 0�� ä���� ����Ͽ� Flex Sensor ���� ����
        flexValue1 = read_mcp3008_adc(1); // MCP3008 ADC�� 1�� ä���� ����Ͽ� Flex Sensor ���� ����
        flexValue2 = read_mcp3008_adc(2); // MCP3008 ADC�� 2�� ä���� ����Ͽ� Flex Sensor ���� ����
        flexValue3 = read_mcp3008_adc(3); // MCP3008 ADC�� 3�� ä���� ����Ͽ� Flex Sensor ���� ����
        flexValue4 = read_mcp3008_adc(4); // MCP3008 ADC�� 4�� ä���� ����Ͽ� Flex Sensor ���� ����
        forceValue5 = read_mcp3008_adc(5); // MCP3008 ADC�� 5�� ä���� ����Ͽ� Force Sensor ���� ����
        forceValue6 = read_mcp3008_adc(6); // MCP3008 ADC�� 6�� ä���� ����Ͽ� Force Sensor ���� ����
        forceValue7 = read_mcp3008_adc(7); // MCP3008 ADC�� 7�� ä���� ����Ͽ� Force Sensor ���� ����

        if (read(mpu6050_fd, mpu6050_data, 14) != 14) {
            perror("Failed to read data");
            close(mpu6050_fd);
            return 1;
        }

        ax = (int16_t)((mpu6050_data[0] << 8) | mpu6050_data[1]);
        ay = (int16_t)((mpu6050_data[2] << 8) | mpu6050_data[3]);
        az = (int16_t)((mpu6050_data[4] << 8) | mpu6050_data[5]);
        gx = (int16_t)((mpu6050_data[8] << 8) | mpu6050_data[9]);
        gy = (int16_t)((mpu6050_data[10] << 8) | mpu6050_data[11]);
        gz = (int16_t)((mpu6050_data[12] << 8) | mpu6050_data[13]);
        pSend_buffer = hand_mode(flexValue0,flexValue1,flexValue2,flexValue3,flexValue4,forceValue5,forceValue6,forceValue7);
            printf("Acceleration: X = %d, Y = %d, Z = %d\n", ax, ay, az);
            printf("Gyroscope: X = %d, Y = %d, Z = %d\n", gx, gy, gz);
            write(clnt_sock,pSend_buffer,strlen(pSend_buffer));
            printf("%c, %c\n",pSend_buffer[0],pSend_buffer[1]);
            b++;
        /*if(b>10){
            write(clnt_sock,end_buffer,strlen(end_buffer));
            break;
        }*/
            sleep(1);
    }
    close(clnt_sock);    
    close(serv_sock);
    close(mpu6050_fd);
    return 0;
}
void error_handling(const char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}
