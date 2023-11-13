// Fill out your copyright notice in the Description page of Project Settings.

#include "Plumber.h"
#include "Camera/CameraComponent.h"
#include "Components/SpotLightComponent.h"
#include "Engine/EngineTypes.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "Components/Image.h"
#include "Components/WidgetComponent.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Valve.h"

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
	bShouldRotate = false;
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
	if (WidgetClass)
	{
		MainUI = CreateWidget<UUserWidget>(GetWorld(), WidgetClass);
		if (MainUI)
		{
			MainUI->AddToViewport();
		}
	}
}

// Called every frame
void APlumber::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector StartLocation = ViewCam->GetComponentLocation();
	FVector ForwardVector = ViewCam->GetForwardVector();

	float RaycastLength = 300.0f;
	FVector EndLocation = StartLocation + (ForwardVector * RaycastLength);

	FHitResult HitResult;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);
	UImage *CrosshairImage = Cast<UImage>(MainUI->GetWidgetFromName(TEXT("Crosshair")));

	if (GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Camera, CollisionParams))
	{

		AValve *Valve = Cast<AValve>(HitResult.GetActor());
		if (Valve)
		{
			CrosshairImage->SetColorAndOpacity(FLinearColor::Red);
		}
		else
		{
			CrosshairImage->SetColorAndOpacity(FLinearColor::White);
		}
	}
	else
	{
		CrosshairImage->SetColorAndOpacity(FLinearColor::White);
	}

	if (!bShouldRotate)
	{
		GetWorld()->GetTimerManager().ClearTimer(InteractTimerHandle);
	}
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
		EIC->BindAction(InteractAction, ETriggerEvent::Started, this, &APlumber::StartInteract);
		EIC->BindAction(InteractAction, ETriggerEvent::Completed, this, &APlumber::StopInteract);
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
void APlumber::Interact()
{
	FVector StartLocation = ViewCam->GetComponentLocation();
	FVector ForwardVector = ViewCam->GetForwardVector();

	float RaycastLength = 300.0f;
	FVector EndLocation = StartLocation + (ForwardVector * RaycastLength);

	FHitResult HitResult;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);
	UImage *CrosshairImage = Cast<UImage>(MainUI->GetWidgetFromName(TEXT("Crosshair")));

	if (GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Camera, CollisionParams))
	{
		AValve *Valve = Cast<AValve>(HitResult.GetActor());
		if (Valve)
		{

			if (bShouldRotate)
			{
				// Get the current rotation of the valve
				FRotator CurrentRotation = Valve->GetActorRotation();

				// Calculate the new pitch rotation by adding the increment
				float NewPitch = FMath::Min(CurrentRotation.Pitch + 1.0f, 90.0f);

				// Create a new rotation with the updated pitch, keeping yaw and roll unchanged
				FRotator NewRotation = FRotator(NewPitch, CurrentRotation.Yaw, CurrentRotation.Roll);

				// Set the new rotation for the valve
				Valve->SetActorRotation(NewRotation);

				// Log a message indicating that the closing has started
				UE_LOG(LogTemp, Warning, TEXT("Closing started"));

				// Schedule a callback to continue rotation every frame
				GetWorld()->GetTimerManager().SetTimer(InteractTimerHandle, this, &APlumber::Interact, 0.01f, true);
			}
			else
			{
				// Clear the timer when rotation is not needed
				GetWorld()->GetTimerManager().ClearTimer(InteractTimerHandle);
			}
		}
		else
		{
			StopInteract();
		}
	}
}

void APlumber::StartInteract()
{
	bShouldRotate = true;
	Interact(); // Call the function once when the key is initially pressed
}

void APlumber::StopInteract()
{
	bShouldRotate = false;
	GetWorld()->GetTimerManager().ClearTimer(InteractTimerHandle);
}
