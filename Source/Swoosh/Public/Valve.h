
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

	// Function to get the instance of the valve
	static AValve *GetInstance();

	void CloseValve();
	int ValvesClosed;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly)
	bool IsValveCompleted;

	static AValve *MyInstance;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly)
	float TotalRotation;

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent *StaticMeshComponent;

	// APlumber* InteractingPlumber;

	// The single instance of the valve
};