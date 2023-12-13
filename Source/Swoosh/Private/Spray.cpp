#include "Spray.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/BoxComponent.h"
#include "Misc/PackageName.h"
#include "Enemy_AIController.h"

ASpray::ASpray()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Can"));
	MeshComponent->SetupAttachment(RootComponent);

	SprayHitBox = CreateDefaultSubobject<UBoxComponent>(TEXT("HitBox"));
	SprayHitBox->SetupAttachment(MeshComponent);
}

void ASpray::ActivateSpray()
{
	if (SpraySmoke)
	{
		SprayHitBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		SpraySmoke->Activate();
	}
}

void ASpray::DeactivateSpray()
{
	if (SpraySmoke)
	{
		SprayHitBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		SpraySmoke->Deactivate();
	}
}

void ASpray::BeginPlay()
{
	Super::BeginPlay();

	// Move the FindComponentByClass call to BeginPlay
	SpraySmoke = FindComponentByClass<UParticleSystemComponent>();

	if (SpraySmoke)
	{

		SpraySmoke->Deactivate();

		UE_LOG(LogTemp, Warning, TEXT("Cascade Particle System component found on %s"), *GetName());
	}
	else
	{
		// The Cascade Particle System component is not found, handle accordingly
		UE_LOG(LogTemp, Warning, TEXT("Cascade Particle System component not found on %s"), *GetName());
	}
	SprayHitBox->OnComponentBeginOverlap.AddDynamic(this, &ASpray::OnCollisionBegin);
}

void ASpray::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASpray::OnCollisionBegin(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	if (OtherActor && OtherActor->ActorHasTag("Enemy"))
	{
		AController *OtherController = nullptr;

		// Try to get the controller from the other actor
		APawn *OtherPawn = Cast<APawn>(OtherActor);
		if (OtherPawn)
		{
			UE_LOG(LogTemp, Warning, TEXT("cHAL GYAA1"));
			OtherController = OtherPawn->GetController();
		}
		else
		{
			// If it's not a pawn, try to get the controller directly
			OtherController = Cast<AController>(OtherActor);
		}

		AEnemy_AIController *EnemyController = Cast<AEnemy_AIController>(OtherController);
		if (EnemyController)
		{
			EnemyController->Death();
			UE_LOG(LogTemp, Warning, TEXT("cHAL GYAA"));
		}
	}
}
