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
	SprayHitBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ASpray::ActivateSpray()
{
	if (SpraySmoke && FuelSpray>0)
	{
		SprayHitBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		SpraySmoke->Activate();
		FuelSpray--;
	}
}

void ASpray::DeactivateSpray()
{
	if (SpraySmoke)
	{
		SprayHitBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		SpraySmoke->Deactivate();
		// FuelSpray++;
		// UE_LOG(LogTemp,Warning,TEXT("Fuel : %f"), FuelSpray);
	}
}

void ASpray::BeginPlay()
{
	Super::BeginPlay();
	FuelSpray = 3000.f;
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
	FuelSpray = FMath::Clamp(FuelSpray, 0, 3000);
}

void ASpray::OnCollisionBegin(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	if (OtherActor && OtherActor->ActorHasTag("Enemy"))
	{
		AController *OtherController = Cast<APawn>(OtherActor) ? Cast<APawn>(OtherActor)->GetController() : Cast<AController>(OtherActor);
		AEnemy_AIController *EnemyController = Cast<AEnemy_AIController>(OtherController);

		if (EnemyController && !EnemyController->isDead)
		{
			EnemyController->Death();
			EnemyController->isDead = true;
		}
	}
}
