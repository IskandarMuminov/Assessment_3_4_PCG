// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "LoggingSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class PCG_DUNGEONGENERATOR_API ULoggingSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
	public:
    	UFUNCTION(BlueprintCallable, Category = "Logging")
    	void LogMessage(FString Message);
	
};
