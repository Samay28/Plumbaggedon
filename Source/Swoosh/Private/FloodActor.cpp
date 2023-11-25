#include "FloodActor.h"
#include "TimerManager.h"
#include "Engine/World.h"

// Sets default values
AFloodActor::AFloodActor()
{

	PrimaryActorTick.bCanEverTick = true;
	TimerValue = 1200.0f;

}

void AFloodActor::BeginPlay()
{
	Super::BeginPlay();

	GetWorldTimerManager().SetTimer(TimerHandle, this, &AFloodActor::TimerCallback, TimerValue, true);
}

// Called every frame
void AFloodActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UE_LOG(LogTemp, Warning, TEXT("Current Time: %f"), GetWorldTimerManager().GetTimerElapsed(TimerHandle));
	UpdateActorPosition(DeltaTime);
}

// Timer callback function
void AFloodActor::TimerCallback()
{

	GetWorldTimerManager().ClearTimer(TimerHandle);
}

void AFloodActor::UpdateActorPosition(float DeltaTime)
{
	float MovePerSecond = 41.f/20.f;

	FVector NewLocation = GetActorLocation();
	NewLocation.Z += MovePerSecond*DeltaTime;

	NewLocation.Z = FMath::Clamp(NewLocation.Z, -127.0f, -86.0f);
	SetActorLocation(NewLocation);
	
}
