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

	UPROPERTY(EditAnywhere, Category = Variables)
	float Ysensi;

	UPROPERTY(EditAnywhere, Category = Variables)
	float Xsensi;

	void LookCharacter(const FInputActionValue &Value);
	void MoveCharacter(const FInputActionValue &Value);
	void Sprint();
	void StopSprint();

private:
	UPROPERTY(VisibleAnywhere)
	UCameraComponent *ViewCam;

	UPROPERTY(VisibleAnywhere)
	USpringArmComponent *SpringArm;

	UPROPERTY(VisibleAnywhere)
	USpotLightComponent *FlashLight;

	// UPROPERTY(EditAnywhere, Category = "Head Bob")
	// float BobSpeed = 10.0f;

	// UPROPERTY(EditAnywhere, Category = "Head Bob")
	// float BobAmount = 10.0f;
};
