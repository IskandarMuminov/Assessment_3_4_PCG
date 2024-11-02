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
	
	void ThrowWeapon(AActor* SpawnedWeapon);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	void OnInteract();

	void ResetInteraction();
	void ServerOnInteract();

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	
	UPROPERTY(VisibleAnywhere, Category = "Components")
	USceneComponent* RootScene;
	
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UStaticMeshComponent* ChestMesh;
	
	UPROPERTY(VisibleAnywhere, Category = "Components")
	USceneComponent* SpawnPoint;
	
	UPROPERTY(EditAnywhere, Category = "Interaction")
	float InteractionCooldown;
	
private:
	FTimerHandle InteractionTimerHandle;
	bool bCanInteract;

	bool ServerOnInteract_Validate();
	void OnRep_CanInteract();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
