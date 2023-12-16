// Fill out your copyright notice in the Description page of Project Settings.

#include "Ladder.h"
#include "Components/BoxComponent.h"
#include "TimerManager.h"

// Sets default values
ALadder::ALadder()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	// Collider = CreateDefaultSubobject<UBoxComponent>(TEXT("Collider"));
	// Collider->SetupAttachment(RootComponent);
}

void ALadder::MoveActorDownwards()
{
	const float TargetZ = -40.0f;
	const float Distance = FMath::Abs(TargetZ - GetActorLocation().Z); // Distance calculation
	const float TimeToMove = 1.5f;

	// Calculate the downward movement for each frame
	const float DeltaMove = Distance / (TimeToMove / 0.01f);

	// Clear any existing timers before setting a new one
	GetWorld()->GetTimerManager().ClearTimer(DownwardMoveTimerHandle);

	// Schedule a callback to move the ladder downwards every frame
	GetWorld()->GetTimerManager().SetTimer(DownwardMoveTimerHandle, this, &ALadder::MoveDownwardCallback, 0.01f, true);
}

// Callback function for the downward movement
void ALadder::MoveDownwardCallback()
{
	const float TargetZ = -40.0f;
	const float DeltaMove = 0.5f; // You can adjust this value based on your desired speed

	// Calculate the new location based on the current location
	FVector NewLocation = GetActorLocation() - FVector(0.0f, 0.0f, DeltaMove);

	// Set the new location
	SetActorLocation(NewLocation);

	// Check if the ladder has moved to the target Z location
	if (GetActorLocation().Z <= TargetZ)
	{
		// Stop the downward movement
		GetWorld()->GetTimerManager().ClearTimer(DownwardMoveTimerHandle);
		UE_LOG(LogTemp, Warning, TEXT("Downward movement stopped."));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Called"));
	}
}
// Called when the game starts or when spawned
void ALadder::BeginPlay()
{
	Super::BeginPlay();
	MoveActorDownwards();
}

// Called every frame
void ALadder::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
