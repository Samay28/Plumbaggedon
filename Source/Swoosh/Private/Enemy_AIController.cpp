// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy_AIController.h"
#include "AIEnemy.h"

AEnemy_AIController::AEnemy_AIController(FObjectInitializer const& ObjectInitializer)
{

}
void AEnemy_AIController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);
    if(AAIEnemy* const bug = Cast<AAIEnemy>(InPawn))
    {
        if(UBehaviorTree* const Tree = bug->GetBehaviorTree())
        {
            UBlackboardComponent* b;
            UseBlackboard(Tree->BlackboardAsset, b);
            Blackboard = b;
            RunBehaviorTree(Tree);
        }
    }
}
