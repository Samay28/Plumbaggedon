// AValve.cpp
#include "Valve.h"
#include "Engine/StaticMeshActor.h"
#include "FlickeringLight.h"

int AValve::ValvesClosed = 0;

AValve::AValve()
    : IsValveCompleted(false), TotalRotation(0.0f)
{
    PrimaryActorTick.bCanEverTick = true;
    StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
    RootComponent = StaticMeshComponent;
}

void AValve::BeginPlay()
{
    Super::BeginPlay();
    ValvesClosed = 0;
}

void AValve::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // Your tick logic here
}

void AValve::CloseValve()
{
    ValvesClosed++;
    if (WaterFlows.Num() > 0)
    {
        // Iterate through each element in the WaterFlows array
        for (AStaticMeshActor *CurrentWaterFlow : WaterFlows)
        {
            // Check if the current element is valid
            if (CurrentWaterFlow)
            {
                // Deactivate the static mesh actor by setting its visibility to false
                CurrentWaterFlow->SetActorHiddenInGame(true);
                CurrentWaterFlow->SetActorEnableCollision(false);
                RedLight->CloseLight();
            }
        }
    }

    UE_LOG(LogTemp, Warning, TEXT("Valves Closed : %d"), ValvesClosed);
}
