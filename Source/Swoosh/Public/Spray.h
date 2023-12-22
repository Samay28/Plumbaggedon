// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Spray.generated.h"

UCLASS()
class SWOOSH_API ASpray : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASpray();
	void ActivateSpray();
	void DeactivateSpray();

	UPROPERTY(BlueprintReadOnly)
	float FuelSpray;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent *MeshComponent;

	UPROPERTY(VisibleAnywhere)
    class UParticleSystemComponent* SpraySmoke;

	UPROPERTY(VisibleAnywhere)
	class UBoxComponent* SprayHitBox;

	UFUNCTION()
	void OnCollisionBegin(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);

	UPROPERTY(EditAnywhere)
	class UAudioComponent* SpraySound;

};
