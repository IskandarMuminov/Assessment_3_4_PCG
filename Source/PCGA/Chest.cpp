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
	
	RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("RootScene"));
	RootComponent = RootScene;
	
	ChestMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ChestMesh"));
	ChestMesh->SetupAttachment(RootScene);
	
	SpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("SpawnPoint"));
	SpawnPoint->SetupAttachment(RootScene);
	
	//Cooldown for weapon spawn
	InteractionCooldown = 5.0f; 
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

	//check if ovelaps with the player
	ACharacter* PlayerCharacter = Cast<ACharacter>(OtherActor);

	// pressed E
	if (PlayerCharacter && bCanInteract)
	{
		OnInteract();

		//apply cooldown
		bCanInteract = false;
		GetWorld()->GetTimerManager().SetTimer(InteractionTimerHandle, this, &AChest::ResetInteraction, InteractionCooldown, false);
	}
}

void AChest::OnInteract()
{
	if (WeaponClass)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = GetInstigator();
		FTransform SpawnTransform = SpawnPoint->GetComponentTransform();

		//Spawn the weapon
		if (AActor* SpawnedWeapon = GetWorld()->SpawnActor<AActor>(WeaponClass, SpawnTransform, SpawnParams))
		{
			ThrowWeapon(SpawnedWeapon);
		}
	}
}

void AChest::ThrowWeapon(AActor* SpawnedWeapon)
{
	//Apply an impulse or force to the weapon to simulate a "throw"
	UStaticMeshComponent* WeaponMesh = SpawnedWeapon->FindComponentByClass<UStaticMeshComponent>();
	if (WeaponMesh)
	{
		FVector ThrowDirection = FVector(1.0f, 0.0f, 1.0f).GetSafeNormal(); //Throw direction
		WeaponMesh->AddImpulse(ThrowDirection * 1000.0f); //Apply force
	}
}

void AChest::ResetInteraction()
{
	bCanInteract = true;
}

