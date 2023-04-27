// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Windows/AllowWindowsPlatformTypes.h"
#include "Windows/prewindowsapi.h"
#include <WinSock2.h>
#include "CoreMinimal.h"
#include "Networking.h"
#include <iostream>
#include "Windows/PostWindowsApi.h"
#include "Windows/HideWindowsPlatformTypes.h"

#pragma comment (lib , "ws2_32.lib")


/**
 * 
 */
class SOCKETEXAMPLE_API Client_Thread : public FRunnable
{
public:
	Client_Thread();
	~Client_Thread() override;

	bool Init() override;
	uint32 Run() override;
	void Exit() override;

	SOCKET Socket;

private:
	FRunnableThread* Thread;
};
