// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_ChasePlayer.h"
#include "Enemy_AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"

UBTTask_ChasePlayer::UBTTask_ChasePlayer(FObjectInitializer const &ObjectInitializer)
{
    NodeName = TEXT("Chase Player");
}
EBTNodeResult::Type UBTTask_ChasePlayer::ExecuteTask(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory)
{
    // get target location from blackboard
    if (auto *const cont = Cast<AEnemy_AIController>(OwnerComp.GetAIOwner()))
    {
        auto const PlayerLocation = OwnerComp.GetBlackboardComponent()->GetValueAsVector(GetSelectedBlackboardKey());

        // move to playerlocation
        UAIBlueprintHelperLibrary::SimpleMoveToLocation(cont, PlayerLocation);

        FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
        return EBTNodeResult::Succeeded;
    }
    return EBTNodeResult::Failed;
}