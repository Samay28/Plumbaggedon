// Fill out your copyright notice in the Description page of Project Settings.

#include "Spray.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
ASpray::ASpray()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Can"));
	MeshComponent->SetupAttachment(RootComponent);

	SpraySmoke = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("SmokeSpray"));
	SpraySmoke->SetupAttachment(MeshComponent);

	SpraySmoke->bAutoActivate = false;
}
void ASpray::ActivateSpray()
{	
	UE_LOG(LogTemp,Warning,TEXT("halp"));
	if (SpraySmoke)
	{
		SpraySmoke->Activate();
	}
}

void ASpray::DeactivateSpray()
{
	if (SpraySmoke)
	{
		SpraySmoke->Deactivate();
	}
}

// Called when the game starts or when spawned
void ASpray::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ASpray::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
