// Fill out your copyright notice in the Description page of Project Settings.


#include "AudioSystem.h"
#include "Kismet/GameplayStatics.h"

void UAudioSystem::PlaySoundAtLocation(USoundBase* Sound, FVector Location)
{
	if (Sound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, Sound, Location);
	}
}