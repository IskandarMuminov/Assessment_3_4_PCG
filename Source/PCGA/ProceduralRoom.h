// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "ProceduralRoom.generated.h"

UCLASS(Blueprintable, BlueprintType)
class PCGA_API AProceduralRoom : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProceduralRoom();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY()
	TArray<FVector> Vertices;
	UPROPERTY()
	TArray<int32> Triangles;
	UPROPERTY()
	TArray<FVector2D> UVCoords;
	UPROPERTY()
	TArray<AActor*> SpawnedObjects;

	UPROPERTY(EditAnywhere)
	bool bShouldRegenerate;
	
	UPROPERTY(EditAnywhere, Category="Spawned Object")
	float ChestRadius;
	UPROPERTY(EditAnywhere, Category="Spawned Object")
	float BarrelRadius;
	UPROPERTY(EditAnywhere, Category="Spawned Object")
	float BrazierRadius;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual bool ShouldTickIfViewportsOnly() const override;

	/** Root scene component */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Default", meta=(AllowPrivateAccess= "true"))
	TObjectPtr<USceneComponent> DefaultSceneRoot;

	/** Scene component for organizing exits */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Default", meta=(AllowPrivateAccess= "true"))
	TObjectPtr<USceneComponent> ExitsFolder;

	/** Box component for detecting overlaps */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Default", meta=(AllowPrivateAccess= "true"))
	TObjectPtr<UBoxComponent> OverlapBox1;

	/** Scene component for organizing overlap-related components */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Default", meta=(AllowPrivateAccess= "true"))
	TObjectPtr<USceneComponent> OverlapFolder;

	/** Scene component for organizing geometry components */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Default", meta=(AllowPrivateAccess= "true"))
	TObjectPtr<USceneComponent> GeometryFolder;

	/** Arrow component indicating the forward direction of the room */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Default", meta=(AllowPrivateAccess= "true"))
	TObjectPtr<UArrowComponent> Arrow;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Room", meta=(AllowPrivateAccess= "true"))
	UProceduralMeshComponent* Floor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Room", meta=(AllowPrivateAccess= "true"))
	TSubclassOf<AActor> ChestClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Room", meta=(AllowPrivateAccess= "true"))
	TSubclassOf<AActor> BarrelClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Room", meta=(AllowPrivateAccess= "true"))
	TSubclassOf<AActor> BrazierClass;

	

	float GridHeight;

	FVector TopLeft;
	FVector BottomRight;
	
	UPROPERTY(EditAnywhere, Category="GridSize")
	int32 GridSizeX;
	UPROPERTY(EditAnywhere, Category="GridSize")
	int32 GridSizeY;
	UPROPERTY(EditAnywhere, Category="GridSize")
	float VertexSpacing;

	void SpawnObject(UClass* ItemToSpawn);
	void ClearObjects();
	void CreateGrid();
	void ClearGrid();

	FVector GetRandomPointInSquare(const FVector& UpperLeft, const FVector& LowerRight);

	void PlacePointOnGrid();
};
