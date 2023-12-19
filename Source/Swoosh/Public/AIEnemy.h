// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BehaviorTree/BehaviorTree.h"
#include "AIEnemy.generated.h"

UCLASS()
class SWOOSH_API AAIEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AAIEnemy();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent *PlayerInputComponent) override;

	UBehaviorTree *GetBehaviorTree() const;

	UPROPERTY(EditAnywhere);
	class UAudioComponent* EnemySound;
	
	UPROPERTY(EditAnywhere);
	class USoundWave* PatrolSound;

	UPROPERTY(EditAnywhere);
	class USoundWave* RageSound; 

	void SetRageSound();
	void SetPatrolSound();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI)
	UBehaviorTree *Tree;

	UPROPERTY(EditAnywhere, Category = Death)
	class USkeletalMeshComponent *SkeletalMeshComp ;


};
