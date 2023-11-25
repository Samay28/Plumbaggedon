// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FloodActor.generated.h"

UCLASS()
class SWOOSH_API AFloodActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFloodActor();
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void TimerCallback();
	void UpdateActorPosition(float DeltaTime);

	float TimerValue;

	struct FTimerHandle TimerHandle;

};
