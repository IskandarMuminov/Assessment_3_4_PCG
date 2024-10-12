// Fill out your copyright notice in the Description page of Project Settings.


#include "MasterRoom.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"
#include "Components/PrimitiveComponent.h"

// Sets default values
AMasterRoom::AMasterRoom()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RoomMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RoomMesh"));
	RootComponent = RoomMesh;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("StaticMesh'/Game/Path/To/Your/Mesh.Mesh'"));
	if (MeshAsset.Succeeded())
	{
		RoomMesh->SetStaticMesh(MeshAsset.Object);
	}

}

// Called when the game starts or when spawned
void AMasterRoom::BeginPlay()
{
	Super::BeginPlay();
	
}

TArray<USceneComponent*> AMasterRoom::GetExits() const
{
	// Implement logic to return the exit points in the room
	//TArray<USceneComponent*> Exits;
	//return Exits;
	return ExitPoints;
}

TArray<UPrimitiveComponent*> AMasterRoom::GetOverlappedComponents() const
{
	// Implement logic to return components that are overlapped
	TArray<UPrimitiveComponent*> OverlappedComponents;

	// Assuming you have some way to check for overlaps
	// You might use a specific collision channel or overlap detection mechanism
	//TArray<AActor*> OverlappingActors;
	//RoomMesh->GetOverlappingActors(OverlappingActors);

	TArray<UActorComponent*> Components;
	RoomMesh->GetOverlappingComponents(OverlappedComponents);

	return OverlappedComponents;
}

// Called every frame
void AMasterRoom::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMasterRoom::AddExit(USceneComponent* ExitPoint)
{
	if (ExitPoint)
	{
		ExitPoints.Add(ExitPoint);
	}
}
