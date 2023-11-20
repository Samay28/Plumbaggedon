// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "Enemy_AIController.generated.h"

class UAISenseConfig_Sight;

UCLASS()
class SWOOSH_API AEnemy_AIController : public AAIController
{
	GENERATED_BODY()

public:
	explicit AEnemy_AIController(FObjectInitializer const& ObjectInitializer);

protected:

	virtual void OnPossess(APawn* InPawn) override;
	UAISenseConfig_Sight* SightConfig;

	void SetUpPerceptionSystem();

	UFUNCTION()
	void OnTargetDetected(AActor* Actor, FAIStimulus const Stimulus);
};
