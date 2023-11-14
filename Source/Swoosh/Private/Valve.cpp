#include "Valve.h"

AValve *AValve::MyInstance = nullptr;

AValve::AValve()
	: IsValveCompleted(false)
{
	PrimaryActorTick.bCanEverTick = true;
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	RootComponent = StaticMeshComponent;
	
	
}

void AValve::BeginPlay()
{
	Super::BeginPlay();

	// Initialize the instance when the valve begins play
	if (MyInstance == nullptr)
	{
		MyInstance = this;
	}
	IsValveCompleted = false;
	TotalRotation = 0;
}

AValve *AValve::GetInstance()
{
	return MyInstance;
}

// void AValve::SetInteractingPlumber(APlumber *InPlumber)
// {
// 	InteractingPlumber = InPlumber;
// }

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