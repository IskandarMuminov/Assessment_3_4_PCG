// Fill out your copyright notice in the Description page of Project Settings.


#include "Master_Room.h"

// Sets default values
AMaster_Room::AMaster_Room()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMaster_Room::BeginPlay()
{
	Super::BeginPlay();
	
}

TArray<USceneComponent*> AMaster_Room::GetExits() const
{
	// Implement logic to return the exit points in the room
	TArray<USceneComponent*> Exits;
	return Exits;
}

TArray<UPrimitiveComponent*> AMaster_Room::GetOverlappedComponents() const
{
	// Implement logic to return components that are overlapped
	TArray<UPrimitiveComponent*> OverlappedComponents;
	return OverlappedComponents;
}

// Called every frame
void AMaster_Room::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

