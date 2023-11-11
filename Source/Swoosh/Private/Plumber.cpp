// Fill out your copyright notice in the Description page of Project Settings.

#include "Plumber.h"
#include "Camera/CameraComponent.h"
#include "Components/SpotLightComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
APlumber::APlumber()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpingArm"));
	SpringArm->SetupAttachment(GetRootComponent());
	SpringArm->TargetArmLength = 0;
	SpringArm->bEnableCameraRotationLag = true;
	SpringArm->CameraRotationLagSpeed = 9.0f;

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

	// if(!GetVelocity().IsNearlyZero())
	// {
	// 	FVector NewCamOffset = GetActorUpVector() * FMath::Sin(GetWorld()->GetTimeSeconds() * GetCharacterMovement()->MaxWalkSpeed/20) * BobAmount;
	// 	ViewCam->AddRelativeLocation(NewCamOffset);
	// }
}

// Called to bind functionality to input
void APlumber::SetupPlayerInputComponent(UInputComponent *PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent *EIC = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EIC->BindAction(LookAction, ETriggerEvent::Triggered, this, &APlumber::LookCharacter);
		EIC->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APlumber::MoveCharacter);
		EIC->BindAction(SprintAction, ETriggerEvent::Triggered, this, &APlumber::Sprint);
		EIC->BindAction(SprintAction, ETriggerEvent::Completed, this, &APlumber::StopSprint);
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

void APlumber::MoveCharacter(const FInputActionValue &Value)
{
	const FVector2D MovementValue = Value.Get<FVector2D>();
	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	AddMovementInput(ForwardDirection, MovementValue.Y);

	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	AddMovementInput(RightDirection, MovementValue.X);
}

void APlumber::Sprint()
{
	GetCharacterMovement()->MaxWalkSpeed = 500.0f;
	// BobSpeed = 12.0f;
}

void APlumber::StopSprint()
{
	// BobSpeed = 10.0f;
	GetCharacterMovement()->MaxWalkSpeed = 300.0f;
}
