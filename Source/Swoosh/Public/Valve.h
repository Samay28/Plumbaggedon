// AValve.h
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Plumber.h"
#include "Valve.generated.h"

class UStaticMeshComponent;
class AStaticMeshActor;
UCLASS()
class SWOOSH_API AValve : public AActor
{
    GENERATED_BODY()

public:
    AValve();

    virtual void Tick(float DeltaTime) override;

    void CloseValve();
    void PlayValveClosingSound();
    void StopValveClosingSound();

    static int ValvesClosed;

    UPROPERTY(EditInstanceOnly, BlueprintReadOnly)
    bool IsValveCompleted;

    UPROPERTY(EditInstanceOnly, BlueprintReadOnly)
    float TotalRotation;

    UPROPERTY(EditInstanceOnly, BlueprintReadOnly)
    TArray<AStaticMeshActor *> WaterFlows;

    UPROPERTY(BlueprintReadOnly)
    bool AllValvesClosed;

    bool isRotating;

protected:
    virtual void BeginPlay() override;

    UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = Light)
    class AFlickeringLight *RedLight;

    UPROPERTY(EditAnywhere)
    class UAudioComponent* ValveSound;

private:
    UPROPERTY(VisibleAnywhere)
    UStaticMeshComponent *StaticMeshComponent;
};
