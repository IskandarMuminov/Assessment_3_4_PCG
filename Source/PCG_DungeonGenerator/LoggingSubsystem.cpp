// Fill out your copyright notice in the Description page of Project Settings.


#include "LoggingSubsystem.h"
#include "Engine/Engine.h"

void ULoggingSubsystem::LogMessage(FString Message)
{
    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, Message);
    }
    UE_LOG(LogTemp, Log, TEXT("%s"), *Message);
}
