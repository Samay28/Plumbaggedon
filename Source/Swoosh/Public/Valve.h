// AValve.h
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Plumber.h"
#include "Valve.generated.h"

class UStaticMeshComponent;

UCLASS()
class SWOOSH_API AValve : public AActor
{
    GENERATED_BODY()

public:
    AValve();

    virtual void Tick(float DeltaTime) override;

    void CloseValve();
    static int ValvesClosed;

    UPROPERTY(EditInstanceOnly, BlueprintReadOnly)
    bool IsValveCompleted;

    UPROPERTY(EditInstanceOnly, BlueprintReadOnly)
    float TotalRotation;

protected:
    virtual void BeginPlay() override;

private:
    UPROPERTY(VisibleAnywhere)
    UStaticMeshComponent *StaticMeshComponent;
};
