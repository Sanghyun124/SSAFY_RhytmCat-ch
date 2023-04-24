// ������ ����ϱ� ���ؼ� ���̺귯�� �����ؾ� �Ѵ�.
#pragma comment(lib, "ws2_32")
// inet_ntoa�� deprecated�� �Ǿ��µ�.. ����Ϸ��� �Ʒ� ������ �ؾ� �Ѵ�.
#pragma warning(disable:4996)
#include <stdio.h>
#include <iostream>
#include <vector>
#include <thread>
// ������ ����ϱ� ���� ���̺귯��
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



	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) //���� ���̺귯�� �ʱ�ȭ

		ErrorHandling("WSAStartup() error!");



	hServSock = socket(PF_INET, SOCK_STREAM, 0); //���ϻ���

	if (hServSock == INVALID_SOCKET)

		ErrorHandling("socket() error");



	memset(&servAddr, 0, sizeof(servAddr));

	servAddr.sin_family = AF_INET;

	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	servAddr.sin_port = htons(8989);



	if (::bind(hServSock, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR) //���Ͽ� IP�ּҿ� PORT ��ȣ �Ҵ�

		ErrorHandling("bind() error");



	if (listen(hServSock, 5) == SOCKET_ERROR) //listen �Լ�ȣ���� ���ؼ� ������ ������ ���� �������� �ϼ�

		ErrorHandling("listen() error");



	szClntAddr = sizeof(clntAddr);

	hClntSock = accept(hServSock, (SOCKADDR*)&clntAddr, &szClntAddr); //Ŭ���̾�Ʈ �����û �����ϱ� ���� accept�Լ� ȣ��

	if (hClntSock == INVALID_SOCKET)

		ErrorHandling("accept() error");



	send(hClntSock, message, sizeof(message), 0); //send�Լ� ȣ���� ���ؼ� ����� Ŭ���̾�Ʈ�� �����͸� ����
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

	WSACleanup(); //���α׷� ���� ���� �ʱ�ȭ�� ���� ���̺귯�� ����

	return 0;

}



void ErrorHandling(const char *message)

{

	fputs(message, stderr);

	fputc('\n', stderr);

	exit(1);

}