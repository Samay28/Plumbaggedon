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
#include "Components/MeshComponent.h"
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
#include "TimerManager.h"
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
	IsPlayerDead = false;
	CanEquipSpray = false;
	count = 0;
	ValvesCount = 0;
	countforcheckvalve = 0;

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
		CheckpointLocation = this->GetActorLocation();
	}

	TArray<AActor *> AttachedActors;
	this->GetAttachedActors(AttachedActors);

	for (AActor *AttachedActor : AttachedActors)
	{
		SprayCan = Cast<ASpray>(AttachedActor);
		SprayCan->SetActorHiddenInGame(true);
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
				InteractTxt->SetVisibility(ESlateVisibility::Visible);
			}
			else
			{
				InteractTxt->SetVisibility(ESlateVisibility::Hidden);
				countforcheckvalve = 0;
				Valve = Cast<AValve>(InteractedValve);
				if (Valve)
				{
					Valve->StopValveClosingSound();
					InteractedValve = nullptr;
				}
			}
		}
		else
		{
			InteractTxt->SetVisibility(ESlateVisibility::Hidden);
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
	if (ValvesCount == 5)
	{
		CanEquipSpray = true;
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
		EIC->BindAction(FireAction, ETriggerEvent::Triggered, this, &APlumber::Fire);
		EIC->BindAction(FireAction, ETriggerEvent::Completed, this, &APlumber::StopFire);
		EIC->BindAction(EquipSprayAction, ETriggerEvent::Triggered, this, &APlumber::EquipSpray);
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
	if (SprayCan && SprayEquiped)
	{
		// Cast to ASpray
		ASpray *Spray = Cast<ASpray>(SprayCan);

		// Check if the cast was successful
		if (Spray)
		{
			Spray->ActivateSpray();
		}
	}
}

void APlumber::StopFire()
{
	// Assuming SprayCan is a UPROPERTY in your APlumber class
	if (SprayCan && SprayEquiped)
	{
		// Cast to ASpray
		ASpray *Spray = Cast<ASpray>(SprayCan);

		// Check if the cast was successful
		if (Spray)
		{
			Spray->DeactivateSpray();
		}
	}
}

void APlumber::EquipSpray()
{
	if (CanEquipSpray)
	{
		TArray<AActor *> AttachedActors;
		this->GetAttachedActors(AttachedActors);

		for (AActor *AttachedActor : AttachedActors)
		{
			SprayCan = Cast<ASpray>(AttachedActor);
			SprayCan->SetActorHiddenInGame(false);
			SprayEquiped = true;
		}
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

void APlumber::RespawnPlayer()
{
	UE_LOG(LogTemp, Warning, TEXT("FEF"));
	APlayerController *PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController)
	{
		EnableInput(PlayerController);
	}
	FlashLight->SetVisibility(true);
	SpringArm->bUsePawnControlRotation = true;
	this->SetActorLocation(CheckpointLocation);
	IsPlayerDead = false;
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	SprayCan->FuelSpray = 3000;
}

void APlumber::OnCollisionBegin(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	// Check if the other actor has the "Enemy" tag
	if (OtherActor && OtherActor->ActorHasTag("Enemy"))
	{
		APlayerController *PlayerController = Cast<APlayerController>(GetController());
		AController *OtherController = Cast<APawn>(OtherActor) ? Cast<APawn>(OtherActor)->GetController() : Cast<AController>(OtherActor);
		AEnemy_AIController *EnemyController = Cast<AEnemy_AIController>(OtherController);

		if (PlayerController && ViewCam && EnemyController && !EnemyController->isDead)
		{
			IsPlayerDead = true;
			DisableInput(PlayerController);
			SpringArm->bUsePawnControlRotation = false;

			APawn *EnemyPawn = EnemyController->GetPawn();
			UStaticMeshComponent *EnemyMeshComponent = EnemyPawn ? Cast<UStaticMeshComponent>(EnemyPawn->GetComponentByClass(UStaticMeshComponent::StaticClass())) : nullptr;

			if (EnemyMeshComponent && FlashLight)
			{
				UE_LOG(LogTemp, Warning, TEXT("Rotation calculation successful"));

				FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(ViewCam->GetComponentLocation(), EnemyMeshComponent->GetComponentLocation());
				FRotator NewRotation = FMath::RInterpTo(ViewCam->GetComponentRotation(), TargetRotation, GetWorld()->GetDeltaSeconds(), 2.f);

				SpringArm->SetWorldRotation(TargetRotation);
				FlashLight->SetVisibility(false);
				GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
				GetWorld()->GetTimerManager().SetTimer(TimerHandle_Respawn, this, &APlumber::RespawnPlayer, 3, false);
			}
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

	if (GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Camera, CollisionParams))
	{
		Valve = Cast<AValve>(HitResult.GetActor());
		InteractedValve = Valve;
		if (Valve && !Valve->IsValveCompleted)
		{
			FRotator RotationDelta = FRotator(0.0f, 0.0f, 1.0f);
			Valve->isRotating = true;

			// Use DeltaTime to make rotation frame rate independent
			float RotationSpeed = 100.0f; // Adjust the speed as needed
			float RotationAmount = RotationSpeed * GetWorld()->GetDeltaSeconds();

			Valve->AddActorWorldRotation(RotationDelta * RotationAmount);

			if (Valve->TotalRotation >= 720.0f)
			{
				Valve->CloseValve();
				StopInteract();
				Valve->IsValveCompleted = true;
				// Valve->isRotating = false;
				CheckpointLocation = this->GetActorLocation();
				ValvesCount++;
			}
			else
			{
				Valve->TotalRotation += RotationAmount;
				// Schedule a callback to continue rotation every frame
				GetWorld()->GetTimerManager().SetTimer(InteractTimerHandle, this, &APlumber::Interact, 0.01f, true);
			}
		}
		else
		{
			// Valve->isRotating = false;
			StopInteract();
		}
	}
}

void APlumber::StartInteract()
{
	Valve = Cast<AValve>(InteractedValve);
	if (Valve && countforcheckvalve == 0)
	{
		Valve->PlayValveClosingSound();
		countforcheckvalve++;
	}
	Interact();
}

void APlumber::StopInteract()
{
	countforcheckvalve = 0;
	Valve = Cast<AValve>(InteractedValve);
	if (Valve)
	{
		Valve->StopValveClosingSound();
		InteractedValve = nullptr;
	}
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
	GetWorldTimerManager().SetTimer(TimerHandle_EnableInput, this, &APlumber::EnableInputFunction, 5.0f, false);
}
