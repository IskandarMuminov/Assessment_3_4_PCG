// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MasterRoom.generated.h"

class UStaticMeshComponent;
class USceneComponent;

UCLASS()
class PCGA_API AMasterRoom : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMasterRoom();

	// Function to get the available exits in the room
	UFUNCTION(BlueprintCallable, Category = "Room")
	TArray<USceneComponent*> GetExits() const;

	// Function to get the components that are overlapped
	UFUNCTION(BlueprintCallable, Category = "Room")
	TArray<UPrimitiveComponent*> GetOverlappedComponents() const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Components for the room
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Room")
	UStaticMeshComponent* RoomMesh;

	// Array of exit points, which can be scene components
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Room")
	TArray<USceneComponent*> ExitPoints;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Room")
	void AddExit(USceneComponent* ExitPoint);

};
