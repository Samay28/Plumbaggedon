// Fill out your copyright notice in the Description page of Project Settings.

#include "Plumber.h"
#include "Camera/CameraComponent.h"
#include "Components/SpotLightComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"

// Sets default values
APlumber::APlumber()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpingArm"));
	SpringArm->SetupAttachment(GetRootComponent());
	SpringArm->TargetArmLength = 0;

	ViewCam = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	ViewCam->SetupAttachment(SpringArm);

	FlashLight = CreateDefaultSubobject<USpotLightComponent>(TEXT("FlashLight"));
	FlashLight->SetupAttachment(ViewCam);
}

// Called when the game starts or when spawned
void APlumber::BeginPlay()
{
	Super::BeginPlay();

	if (APlayerController *PlayerController = Cast<APlayerController>(GetController()))
	{
		UEnhancedInputLocalPlayerSubsystem *Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());

		if (Subsystem)
		{
			Subsystem->AddMappingContext(PlumberMappingContext, 0);
		}
	}
}

// Called every frame
void APlumber::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void APlumber::SetupPlayerInputComponent(UInputComponent *PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent *EIC = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EIC->BindAction(LookAction, ETriggerEvent::Triggered, this, &APlumber::LookCharacter);
	}
}

void APlumber::LookCharacter(const FInputActionValue &Value)
{
	FVector2D LookAxisValue = Value.Get<FVector2D>();

	float YawInput = LookAxisValue.X * Xsensi;
	float PitchInput = LookAxisValue.Y * Ysensi;

	if (GetController())
	{
		AddControllerYawInput(YawInput);
		AddControllerPitchInput(PitchInput);
	}
}
