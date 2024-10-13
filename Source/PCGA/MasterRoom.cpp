// Fill out your copyright notice in the Description page of Project Settings.


#include "MasterRoom.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"
#include "Components/PrimitiveComponent.h"

// Sets default values
AMasterRoom::AMasterRoom()
{
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
	return ExitPoints;
}

TArray<UPrimitiveComponent*> AMasterRoom::GetOverlappedComponents() const
{
	// Implement logic to return components that are overlapped
	TArray<UPrimitiveComponent*> OverlappedComponents;

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
