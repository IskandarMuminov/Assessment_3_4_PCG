// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h"
#include "ProceduralRoom.generated.h"

UCLASS()
class PCGA_API AProceduralRoom : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProceduralRoom();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual bool ShouldTickIfViewportsOnly() const override;

private:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Room", meta=(AllowPrivateAccess= "true"))
	UProceduralMeshComponent* Floor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Room", meta=(AllowPrivateAccess= "true"))
	TSubclassOf<AActor> ChestClass;

	void SpawnObject(UClass* ItemToSpawn);

	float SquareWidth;
	float GridHeight;

	float RoomWidth;
	float RoomLength;
	
	FVector TopLeft;
	FVector BottomRight;
	
	int32 GridSizeX;
	int32 GridSizeY;

	void CreateGrid();
};
