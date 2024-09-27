// Copyright Epic Games, Inc. All Rights Reserved.

#include "Assessment_3_4_PCGGameMode.h"
#include "Assessment_3_4_PCGCharacter.h"
#include "UObject/ConstructorHelpers.h"

AAssessment_3_4_PCGGameMode::AAssessment_3_4_PCGGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}
