// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameInstance.h"
#include "AudioSystem.h"

UMyGameInstance::UMyGameInstance()
{
	// Initialize the AudioSystem
	AudioSystem = NewObject<UAudioSystem>(this, UAudioSystem::StaticClass());
}

void UMyGameInstance::Init()
{
	Super::Init();
	// Additional initialization code can go here if needed

	if (AudioSystem)
	{
		UE_LOG(LogTemp, Warning, TEXT("Audio System Initialised"));
	}
}