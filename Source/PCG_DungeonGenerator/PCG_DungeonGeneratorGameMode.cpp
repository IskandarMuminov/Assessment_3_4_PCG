// Copyright Epic Games, Inc. All Rights Reserved.

#include "PCG_DungeonGeneratorGameMode.h"
#include "PCG_DungeonGeneratorCharacter.h"
#include "UObject/ConstructorHelpers.h"

APCG_DungeonGeneratorGameMode::APCG_DungeonGeneratorGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
