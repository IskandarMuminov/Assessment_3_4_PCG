// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "TestWorldSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class PCGA_API UTestWorldSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	void CallThisInBP();

	UPROPERTY(BluePrintReadWrite)
	int32 IntRef = 2;

protected:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	
};
