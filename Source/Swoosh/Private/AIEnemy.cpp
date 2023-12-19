// Fill out your copyright notice in the Description page of Project Settings.

#include "AIEnemy.h"
#include "Plumber.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundWave.h"
// Sets default values
AAIEnemy::AAIEnemy()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	EnemySound = CreateDefaultSubobject<UAudioComponent>(TEXT("Sound"));
	EnemySound->SetupAttachment(RootComponent);
}

void AAIEnemy::SetRageSound()
{
	EnemySound->SetSound(RageSound);
}

void AAIEnemy::SetPatrolSound()
{
	EnemySound->SetSound(PatrolSound);
}

// Called when the game starts or when spawned
void AAIEnemy::BeginPlay()
{
	Super::BeginPlay();
	EnemySound->bAlwaysPlay = true;
	// static ConstructorHelpers::FObjectFinder<USoundCue> SoundCueAsset(TEXT("/Game/Sounds/Enemy/flap_chase-1"));
	// if (SoundCueAsset.Succeeded())
	// {
	// 	EnemySound->SetSound(SoundCueAsset.Object);
	// }
}

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
