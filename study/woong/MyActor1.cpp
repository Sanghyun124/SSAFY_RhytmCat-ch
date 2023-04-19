// Fill out your copyright notice in the Description page of Project Settings.


#include "MyActor1.h"

// Sets default values
AMyActor1::AMyActor1()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create the skeletal mesh component and set it as the root component
	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	RootComponent = SkeletalMesh;

	// Load the mesh and set it as the skeletal mesh for the pawn
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> MeshObject(TEXT("/Game/Path/To/YourMesh.YourMesh"));
	if (MeshObject.Succeeded())
	{
		SkeletalMesh->SetSkeletalMesh(MeshObject.Object);
	}


}

// Called when the game starts or when spawned
void AMyActor1::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMyActor1::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

