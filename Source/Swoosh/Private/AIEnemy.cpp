// Fill out your copyright notice in the Description page of Project Settings.

#include "AIEnemy.h"
#include "Plumber.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
// Sets default values
AAIEnemy::AAIEnemy()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AAIEnemy::BeginPlay()
{
	Super::BeginPlay();
	// UCapsuleComponent *CC = FindComponentByClass<UCapsuleComponent>();
	// if (CC)
	// {
	// 	// Bind the OnComponentBeginOverlap event
	// 	CC->OnComponentHit.AddDynamic(this, &AAIEnemy::OnCapsuleHit);
	// }
	// else
	// {
	// 	// Log an error if the capsule component is not found
	// 	UE_LOG(LogTemp, Error, TEXT("Capsule component not found in EnemyController"));
	// }
	// SkeletalMeshComp = FindComponentByClass<USkeletalMeshComponent>();
	// if (SkeletalMeshComp)
	// {
	// 	UE_LOG(LogTemp, Warning, TEXT("gm"));
	// }
}
// void AAIEnemy::OnCapsuleHit(UPrimitiveComponent *HitComponent, AActor *OtherActor,
// 							UPrimitiveComponent *OtherComp, FVector NormalImpulse,
// 							const FHitResult &Hit)
// {
// 	// Check if the other actor has Plumber.cpp as its class
// 	if (OtherActor && OtherActor->IsA(APlumber::StaticClass()))
// 	{
// 		// Log a message
// 		UE_LOG(LogTemp, Warning, TEXT("Capsule hit an actor of class Plumber"));
// 	}
// }



// Called every frame
void AAIEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AAIEnemy::SetupPlayerInputComponent(UInputComponent *PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

UBehaviorTree *AAIEnemy::GetBehaviorTree() const
{
	return Tree;
}
