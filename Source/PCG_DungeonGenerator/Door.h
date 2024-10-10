// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Door.generated.h"

class UBoxComponent;
class UStaticMeshComponent;
class UTimelineComponent;

UCLASS()
class PCG_DUNGEONGENERATOR_API ADoor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADoor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Box Component to detect overlaps
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Door")
	TObjectPtr<UBoxComponent> ProximityBox;

	// Static Mesh for the door
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Door")
	TObjectPtr<UStaticMeshComponent> DoorMesh;

	// Timeline to animate the door
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Door")
	TObjectPtr<UTimelineComponent> DoorTimeline;

	// Function to open the door
	UFUNCTION()
	void OpenDoor();

	// Function to close the door
	UFUNCTION()
	void CloseDoor();

	// Function to change door color when the player is near
	UFUNCTION()
	void ChangeDoorColor(bool bPlayerNear);

	// Overlap Begin function
	UFUNCTION()
	void OnPlayerEnter(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	// Overlap End function
	UFUNCTION()
	void OnPlayerExit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY(EditDefaultsOnly, Category = "Materials")
	UMaterial* PlayerNearMaterial;

	UPROPERTY(EditDefaultsOnly, Category = "Materials")
	UMaterial* DefaultMaterial;

private:
	// Door animation progress (0 - closed, 1 - open)
	UPROPERTY()
	float DoorProgress;

	// Curve for smooth door opening animation
	UPROPERTY(EditDefaultsOnly, Category = "Door")
	TObjectPtr<UCurveFloat> DoorCurve;

};
