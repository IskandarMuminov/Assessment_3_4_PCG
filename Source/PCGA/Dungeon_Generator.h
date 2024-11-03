// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Dungeon_Generator.generated.h"

class AMasterRoom;

UCLASS()
class PCGA_API ADungeon_Generator : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADungeon_Generator();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dungeon")
	TSubclassOf<AActor> DungeonRoom1Class;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dungeon")
	TSubclassOf<AActor> DungeonRoom2Class;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dungeon")
	TSubclassOf<AActor> DungeonRoom3Class;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dungeon")
	TSubclassOf<AActor> DungeonRoom4Class;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dungeon")
	TSubclassOf<AActor> DungeonRoom5Class;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	FTimerHandle OverlapCheckTimerHandle;

	void CheckForOverlaps();

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/** Default root scene component for the actor */
    UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Default")
    TObjectPtr<USceneComponent> DefaultSceneRoot;

    /** List of exits from the current room, used to determine where the next room can be spawned */
    UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Default")
    TArray<USceneComponent*> ExitsList;

    /** The exit point selected for spawning the next room */
    UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Default")
    TObjectPtr<USceneComponent> SelectedExitPoint;

    /** List of possible room types that can be spawned in the dungeon */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Default")
	TArray<UClass*> RoomList;

    /** Reference to the most recently spawned room in the dungeon */
    UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Default")
    TObjectPtr<AMasterRoom> LatestRoom;

    /** Number of rooms generated in the dungeon so far */
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Default", meta = (ExposeOnSpawn = "true"))
    int32 RoomAmount;

	// Reference to the class used for spawning rooms
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Default")
	TSubclassOf<AMasterRoom> RoomToSpawnClass;

	// Reference to the class used for spawning end walls
	//UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Default")
	//TSubclassOf<AEndWallClass> EndWallClass;

	// Function to spawn the next room
	//UFUNCTION(BlueprintCallable, Category = "Dungeon")
	//void SpawnNextRoom();

	// Function to close any open exits after generation
	//UFUNCTION(BlueprintCallable, Category = "Dungeon")
	//void CloseHoles();

    /** List of components that overlap with the latest room, used to check for collisions */
    UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Default")
    TArray<UPrimitiveComponent*> OverlappedList;

    /** Whether the dungeon generation is complete */
    UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Default")
    bool bDungeonComplete;

    /** Timer handle used for controlling the delay between room generation steps */
    UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Default")
    FTimerHandle TimerHandle;

    /** Maximum time allowed for dungeon generation */
    UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Default")
    double MaxDungeonTime;

	UFUNCTION(BlueprintCallable, Category = "Dungeon")
	void GenerateDungeon();

    /** Random seed value used to initialize the dungeon generation's randomness */
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Default", meta = (ExposeOnSpawn = "true"))
    int32 Seed;

    /** Random stream object used to generate random values based on the seed */
    UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Default")
    FRandomStream Stream;

private:
	bool DungeonComplete;

};
