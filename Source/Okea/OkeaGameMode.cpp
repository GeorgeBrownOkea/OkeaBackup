// Copyright Epic Games, Inc. All Rights Reserved.

#include "OkeaGameMode.h"
#include "OkeaCharacter.h"
#include "UObject/ConstructorHelpers.h"

AOkeaGameMode::AOkeaGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
