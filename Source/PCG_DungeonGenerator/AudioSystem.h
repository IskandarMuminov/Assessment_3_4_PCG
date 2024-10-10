// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/Subsystem.h"
//#include "Kismet/GameplayStatics.h"
#include "Sound/SoundBase.h" 
#include "AudioSystem.generated.h"

/**
 * 
 */
UCLASS()
class PCG_DUNGEONGENERATOR_API UAudioSystem : public USubsystem
{
	GENERATED_BODY()

public:
	// Function to play sound at a specific location
	UFUNCTION(BlueprintCallable, Category = "Audio")
	void PlaySoundAtLocation(USoundBase* Sound, FVector Location);
	
};
