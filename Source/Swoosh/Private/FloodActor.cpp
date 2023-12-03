#include "FloodActor.h"
#include "TimerManager.h"
#include "Valve.h"
#include "Plumber.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/StaticMeshActor.h"

// Sets default values
AFloodActor::AFloodActor()
{

	PrimaryActorTick.bCanEverTick = true;
	TimerValue = 1200.0f;
}

void AFloodActor::BeginPlay()
{
	Super::BeginPlay();

	const FName ActorTag = FName("WaterFlood");
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), ActorTag, WaterMeshes);
	//
}

// Called every frame
void AFloodActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// UE_LOG(LogTemp, Warning, TEXT("Current Time: %f"), GetWorldTimerManager().GetTimerElapsed(TimerHandle));

	if (Valves->ValvesClosed < 5 && Player->CanStartGame)
	{
		UpdateActorPosition(DeltaTime);
	}
}

// Timer callback function
// void AFloodActor::TimerCallback()
// {
// 	GetWorldTimerManager().ClearTimer(TimerHandle);
// }

void AFloodActor::UpdateActorPosition(float DeltaTime)
{
	float MovePerSecond = 170.f / 1200.f;

	// FVector NewLocation = GetActorLocation();
	// NewLocation.Z += MovePerSecond * DeltaTime;

	// NewLocation.Z = FMath::Clamp(NewLocation.Z, -127.0f, 40.0f);
	// SetActorLocation(NewLocation);

	for (AActor*  StaticMesh : WaterMeshes)
	{	
		AStaticMeshActor* StaticMeshActor = Cast<AStaticMeshActor>(StaticMesh);
		if (StaticMeshActor) // Check if the current static mesh actor is valid
		{
			FVector NewLocation = StaticMesh->GetActorLocation();
			NewLocation.Z += MovePerSecond * DeltaTime;

			NewLocation.Z = FMath::Clamp(NewLocation.Z, -127.0f, 40.0f);

			StaticMesh->SetActorLocation(NewLocation);
			UE_LOG(LogTemp,Warning,TEXT("chl gya"));
		}
	}
}
