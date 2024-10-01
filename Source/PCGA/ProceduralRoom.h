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
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Room Properties")
	int32 RoomWidth = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Room Properties")
	int32 RoomHeight = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Room Properties")
	float TileSize = 100.0f;

	UPROPERTY(EditAnywhere)
	bool bShouldRegenerate;
	
	UFUNCTION(BlueprintCallable, Category = "Generation")
	void GenerateRoom();

	UFUNCTION(BlueprintCallable, Category = "Generation")
	void ClearRoom();

	void DrawDebugLineBetweenVertices(int32 X, int32 Y) const;

private:
	
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UProceduralMeshComponent* ProceduralMesh;
	
	TArray<FVector> Vertices;
	TArray<int32> Triangles;
	TArray<FVector2D> UVCoords;

};
