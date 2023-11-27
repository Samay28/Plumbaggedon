// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FlickeringLight.generated.h"

UCLASS()
class SWOOSH_API AFlickeringLight : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AFlickeringLight();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void CloseLight();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	class UPointLightComponent *PointLightComponent;

	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent *MeshComponent;

	// Variables for flickering effect
	float MinIntensity;
	float MaxIntensity;
	float FlickerSpeed;

private:
};
