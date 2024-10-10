// Fill out your copyright notice in the Description page of Project Settings.


#include "Dungeon_Generator.h"
#include "Master_Room.h"
#include "Components/SceneComponent.h"
#include "Engine/World.h"
#include "TimerManager.h"
//#include "Door.h" 

// Sets default values
ADungeon_Generator::ADungeon_Generator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set the root component of the actor
	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
	RootComponent = DefaultSceneRoot;

	// Initialize default values
	RoomAmount = 30;
	DungeonComplete = false;
	MaxDungeonTime = 60.0;
	Seed = -1;

	//SpawnDoorsAtExits();

}

// Called when the game starts or when spawned
void ADungeon_Generator::BeginPlay()
{
	Super::BeginPlay();

	// Initialize the random stream using the seed
	Stream.Initialize(Seed);

	GenerateDungeon();
	
	// Start the dungeon generation process
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ADungeon_Generator::GenerateDungeon, 1.0f, true);

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Simple log message."));
	}
}

void ADungeon_Generator::GenerateDungeon()
{
	// Check if the dungeon is already complete
	if (DungeonComplete || RoomAmount <= 0)
	{
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
		DungeonComplete = true;
		return;
	}

	// Pick a random room from the RoomList
	if (RoomList.Num() > 0)
	{
		// Select a random room class
		int32 RandomIndex = Stream.RandRange(0, RoomList.Num() - 1);
		UClass* RoomToSpawn = RoomList[RandomIndex];

		// Spawn the room at the selected exit point
		if (RoomToSpawn && SelectedExitPoint)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = GetInstigator();

			FTransform SpawnTransform = SelectedExitPoint->GetComponentTransform();
			LatestRoom = GetWorld()->SpawnActor<AMaster_Room>(RoomToSpawn, SpawnTransform, SpawnParams);

			if (LatestRoom)
			{
				// Reduce the remaining room amount
				RoomAmount--;

				// Update ExitsList and set a new SelectedExitPoint
				ExitsList = LatestRoom->GetExits(); // Assuming `GetExits()` returns an array of exits
				if (ExitsList.Num() > 0)
				{
					int32 ExitIndex = Stream.RandRange(0, ExitsList.Num() - 1);
					SelectedExitPoint = ExitsList[ExitIndex];
				}

				// Optionally handle overlaps
				CheckForOverlaps();
			}
		}
	}
}

void ADungeon_Generator::CheckForOverlaps()
{
	// Assuming LatestRoom has a function to get overlapped components
	//OverlappedList = LatestRoom->GetOverlappedComponents();

	if (LatestRoom)
	{
		// Get the overlapped components
		OverlappedList = LatestRoom->GetOverlappedComponents();

		if (OverlappedList.Num() > 0)
		{
			// Handle overlaps
			for (UPrimitiveComponent* OverlappedComponent : OverlappedList)
			{
				if (OverlappedComponent)
				{
					// Here you can handle what happens during an overlap.
					// For example, you could log the overlap or destroy the room.
					UE_LOG(LogTemp, Warning, TEXT("Overlap detected with: %s"), *OverlappedComponent->GetName());

					// Destroy the latest room if overlapping too much
					LatestRoom->Destroy();
					RoomAmount++; // Increment RoomAmount as this room was not valid
					break; // Exit after the first overlap is handled, adjust as needed
				}
			}
		}
	}
}

//void ADungeon_Generator::SpawnNextRoom()
//{
	// Ensure room management logic
	//if (RoomList.Num() > 0)
	//{
		// Remove the oldest room from the array
		//RoomList.RemoveAt(0);
	//}

	// Append the new room to the RoomList
	//RoomList.Add(LatestRoom);

	// Example condition to check if spawning can continue
	//bool bCanSpawn = (RoomList.Num() > 0);

	// If we can spawn, proceed to spawn the next room
	//if (bCanSpawn)
	//{
		// Spawn the next room using the selected exit point's transform
		//FTransform SpawnTransform = SelectedExitPoint->GetComponentTransform();
		//FActorSpawnParameters SpawnParams;

		// Spawn the room at the transform of the selected exit point
		//GetWorld()->SpawnActor<AMaster_Room>(RoomToSpawnClass, SpawnTransform, SpawnParams);
	//}
//}

//void ADungeon_Generator::CloseHoles()
//{
	// Loop through all exit components in the current room's ExitsList
	//for (USceneComponent* ExitComponent : ExitsList)
	//{
		//if (ExitComponent)
		//{
			// Spawn end walls at the transform of each exit component
			//FTransform SpawnTransform = ExitComponent->GetComponentTransform();
			//FActorSpawnParameters SpawnParams;
			//SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

			// Spawn the end wall actor to close the hole
			//GetWorld()->SpawnActor<AEndWallClass>(EndWallClass, SpawnTransform, SpawnParams);
		//}
	//}
//}

//void ADungeon_Generator::SpawnDoorsAtExits()
//{
	//if (DoorClass && LatestRoom)
	//{
		//for (USceneComponent* ExitComponent : LatestRoom->GetExits())
		//{
			//if (ExitComponent)
			//{
				//FTransform SpawnTransform = ExitComponent->GetComponentTransform();
				//FActorSpawnParameters SpawnParams;
				//SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

				// Spawn the door
				//ADoor* SpawnedDoor = GetWorld()->SpawnActor<ADoor>(DoorClass, SpawnTransform, SpawnParams);

				// Optionally log the spawn position for debugging
				//UE_LOG(LogTemp, Warning, TEXT("Spawned door at: %s"), *SpawnTransform.GetLocation().ToString());
			//}
			//else
			//{
				//UE_LOG(LogTemp, Warning, TEXT("ExitComponent is null!"));
			//}
		//}
	//}
//}

// Called every frame
void ADungeon_Generator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

/**
 * EndPlay - Cleanup when the actor is destroyed or the game ends
 */
void ADungeon_Generator::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	// Clear the timer if it is still running
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
}

