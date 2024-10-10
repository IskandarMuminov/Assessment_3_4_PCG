// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Master_Room.generated.h"

UCLASS()
class PCG_DUNGEONGENERATOR_API AMaster_Room : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMaster_Room();

	// Function to get the available exits in the room
	UFUNCTION(BlueprintCallable, Category = "Room")
	TArray<USceneComponent*> GetExits() const;

	// Function to get the components that are overlapped
	UFUNCTION(BlueprintCallable, Category = "Room")
	TArray<UPrimitiveComponent*> GetOverlappedComponents() const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
