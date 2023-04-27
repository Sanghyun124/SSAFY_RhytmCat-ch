// Fill out your copyright notice in the Description page of Project Settings.


#include "Client_Thread.h"

Client_Thread::Client_Thread()
{
	Thread = FRunnableThread::Create(this, TEXT("Network Thread"));
}

Client_Thread::~Client_Thread()
{
	if (Thread) {
		//스레드 종료
		Thread->WaitForCompletion();
		Thread->Kill();
		delete Thread;
	}
}

#pragma endregion
void showError(const char * msg)
{
	std::cout << "error : " << msg << std::endl;
	exit(-1);
}

bool Client_Thread::Init()
{
	UE_LOG(LogNet, Warning, TEXT("Thread has been initialized"));

	// Socket 연결을 하는 코드를 넣으면 된다. (너무 길어서 뺐다)
	WSADATA data;
	::WSAStartup(MAKEWORD(2, 2), &data);

	Socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (Socket == INVALID_SOCKET)
		showError("fail generating client");

	sockaddr_in addr = { 0 };

	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr("192.168.100.252");
	addr.sin_port = htons(30020);

	if (connect(Socket, (sockaddr *)&addr, sizeof(addr)) == SOCKET_ERROR)
		showError("fail connection");

	char message[30];
	int str_len = recv(Socket, message, sizeof(message) - 1, 0);
	printf("Message from server : %s\n", message);

	return true;
}

uint32 Client_Thread::Run()
{
	char buffer[10] = "10";
	char end_buffer[10] = "00";
	int b = 0;

	while (1) {
		recv(Socket, buffer, 5, 0);
		if (buffer[0] == '0') {
			break;
		}
		//if (b > 10) {
		//	send(client, end_buffer, strlen(end_buffer), 0);
		//}
		b++;
		printf("%d\n", b);
		UE_LOG(LogTemp, Log, TEXT("%c"), buffer[0]);
	}

	return 0;
}

void Client_Thread::Exit()
{
	if (Socket)
	{
		// Socket 연결을 끊고 Winsock 사용을 종료
		closesocket(Socket);
		::WSACleanup();
	}
}
