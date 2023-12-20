// AValve.cpp
#include "Valve.h"
#include "Engine/StaticMeshActor.h"
#include "FlickeringLight.h"
#include "Components/AudioComponent.h"

int AValve::ValvesClosed = 0;

AValve::AValve()
    : IsValveCompleted(false), TotalRotation(0.0f)
{
    PrimaryActorTick.bCanEverTick = true;
    StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
    RootComponent = StaticMeshComponent;
    AllValvesClosed = false;

    ValveSound = CreateDefaultSubobject<UAudioComponent>(TEXT("Sound"));
    ValveSound->SetupAttachment(RootComponent);

    isRotating = false;
}

void AValve::BeginPlay()
{
    Super::BeginPlay();
    ValvesClosed = 0;
    ValveSound->Stop();
}

void AValve::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (ValvesClosed == 5)
    {
        AllValvesClosed = true;
    }

    // if (isRotating)
    // {
    //     PlayValveClosingSound();
    // }
    // else if(!isRotating)
    // {
    //     StopValveClosingSound();
    // }
}

void AValve::CloseValve()
{
    ValvesClosed++;
    if (WaterFlows.Num() > 0)
    {
        for (AStaticMeshActor *CurrentWaterFlow : WaterFlows)
        {
            if (CurrentWaterFlow)
            {
                CurrentWaterFlow->SetActorHiddenInGame(true);
                CurrentWaterFlow->SetActorEnableCollision(false);
                RedLight->CloseLight();
            }
        }
    }

    UE_LOG(LogTemp, Warning, TEXT("Valves Closed : %d"), ValvesClosed);
}

void AValve::PlayValveClosingSound()
{
    ValveSound->Play();
    UE_LOG(LogTemp, Warning, TEXT("SoundValve"));
}

void AValve::StopValveClosingSound()
{
    ValveSound->Stop();
    UE_LOG(LogTemp, Warning, TEXT("Nah"));
}
