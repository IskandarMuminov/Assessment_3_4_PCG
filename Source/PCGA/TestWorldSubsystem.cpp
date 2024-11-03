// Fill out your copyright notice in the Description page of Project Settings.


#include "TestWorldSubsystem.h"

#include "WorldPartition/ContentBundle/ContentBundleLog.h"

void UTestWorldSubsystem::CallThisInBP()
{
	UE_LOG(LogTemp, Warning, TEXT("Called in Subsystem"));
}

void UTestWorldSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	UE_LOG(LogTemp, Warning, TEXT("Subsystem World Started"));
}
