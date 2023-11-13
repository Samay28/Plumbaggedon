// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Valve.generated.h"

class UStaticMeshComponent; 
UCLASS()
class SWOOSH_API AValve : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AValve();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void CloseValve();
	int ValvesClosed;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* StaticMeshComponent;
};
