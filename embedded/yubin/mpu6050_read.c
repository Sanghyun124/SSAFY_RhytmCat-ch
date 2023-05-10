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

#define F0_LIMIT 470
#define F1_LIMIT 320
#define F2_LIMIT 280
#define F3_LIMIT 350
#define F4_LIMIT 330
#define FORCE_LIMIT 500 

void error_handling(const char* message);

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

// f0 pinky f1 ring f2 middle f3 index f4 thumb
// f5 thumb f6 palm f7 knife
char* hand_mode(int f0, int f1, int f2, int f3, int f4, int f5, int f6, int f7) {
	// receive
	if (f6 > FORCE_LIMIT) {
		return "1";
	}
	// bat
	if (f7 > FORCE_LIMIT) {
		return "2";
	}
	// rock
	if (f0 > F0_LIMIT && f1 > F1_LIMIT && f2 > F2_LIMIT && f3 > F3_LIMIT && f4 > F4_LIMIT) {
		return "3";
	}
	// thumb up
	if (f0 < F0_LIMIT && f1 > F1_LIMIT && f2 > F2_LIMIT && f3 > F3_LIMIT && f4 > F4_LIMIT) {
		return "4";
	}
	// thumb up
	if (f0 < F0_LIMIT && f1 > F1_LIMIT && f2 > F2_LIMIT && f3 > F3_LIMIT && f4 > F4_LIMIT) {
		return "5";
	}
	// knife
	if (f0 < F0_LIMIT && f1 < F1_LIMIT && f2 < F2_LIMIT && f3 < F3_LIMIT && f4 < F4_LIMIT) {
		return "6";
	}
	// spike
	if (f0 < F0_LIMIT && f1 < F1_LIMIT && f2 < F2_LIMIT && f3 < F3_LIMIT && f4 < F4_LIMIT) {
		return "4";
	}
	else {
		return "0";
	}
}

int main(int argc, char* argv[])
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

	// 소켓을 생성
	int serv_sock = socket(PF_INET, SOCK_STREAM, 0);
	if (serv_sock == -1)
		error_handling("socket() error");

	// 주소 정보 초기화
	struct sockaddr_in serv_addr;
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	// INADDR_ANY는 서버 자신의 IP주소
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(30020);

	// 소켓에 위에서 생성한 주소 정보를 할당
	if (bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
		error_handling("bind() error");

	// 클라이언트가 연결요청이 가능하도록 5크기의 대기실 생성
	if (listen(serv_sock, 5) == -1)
		error_handling("listen() error");

	// accept()함수호출을 해서 실제 데이터를 보낼 수 있는 소켓을 생성
	socklen_t clnt_addr_size = sizeof(serv_addr);
	int clnt_sock = accept(serv_sock, (struct sockaddr*)&serv_addr, &clnt_addr_size);
	if (clnt_sock == -1)
		error_handling("accept() error");

	// 메시지를 보낸다
	char message[] = "Hello UE!";
	write(clnt_sock, message, sizeof(message));
	// 서버, 클라이언트 소켓의 연결을 해제
	char buffer[10] = "10";
	char send_buffer[10] = "10";
	char* pSend_buffer;
	char end_buffer[10] = "00";
	int b = 0;
	int flexValue0 = 0;
	int flexValue1 = 0;
	int flexValue2 = 0;
	int flexValue3 = 0;
	int flexValue4 = 0;
	int forceValue5 = 0;
	int forceValue6 = 0;
	int forceValue7 = 0;

	while (1) {
		flexValue0 = read_mcp3008_adc(0); // MCP3008 ADC의 0번 채널을 사용하여 Flex Sensor 값을 읽음
		flexValue1 = read_mcp3008_adc(1); // MCP3008 ADC의 1번 채널을 사용하여 Flex Sensor 값을 읽음
		flexValue2 = read_mcp3008_adc(2); // MCP3008 ADC의 2번 채널을 사용하여 Flex Sensor 값을 읽음
		flexValue3 = read_mcp3008_adc(3); // MCP3008 ADC의 3번 채널을 사용하여 Flex Sensor 값을 읽음
		flexValue4 = read_mcp3008_adc(4); // MCP3008 ADC의 4번 채널을 사용하여 Flex Sensor 값을 읽음
		forceValue5 = read_mcp3008_adc(5); // MCP3008 ADC의 5번 채널을 사용하여 Force Sensor 값을 읽음
		forceValue6 = read_mcp3008_adc(6); // MCP3008 ADC의 6번 채널을 사용하여 Force Sensor 값을 읽음
		forceValue7 = read_mcp3008_adc(7); // MCP3008 ADC의 7번 채널을 사용하여 Force Sensor 값을 읽음

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
		pSend_buffer = hand_mode(flexValue0, flexValue1, flexValue2, flexValue3, flexValue4, forceValue5, forceValue6, forceValue7);
		printf("Acceleration: X = %d, Y = %d, Z = %d\n", ax, ay, az);
		printf("Gyroscope: X = %d, Y = %d, Z = %d\n", gx, gy, gz);
		printf("Hand_Mode\n0 : %d\n1 : %d\n2 : %d\n3 : %d\n4 : %d\n5 : %d\n6 : %d\n7 : %d\n", flexValue0, flexValue1, flexValue2, flexValue3, flexValue4, forceValue5, forceValue6, forceValue7);
		write(clnt_sock, pSend_buffer, strlen(pSend_buffer));
		printf("%c, %c\n", pSend_buffer[0]);
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
void error_handling(const char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}