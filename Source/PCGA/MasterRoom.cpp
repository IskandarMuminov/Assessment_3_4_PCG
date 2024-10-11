// Fill out your copyright notice in the Description page of Project Settings.


#include "MasterRoom.h"

// Sets default values
AMasterRoom::AMasterRoom()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMasterRoom::BeginPlay()
{
	Super::BeginPlay();
	
}

TArray<USceneComponent*> AMasterRoom::GetExits() const
{
	// Implement logic to return the exit points in the room
	TArray<USceneComponent*> Exits;
	return Exits;
}

TArray<UPrimitiveComponent*> AMasterRoom::GetOverlappedComponents() const
{
	// Implement logic to return components that are overlapped
	TArray<UPrimitiveComponent*> OverlappedComponents;
	return OverlappedComponents;
}

// Called every frame
void AMasterRoom::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
