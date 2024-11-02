// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProceduralMeshComponent.h"
#include "Components/ActorComponent.h"
#include "GeometryCollection/GeometryCollectionActor.h"
#include "GeometryCollection/GeometryCollectionComponent.h"
#include "GeometryCollection/GeometryCollectionObject.h"
#include "ProceduralInterior.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PCGA_API UProceduralInterior : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UProceduralInterior();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	void SetBoundsToParent();
	UPROPERTY()
	TArray<FVector> Vertices;
	UPROPERTY()
	TArray<int32> Triangles;
	UPROPERTY()
	TArray<FVector2D> UVCoords;
	UPROPERTY()
	TArray<AActor*> SpawnedObjects;
	
	UPROPERTY(EditAnywhere, Category="Spawned Object")
	float ObjectRadius;
	
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Room", meta=(AllowPrivateAccess= "true"))
	TArray<TSubclassOf<AActor>> SpawnableObjects;
	
protected:

	float GridHeight;
	float GridOffset;
	FVector TopLeft;
	FVector BottomRight;


	UPROPERTY(EditAnywhere, Category="GridSize")
	int32 GridSizeX;
	UPROPERTY(EditAnywhere, Category="GridSize")
	int32 GridSizeY;
	UPROPERTY(EditAnywhere, Category="GridSize")
	float VertexSpacing;
	
	void CreateGrid();
	void PlaceObjectsOnGrid();
	
	void SpawnObjectAtLocation(UClass* ItemToSpawn, const FVector& Location, float Yaw);
	void SpawnGeometryAtLocation(TSubclassOf<UGeometryCollection> GeometryToSpawn, const FVector& Location,
	                             float Yaw);
	FVector GetRandomPointInSquare(const FVector& UpperLeft, const FVector& LowerRight);
	bool IsPointBlocked(const FVector& Point);

};
