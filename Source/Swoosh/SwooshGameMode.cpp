// Copyright Epic Games, Inc. All Rights Reserved.

#include "SwooshGameMode.h"
#include "SwooshCharacter.h"
#include "UObject/ConstructorHelpers.h"

ASwooshGameMode::ASwooshGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
