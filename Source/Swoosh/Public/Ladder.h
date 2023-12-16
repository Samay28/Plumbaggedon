// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Ladder.generated.h"

UCLASS()
class SWOOSH_API ALadder : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALadder();
	void MoveDownwards();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	class UBoxComponent *Collider;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
