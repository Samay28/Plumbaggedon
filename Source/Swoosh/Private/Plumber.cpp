// Fill out your copyright notice in the Description page of Project Settings.

#include "Plumber.h"
#include "Camera/CameraComponent.h"
#include "Components/SpotLightComponent.h"
#include "Engine/EngineTypes.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "Components/ChildActorComponent.h"
#include "Components/TextBlock.h"
#include "Components/WidgetComponent.h"
#include "Components/CapsuleComponent.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundCue.h"
#include "Valve.h"
#include "Spray.h"
#include "LevelSequenceActor.h"
#include "LevelSequencePlayer.h"
#include "EngineUtils.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISense_Sight.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetMathLibrary.h"
#include "Enemy_AIController.h"

// Sets default values
APlumber::APlumber()
{

	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpingArm"));
	SpringArm->SetupAttachment(GetRootComponent());
	SpringArm->TargetArmLength = 0;

	ViewCam = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	ViewCam->SetupAttachment(SpringArm);

	FlashLight = CreateDefaultSubobject<USpotLightComponent>(TEXT("FlashLight"));
	FlashLight->SetupAttachment(ViewCam);

	SetupStimulusSource();
	CanStartGame = false;
	count = 0;

	// Iterate through all actors in the world to find the LevelSequenceActor
}

// Called when the game starts or when spawned
void APlumber::BeginPlay()
{
	Super::BeginPlay();

	if (APlayerController *PlayerController = Cast<APlayerController>(GetController()))
	{
		UEnhancedInputLocalPlayerSubsystem *Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
		this->DisableInput(PlayerController);
		if (Subsystem)
		{
			Subsystem->AddMappingContext(PlumberMappingContext, 0);
		}

		MainUI = CreateWidget<UUserWidget>(GetWorld(), WidgetClasses[0]);
		MainMenuUI = CreateWidget<UUserWidget>(GetWorld(), WidgetClasses[1]);
		if (MainMenuUI)
		{
			MainMenuUI->AddToViewport();
		}
	}

	TArray<AActor *> AttachedActors;
	this->GetAttachedActors(AttachedActors);

	for (AActor *AttachedActor : AttachedActors)
	{
		SprayCan = Cast<ASpray>(AttachedActor);
	}

	if (StartScene)
	{
		ULevelSequence *LevelSequence = StartScene->GetSequence();
		if (LevelSequence)
		{
			// Create a new LevelSequencePlayer
			FMovieSceneSequencePlaybackSettings PlaybackSettings;
			PlaybackSettings.bAutoPlay = true;
			// Create a new LevelSequencePlayer
			SequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(GetWorld(), LevelSequence, PlaybackSettings, StartScene);
			if (SequencePlayer)
			{
				SequencePlayer->Pause();
			}
		}
	}
	else
	{
	}
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &APlumber::OnCollisionBegin);
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

	if (MainUI && CanStartGame && count == 1)
	{
		UTextBlock *InteractTxt = Cast<UTextBlock>(MainUI->GetWidgetFromName(TEXT("HoldE")));

		if (GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Camera, CollisionParams))
		{

			Valve = Cast<AValve>(HitResult.GetActor());
			if (Valve && !Valve->IsValveCompleted)
			{
				InteractTxt->SetText(FText::FromString(TEXT("Hold E")));
			}
			else
			{
				InteractTxt->SetText(FText::FromString(TEXT("")));
			}
		}
		else
		{
			InteractTxt->SetText(FText::FromString(TEXT("")));
		}
	}

	if (WidgetClasses[0])
	{
		if (MainUI && CanStartGame && count == 0)
		{
			MainUI->AddToViewport();
			count++;
		}
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
		EIC->BindAction(InteractAction, ETriggerEvent::Triggered, this, &APlumber::StartInteract);
		EIC->BindAction(InteractAction, ETriggerEvent::Completed, this, &APlumber::StopInteract);
		EIC->BindAction(FireAction, ETriggerEvent::Started, this, &APlumber::Fire);
		EIC->BindAction(FireAction, ETriggerEvent::Completed, this, &APlumber::StopFire);
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
	GetCharacterMovement()->MaxWalkSpeed = 600.0f;
	// BobSpeed = 12.0f;
}

void APlumber::StopSprint()
{
	// BobSpeed = 10.0f;
	GetCharacterMovement()->MaxWalkSpeed = 300.0f;
}

void APlumber::Fire()
{
	// Assuming SprayCan is a UPROPERTY in your APlumber class
	if (SprayCan)
	{
		// Cast to ASpray
		ASpray *Spray = Cast<ASpray>(SprayCan);

		// Check if the cast was successful
		if (Spray)
		{
			// Call the ActivateSpray function on the casted ASpray instance
			Spray->ActivateSpray();
			UE_LOG(LogTemp, Warning, TEXT("Spray activated"));
		}
		else
		{
			// Handle the case where the cast failed
			UE_LOG(LogTemp, Warning, TEXT("Failed to cast SprayCan to ASpray"));
		}
	}
	else
	{
		// Handle the case where SprayCan is nullptr
		UE_LOG(LogTemp, Warning, TEXT("SprayCan is nullptr"));
	}
}

