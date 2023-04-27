// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <iostream>
#include "GameFramework/Actor.h"
#include "Client_Thread.h"
#include "SocketActor.generated.h"

#pragma comment (lib , "ws2_32.lib")

UCLASS()
class SOCKETEXAMPLE_API ASocketActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASocketActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
