// 소켓을 사용하기 위해서 라이브러리 참조해야 한다.
#pragma comment(lib, "ws2_32")
// inet_ntoa가 deprecated가 되었는데.. 사용하려면 아래 설정을 해야 한다.
#pragma warning(disable:4996)
#include <stdio.h>
#include <iostream>
#include <vector>
#include <thread>
// 소켓을 사용하기 위한 라이브러리
#include <WinSock2.h>

using namespace std;
void ErrorHandling(const char *message);



int main()

{

	WSADATA wsaData;

	SOCKET hServSock, hClntSock;

	SOCKADDR_IN servAddr, clntAddr;



	int szClntAddr;

	char message[] = "Hello World!";



	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) //소켓 라이브러리 초기화

		ErrorHandling("WSAStartup() error!");



	hServSock = socket(PF_INET, SOCK_STREAM, 0); //소켓생성

	if (hServSock == INVALID_SOCKET)

		ErrorHandling("socket() error");



	memset(&servAddr, 0, sizeof(servAddr));

	servAddr.sin_family = AF_INET;

	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	servAddr.sin_port = htons(8989);



	if (::bind(hServSock, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR) //소켓에 IP주소와 PORT 번호 할당

		ErrorHandling("bind() error");



	if (listen(hServSock, 5) == SOCKET_ERROR) //listen 함수호출을 통해서 생성한 소켓을 서버 소켓으로 완성

		ErrorHandling("listen() error");



	szClntAddr = sizeof(clntAddr);

	hClntSock = accept(hServSock, (SOCKADDR*)&clntAddr, &szClntAddr); //클라이언트 연결요청 수락하기 위해 accept함수 호출

	if (hClntSock == INVALID_SOCKET)

		ErrorHandling("accept() error");



	send(hClntSock, message, sizeof(message), 0); //send함수 호출을 통해서 연결된 클라이언트에 데이터를 전송
	char buffer[2] = { 0 };
	while (1) {
		recv(hClntSock, buffer, 2, 0);
		if (buffer[0] == 0) {
			break;
		}
		cout << buffer;
		Sleep(50);
	}

	closesocket(hClntSock);

	closesocket(hServSock);

	WSACleanup(); //프로그램 종료 전에 초기화한 소켓 라이브러리 해제

	return 0;

}



void ErrorHandling(const char *message)

{

	fputs(message, stderr);

	fputc('\n', stderr);

	exit(1);

}