void APlumber::StopFire()
{
	// Assuming SprayCan is a UPROPERTY in your APlumber class
	if (SprayCan)
	{
		// Cast to ASpray
		ASpray *Spray = Cast<ASpray>(SprayCan);

		// Check if the cast was successful
		if (Spray)
		{
			// Call the DeactivateSpray function on the casted ASpray instance
			Spray->DeactivateSpray();
			UE_LOG(LogTemp, Warning, TEXT("Spray deactivated"));
		}
		else
		{
			// Handle the case where the cast failed
			UE_LOG(LogTemp, Warning, TEXT("Failed to cast SprayCan to ASpray"));
		}
	}
	else
	{
		// Handle the case where SprayCan is nullptr
		UE_LOG(LogTemp, Warning, TEXT("SprayCan is nullptr"));
	}
}

void APlumber::EnableInputFunction()
{

	if (APlayerController *PlayerController = Cast<APlayerController>(GetController()))
	{
		if (PlayerController)
		{
			this->EnableInput(PlayerController);
			CanStartGame = true;
		}
	}
}

void APlumber::OnCollisionBegin(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	AController *OtherController = nullptr;

	// Try to get the controller from the other actor
	APawn *OtherPawn = Cast<APawn>(OtherActor);
	if (OtherPawn)
	{
		OtherController = OtherPawn->GetController();
	}
	else
	{
		// If it's not a pawn, try to get the controller directly
		OtherController = Cast<AController>(OtherActor);
	}

	if (OtherController && OtherController->IsA(AEnemy_AIController::StaticClass()))
	{
		APlayerController *PlayerController = Cast<APlayerController>(GetController());
		if (PlayerController)
		{
			// Disable input for the player controller
			DisableInput(PlayerController);
			ViewCam->bUsePawnControlRotation = false;

			// Disable automatic control rotation during the manual rotation
		}

		// Rotate the SpringArm towards the enemy over 2 seconds
		AEnemy_AIController *EnemyController = Cast<AEnemy_AIController>(OtherController);
		if (EnemyController && ViewCam)
		{

			// Calculate rotation towards the enemy
			FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(ViewCam->GetComponentLocation(), EnemyController->GetPawn()->GetActorLocation());

			// Interpolate the rotation for smooth movement over 2 seconds
			FRotator NewRotation = FMath::RInterpTo(ViewCam->GetComponentRotation(), TargetRotation, GetWorld()->GetDeltaSeconds(), 2.0f);

			// Set the new rotation for the camera (ViewCam)
			ViewCam->SetWorldRotation(NewRotation);

			// Optionally, you can also adjust the rotation of the pawn's root component
			// GetRootComponent()->SetWorldRotation(NewRotation);
		}
	}
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
	// UImage *CrosshairImage = Cast<UImage>(MainUI->GetWidgetFromName(TEXT("Crosshair")));

	if (GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Camera, CollisionParams))
	{
		Valve = Cast<AValve>(HitResult.GetActor());
		if (Valve && !Valve->IsValveCompleted)
		{
			// Valve->SetInteractingPlumber(this);
			// Create a rotation delta with the desired pitch increment
			FRotator RotationDelta = FRotator(0.0f, 0.0f, 1.0f);

			// Rotate the valve around the X-axis
			Valve->AddActorWorldRotation(RotationDelta);

			// Update the total rotation
			Valve->TotalRotation += 1.0f;

			// Log a message indicating that the closing has started
			UE_LOG(LogTemp, Warning, TEXT("Closing started"));

			// Check if the total rotation exceeds 720 degrees
			if (Valve->TotalRotation >= 720.0f)
			{
				Valve->CloseValve();
				StopInteract();
				Valve->IsValveCompleted = true;
				UE_LOG(LogTemp, Warning, TEXT("Closing Stopped. Total rotation exceeded 720 degrees."));
			}
			else
			{
				// Schedule a callback to continue rotation every frame
				GetWorld()->GetTimerManager().SetTimer(InteractTimerHandle, this, &APlumber::Interact, 0.01f, true);
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
	// bShouldRotate = true;
	Interact(); // Call the function once when the key is initially pressed
}

void APlumber::StopInteract()
{
	// bShouldRotate = false;
	GetWorld()->GetTimerManager().ClearTimer(InteractTimerHandle);
}
void APlumber::SetupStimulusSource()
{
	StimulusSource = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("Stimulus"));
	if (StimulusSource)
	{
		StimulusSource->RegisterForSense(TSubclassOf<UAISense_Sight>());
		StimulusSource->RegisterWithPerceptionSystem();
	}
}

void APlumber::StartGameFunctions()
{
	if (SequencePlayer)
	{
		SequencePlayer->Play();
	}
	MainMenuUI->RemoveFromParent();
	GetWorldTimerManager().SetTimer(TimerHandle_EnableInput, this, &APlumber::EnableInputFunction, 5.0f, false);
}
