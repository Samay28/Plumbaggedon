// Fill out your copyright notice in the Description page of Project Settings.

#include "FlickeringLight.h"
#include "Components/PointLightComponent.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AFlickeringLight::AFlickeringLight()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LightMesh"));
	MeshComponent->SetupAttachment(RootComponent); // Attach the mesh to the root component

	PointLightComponent = CreateDefaultSubobject<UPointLightComponent>(TEXT("PointLight"));
	PointLightComponent->SetupAttachment(MeshComponent);
	MinIntensity = 40.0f;
	MaxIntensity = 500.0f;
	FlickerSpeed = 2.f;
}

// Called when the game starts or when spawned
void AFlickeringLight::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AFlickeringLight::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	float NewIntensity = FMath::Lerp(MinIntensity, MaxIntensity, FMath::Abs(FMath::Sin(FlickerSpeed * GetGameTimeSinceCreation())));
	PointLightComponent->SetIntensity(NewIntensity);
}

void AFlickeringLight::CloseLight()
{
	MaxIntensity = 0;
	MinIntensity = 0;
}
