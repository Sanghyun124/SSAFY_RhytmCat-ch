#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <wiringPi.h>
#include <wiringPiSPI.h>

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
    // 소켓을 생성
    int serv_sock=socket(PF_INET, SOCK_STREAM, 0);
    if(serv_sock == -1)
        error_handling("socket() error");
	
    // 주소 정보 초기화
    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family=AF_INET;
    // INADDR_ANY는 서버 자신의 IP주소
    serv_addr.sin_addr.s_addr=htonl(INADDR_ANY);
    serv_addr.sin_port=htons(30020);
	
    // 소켓에 위에서 생성한 주소 정보를 할당
    if(bind(serv_sock, (struct sockaddr*) &serv_addr, sizeof(serv_addr))==-1 )
        error_handling("bind() error"); 
	
    // 클라이언트가 연결요청이 가능하도록 5크기의 대기실 생성
    if(listen(serv_sock, 5)==-1)
        error_handling("listen() error");
	
    // accept()함수호출을 해서 실제 데이터를 보낼 수 있는 소켓을 생성
    socklen_t clnt_addr_size=sizeof(serv_addr);
    int clnt_sock=accept(serv_sock, (struct sockaddr*)&serv_addr,&clnt_addr_size);
    if(clnt_sock==-1)
        error_handling("accept() error");  
    
    // 메시지를 보낸다
    char message[]="Hello UE!";
    write(clnt_sock, message, sizeof(message));
    // 서버, 클라이언트 소켓의 연결을 해제
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
        flexValue0 = read_mcp3008_adc(0); // MCP3008 ADC의 0번 채널을 사용하여 Flex Sensor 값을 읽음
        flexValue1 = read_mcp3008_adc(1); // MCP3008 ADC의 1번 채널을 사용하여 Flex Sensor 값을 읽음
        flexValue2 = read_mcp3008_adc(2); // MCP3008 ADC의 2번 채널을 사용하여 Flex Sensor 값을 읽음
        flexValue3 = read_mcp3008_adc(3); // MCP3008 ADC의 3번 채널을 사용하여 Flex Sensor 값을 읽음
        flexValue4 = read_mcp3008_adc(4); // MCP3008 ADC의 4번 채널을 사용하여 Flex Sensor 값을 읽음
        forceValue5 = read_mcp3008_adc(5); // MCP3008 ADC의 5번 채널을 사용하여 Force Sensor 값을 읽음
        forceValue6 = read_mcp3008_adc(6); // MCP3008 ADC의 6번 채널을 사용하여 Force Sensor 값을 읽음
        forceValue7 = read_mcp3008_adc(7); // MCP3008 ADC의 7번 채널을 사용하여 Force Sensor 값을 읽음

        pSend_buffer = hand_mode(flexValue0,flexValue1,flexValue2,flexValue3,flexValue4,forceValue5,forceValue6,forceValue7);
        //send_buffer=pSend_buffer;

	    write(clnt_sock,pSend_buffer,strlen(pSend_buffer));
	    printf("%c, %c\n",pSend_buffer[0],pSend_buffer[1]);
	    b++;
        if(b>10){
            write(clnt_sock,end_buffer,strlen(end_buffer));
	    break;
        }
	    sleep(1);
    }
    close(clnt_sock);	
    close(serv_sock);
    return 0;
}

void error_handling(const char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}
