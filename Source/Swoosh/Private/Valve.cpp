// AValve.cpp
#include "Valve.h"

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
    UE_LOG(LogTemp, Warning, TEXT("Valves Closed : %d"), ValvesClosed);
}
