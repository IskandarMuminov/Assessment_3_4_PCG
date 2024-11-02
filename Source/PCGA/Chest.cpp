#include "Chest.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "GameFramework/Character.h"
#include "Net/UnrealNetwork.h"

// Sets default values
AChest::AChest()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true; // Enable replication for the chest

	RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("RootScene"));
	RootComponent = RootScene;
	
	ChestMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ChestMesh"));
	ChestMesh->SetupAttachment(RootScene);
	
	SpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("SpawnPoint"));
	SpawnPoint->SetupAttachment(RootScene);
	
	// Cooldown for weapon spawn
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

	// Check if overlaps with the player
	ACharacter* PlayerCharacter = Cast<ACharacter>(OtherActor);

	if (PlayerCharacter && bCanInteract)
	{
		// Request interaction on the server
		if (HasAuthority())
		{
			ServerOnInteract();
		}
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

		// Spawn the weapon on the server only
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
		FVector ThrowDirection = FVector(1.0f, 0.0f, 1.0f).GetSafeNormal(); // Throw direction
		WeaponMesh->AddImpulse(ThrowDirection * 1000.0f); // Apply force
	}
}

void AChest::ResetInteraction()
{
	bCanInteract = true;
	OnRep_CanInteract(); // Ensure the state is updated visually on clients
}

void AChest::ServerOnInteract()
{
	// This function is only executed on the server
	if (bCanInteract)
	{
		OnInteract();

		// Apply cooldown
		bCanInteract = false;
		OnRep_CanInteract(); // Update clients about the interaction state
		GetWorld()->GetTimerManager().SetTimer(InteractionTimerHandle, this, &AChest::ResetInteraction, InteractionCooldown, false);
	}
}

bool AChest::ServerOnInteract_Validate()
{
	return true;
}

void AChest::OnRep_CanInteract()
{
	// Optional: add visual feedback for interaction cooldown on clients
}

void AChest::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Replicate the interaction state
	//DOREPLIFETIME(AChest, bCanInteract);
}
