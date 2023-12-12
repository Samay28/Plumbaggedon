#include "BTTask_FindPlayerLocation.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

UBTTask_FindPlayerLocation::UBTTask_FindPlayerLocation(FObjectInitializer const &ObjectInitializer)
{
    NodeName = TEXT("Find Player Location");
}
EBTNodeResult::Type UBTTask_FindPlayerLocation::ExecuteTask(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory)
{
    // GET PLAYER CHARACTER
    if (auto *const Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))
    {
        // get player location to use as an origin
        auto const PlayerLocation = Player->GetActorLocation();
        if (SearchRandom)
        {
            FNavLocation loc;

            // get nav system and generate random location near player
            if (auto *const NavSys = UNavigationSystemV1::GetCurrent(GetWorld()))
            {
                // try to get a random loc near player
                if (NavSys->GetRandomPointInNavigableRadius(PlayerLocation, SearchRadius, loc))
                {
                    OwnerComp.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), loc.Location);
                    FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
                    return EBTNodeResult::Succeeded;
                }
            }
        }
        else
        {
            OwnerComp.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), PlayerLocation);
            FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
            return EBTNodeResult::Succeeded;
        }
    }
    return EBTNodeResult::Failed;
}