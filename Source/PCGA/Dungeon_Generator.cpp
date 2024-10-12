// Fill out your copyright notice in the Description page of Project Settings.


#include "Dungeon_Generator.h"
#include "MasterRoom.h"
#include "Components/SceneComponent.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "UObject/ConstructorHelpers.h" 

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

	static ConstructorHelpers::FClassFinder<AMasterRoom> Room1(TEXT("/Game/Path/To/Dungeon_Room_1"));
	if (Room1.Succeeded())
	{
		RoomList.Add(Room1.Class);
	}

	static ConstructorHelpers::FClassFinder<AMasterRoom> Room2(TEXT("/Game/Path/To/Dungeon_Room_2"));
	if (Room2.Succeeded())
	{
		RoomList.Add(Room2.Class);
	}

	static ConstructorHelpers::FClassFinder<AMasterRoom> Room3(TEXT("/Game/Path/To/Dungeon_Room_3"));
	if (Room3.Succeeded())
	{
		RoomList.Add(Room3.Class);
	}

	static ConstructorHelpers::FClassFinder<AMasterRoom> Room4(TEXT("/Game/Path/To/Dungeon_Room_4"));
	if (Room4.Succeeded())
	{
		RoomList.Add(Room4.Class);
	}

	static ConstructorHelpers::FClassFinder<AMasterRoom> Room5(TEXT("/Game/Path/To/Dungeon_Room_5"));
	if (Room5.Succeeded())
	{
		RoomList.Add(Room5.Class);
	}

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
			LatestRoom = GetWorld()->SpawnActor<AMasterRoom>(RoomToSpawn, SpawnTransform, SpawnParams);

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
	OverlappedList = LatestRoom->GetOverlappedComponents();

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
		//
			// Spawn end walls at the transform of each exit component
			//FTransform SpawnTransform = ExitComponent->GetComponentTransform();
			//FActorSpawnParameters SpawnParams;
			//SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

			// Spawn the end wall actor to close the hole
			//GetWorld()->SpawnActor<AEndWallClass>(EndWallClass, SpawnTransform, SpawnParams);
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

