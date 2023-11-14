// Fill out your copyright notice in the Description page of Project Settings.

#include "Valve.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
// Sets default values
AValve::AValve()
{
	// Set this actor to call Tick() every frame. You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create a static mesh component
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	RootComponent = StaticMeshComponent;

	// Ensure that StaticMeshComponent is valid
	if (!ensure(StaticMeshComponent))
	{
		UE_LOG(LogTemp, Error, TEXT("StaticMeshComponent creation failed in AValve constructor."));
		return;
	}
	IsValveCompleted = false;
}


// Called when the game starts or when spawned
void AValve::BeginPlay()
{
	Super::BeginPlay();
	ValvesClosed = 0;
}

// Called every frame
void AValve::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AValve::CloseValve()
{
	ValvesClosed++;
	IsValveCompleted = true;
}

