// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "AudioSystem.h"
#include "MyGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class PCG_DUNGEONGENERATOR_API UMyGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UMyGameInstance();

	virtual void Init() override;

	// Audio System
	UPROPERTY()
	UAudioSystem* AudioSystem;

};
