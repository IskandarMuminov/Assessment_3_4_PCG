// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Chest.generated.h"

UCLASS()
class PCGA_API AChest : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AChest();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Weapon class to spawn
	UPROPERTY(EditAnywhere, Category = "Spawning")
	
	TSubclassOf<AActor> WeaponClass;

	// Function to handle interaction (spawn and throw weapon)
	void OnInteract();

	// Function to throw the spawned weapon
	void ThrowWeapon(AActor* SpawnedWeapon);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void ResetInteraction();
	// Function to detect key press and trigger interaction
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

	// Scene component to act as root
	UPROPERTY(VisibleAnywhere, Category = "Components")
	USceneComponent* RootScene;

	// Static mesh component to represent the chest
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UStaticMeshComponent* ChestMesh;

	// Point where the weapon will spawn
	UPROPERTY(VisibleAnywhere, Category = "Components")
	USceneComponent* SpawnPoint;

	// Delay between key presses (prevent rapid spamming)
	UPROPERTY(EditAnywhere, Category = "Interaction")
	float InteractionCooldown;

private:
	FTimerHandle InteractionTimerHandle;
	bool bCanInteract;
};
