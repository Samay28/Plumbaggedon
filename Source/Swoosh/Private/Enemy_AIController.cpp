// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemy_AIController.h"
#include "Plumber.h"
#include "AIEnemy.h"
#include "Perception/AIPerceptionComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "GameFramework/CharacterMovementComponent.h"

AEnemy_AIController::AEnemy_AIController(FObjectInitializer const &ObjectInitializer)
{
    SetUpPerceptionSystem();
}
void AEnemy_AIController::OnPossess(APawn *InPawn)
{
    Super::OnPossess(InPawn);
    if (AAIEnemy *const bug = Cast<AAIEnemy>(InPawn))
    {
        if (UBehaviorTree *const Tree = bug->GetBehaviorTree())
        {
            UBlackboardComponent *b;
            UseBlackboard(Tree->BlackboardAsset, b);
            Blackboard = b;
            RunBehaviorTree(Tree);
        }
    }
}

void AEnemy_AIController::SetUpPerceptionSystem()
{
    SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
    if (SightConfig)
    {
        SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception Component")));
        SightConfig->SightRadius = 500.0f;
        SightConfig->LoseSightRadius = SightConfig->SightRadius + 25.f;
        SightConfig->PeripheralVisionAngleDegrees = 90.0f;
        SightConfig->SetMaxAge(5.f);
        SightConfig->AutoSuccessRangeFromLastSeenLocation = 520.0f;
        SightConfig->DetectionByAffiliation.bDetectEnemies = true;
        SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
        SightConfig->DetectionByAffiliation.bDetectNeutrals = true;

        GetPerceptionComponent()->SetDominantSense(*SightConfig->GetSenseImplementation());
        GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &AEnemy_AIController::OnTargetDetected);
        GetPerceptionComponent()->ConfigureSense(*SightConfig);
    }
}

void AEnemy_AIController::OnTargetDetected(AActor *Actor, FAIStimulus const Stimulus)
{
    bool CanSeePlayer;
    if (auto *const ch = Cast<APlumber>(Actor))
    {
        CanSeePlayer = Stimulus.WasSuccessfullySensed();
        UE_LOG(LogTemp, Warning, TEXT("Player detected. CanSeePlayer: %s"), CanSeePlayer ? TEXT("true") : TEXT("false"));

        GetBlackboardComponent()->SetValueAsBool("CanSeePlayer", CanSeePlayer);

        if (CanSeePlayer)
        {
            if (ACharacter *Enemy = GetCharacter())
            {
                if (UCharacterMovementComponent *MovementComp = Enemy->GetCharacterMovement())
                {
                    MovementComp->MaxWalkSpeed = 500.0f;
                }
            }
        }
        else
        {
            if (ACharacter *Enemy = GetCharacter())
            {
                if (UCharacterMovementComponent *MovementComp = Enemy->GetCharacterMovement())
                {
                    MovementComp->MaxWalkSpeed = 250.0f;
                }
            }
        }
    }
}
