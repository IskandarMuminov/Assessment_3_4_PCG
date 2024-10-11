// Fill out your copyright notice in the Description page of Project Settings.


#include "Chest.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "GameFramework/Character.h"

// Sets default values
AChest::AChest()
{
	PrimaryActorTick.bCanEverTick = true;

	// Create scene component as the root
	RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("RootScene"));
	RootComponent = RootScene;

	// Create static mesh for the chest
	ChestMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ChestMesh"));
	ChestMesh->SetupAttachment(RootScene);

	// Create spawn point for the weapon
	SpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("SpawnPoint"));
	SpawnPoint->SetupAttachment(RootScene);

	// Initialize variables
	InteractionCooldown = 1.0f; // 1 second delay between interactions
	bCanInteract = true;
}

// Called when the game starts or when spawned
void AChest::BeginPlay()
{
	Super::BeginPlay();
}

void AChest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AChest::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	// Check if the overlapping actor is a player character
	ACharacter* PlayerCharacter = Cast<ACharacter>(OtherActor);

	// If the player presses the E key and can interact
	if (PlayerCharacter && bCanInteract)
	{
		OnInteract();

		// Prevent rapid interactions by using a cooldown
		bCanInteract = false;
		GetWorld()->GetTimerManager().SetTimer(InteractionTimerHandle, this, &AChest::ResetInteraction, InteractionCooldown, false);
	}
}

void AChest::OnInteract()
{
	// Ensure that the weapon class is set
	if (WeaponClass)
	{
		// Set spawn parameters
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = GetInstigator();

		// Get the spawn transform from the spawn point
		FTransform SpawnTransform = SpawnPoint->GetComponentTransform();

		// Spawn the weapon

		// Throw the weapon if successfully spawned
		if (AActor* SpawnedWeapon = GetWorld()->SpawnActor<AActor>(WeaponClass, SpawnTransform, SpawnParams))
		{
			ThrowWeapon(SpawnedWeapon);
		}
	}
}

void AChest::ThrowWeapon(AActor* SpawnedWeapon)
{
	// Apply an impulse or force to the weapon to simulate a "throw"
	UStaticMeshComponent* WeaponMesh = SpawnedWeapon->FindComponentByClass<UStaticMeshComponent>();
	if (WeaponMesh)
	{
		FVector ThrowDirection = FVector(1.0f, 0.0f, 1.0f).GetSafeNormal(); // Example throw direction
		WeaponMesh->AddImpulse(ThrowDirection * 1000.0f); // Apply impulse force
	}
}

void AChest::ResetInteraction()
{
	bCanInteract = true;
}

