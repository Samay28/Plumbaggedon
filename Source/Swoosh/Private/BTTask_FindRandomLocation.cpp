// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_FindRandomLocation.h"
#include "NavigationSystem.h"
#include "Enemy_AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

UBTTask_FindRandomLocation::UBTTask_FindRandomLocation(FObjectInitializer const &ObjectInitializer)
{
    NodeName = "Find Random Location In NavMesh";
}
EBTNodeResult::Type UBTTask_FindRandomLocation::ExecuteTask(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory)
{
    // get AI controller and enemy
    if (auto *const cont = Cast<AEnemy_AIController>(OwnerComp.GetAIOwner()))
    {
        if (auto *const enemy = cont->GetPawn())
        {
            if (auto *const Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))
            {
                // obtain Player location to use as an origin
                // auto const Origin = enemy->GetActorLocation();
                auto const PlayerLocation = Player->GetActorLocation();

                // get nav and generate random location
                if (auto *const NavSys = UNavigationSystemV1::GetCurrent(GetWorld()))
                {
                    FNavLocation loc;
                    if (NavSys->GetRandomPointInNavigableRadius(PlayerLocation, SearchRadius, loc))
                    {
                        OwnerComp.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), loc.Location);
                    }

                    // finish with success
                    FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
                    return EBTNodeResult::Succeeded;
                }
            }
        }
    }
    return EBTNodeResult::Failed;
}