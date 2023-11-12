// Fill out your copyright notice in the Description page of Project Settings.

#include "Valve.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AValve::AValve()
{
	// Set this actor to call Tick() every frame. You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create a static mesh component
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	RootComponent = StaticMeshComponent;

	// Set up the static mesh or other necessary properties for collision
	// StaticMeshComponent->SetStaticMesh(YourMeshAsset); // Uncomment and replace with your mesh asset
	// StaticMeshComponent->SetCollisionProfileName("YourCollisionProfile"); // Uncomment and replace with your collision profile
}

// Called when the game starts or when spawned
void AValve::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AValve::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
