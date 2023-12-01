// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "Plumber.generated.h"

class UInputMappingContext;
class UInputAction;
class FInput;
class UCameraComponent;
class USpringArmComponent;
class USpotLightComponent;
class UWidgetComponent;
class AValve;
struct FTimerHandle;

UCLASS()
class SWOOSH_API APlumber : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlumber();

	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent *PlayerInputComponent) override;

	AValve *Valve;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool CanStartGame;

	// UPROPERTY(EditAnywhere, BlueprintReadWrite)
	// bool StartButtonPressed;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputMappingContext *PlumberMappingContext;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction *LookAction;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction *MoveAction;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction *SprintAction;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction *InteractAction;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction *FireAction;

	UPROPERTY(EditAnywhere, Category = Variables)
	float Ysensi;

	UPROPERTY(EditAnywhere, Category = Variables)
	float Xsensi;

	UPROPERTY(EditDefaultsOnly, Category = Widgets)
	TArray<TSubclassOf<UUserWidget>> WidgetClasses;

	void LookCharacter(const FInputActionValue &Value);
	void MoveCharacter(const FInputActionValue &Value);
	void Sprint();
	void Interact();
	void StopInteract();
	void StartInteract();
	void StopSprint();
	void Fire();
	void StopFire();
	void EnableInputFunction();
	FTimerHandle InteractTimerHandle;
	FTimerHandle TimerHandle_EnableInput;

	// ai sensing
	class UAIPerceptionStimuliSourceComponent *StimulusSource;
	class UChildActorComponent *SprayActor;
	void SetupStimulusSource();

	//buttons
	UFUNCTION(BlueprintCallable)
	void StartGameFunctions();

	// UPROPERTY(EditAnywhere)
	// class ULevelSequencePlayer* StartScene;

	// UPROPERTY(BlueprintReadWrite)
	// bool Can

private:
	UPROPERTY(VisibleAnywhere)
	UCameraComponent *ViewCam;

	UPROPERTY(VisibleAnywhere)
	USpringArmComponent *SpringArm;

	UPROPERTY(VisibleAnywhere)
	USpotLightComponent *FlashLight;

	UPROPERTY(EditAnywhere)
	UUserWidget *MainUI;

	UPROPERTY(EditAnywhere)
	UUserWidget *MainMenuUI;


	int count;

	class AValve *ValveActor;
	class ASpray *SprayCan;
};
