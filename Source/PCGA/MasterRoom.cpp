// Fill out your copyright notice in the Description page of Project Settings.


#include "MasterRoom.h"

#include "ProceduralMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"
#include "Components/PrimitiveComponent.h"
#include "Kismet/KismetSystemLibrary.h"

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

	TopLeft = FVector(0.f);
	BottomRight = FVector(1000.f, 1000.f , 0.f );
	GridHeight = 1.f;
	ChestRadius = 100.f;

}

// Called when the game starts or when spawned
void AMasterRoom::BeginPlay()
{
	Super::BeginPlay();
	ClearGrid();
	CreateGrid();
	ClearObjects();
	PlacePointOnGrid();
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

	if (bShouldRegenerate)
	{
		ClearGrid();
		CreateGrid();
		ClearObjects();
		PlacePointOnGrid();
		bShouldRegenerate = false;
	}

}

bool AMasterRoom::ShouldTickIfViewportsOnly() const
{
	return true;
}

void AMasterRoom::AddExit(USceneComponent* ExitPoint)
{
	if (ExitPoint)
	{
		ExitPoints.Add(ExitPoint);
	}
}

void AMasterRoom::SpawnObject(UClass* ItemToSpawn)
{
	float XCoordinate = FMath::FRandRange(-500.f , 500.f);
	float YCoordinate = FMath::FRandRange(-500.f , 500.f);

	float Yaw = FMath::FRandRange(0.f , 360.f);
    
	FVector Location (XCoordinate, YCoordinate,0.f);
	FRotator Rotation (0.f,Yaw,0.f);
	GetWorld()->SpawnActor<AActor>(ItemToSpawn, Location, Rotation);
}

void AMasterRoom::ClearObjects()
{
	for (AActor* SpawnedObject : SpawnedObjects)
	{
		if (SpawnedObject)
		{
			SpawnedObject->Destroy(); 
		}
	}
	SpawnedObjects.Empty();
}

void AMasterRoom::CreateGrid()
{
	for(int32 Y = 0; Y < GridSizeY; Y++)
	{
		for (int32 X= 0; X < GridSizeX; X ++)
		{
			//Add vertices
			FVector VertexLocation = FVector(X* VertexSpacing, Y*VertexSpacing, 0.f);
			Vertices.Add(VertexLocation);
			UVCoords.Add(FVector2D(X,Y));
			//DrawDebugSphere(GetWorld(), VertexLocation, 10.0f, 3, FColor::Blue, true, -1, 0, 5.0f);
            
			//Draw a line on X axis
			if (X < GridSizeX - 1)
			{
				FVector NextInRow = FVector((X + 1) * VertexSpacing, Y * VertexSpacing, 0.f);
				//DrawDebugLine(GetWorld(), VertexLocation, NextInRow, FColor::Green, true, -1, 0, 5.0f);
			}
			//Draw a line on Y axis
			if (Y < GridSizeY - 1)
			{
				FVector NextInColumn = FVector(X * VertexSpacing, (Y + 1) * VertexSpacing, 0.f);
				//DrawDebugLine(GetWorld(), VertexLocation, NextInColumn, FColor::Green, true, -1, 0, 5.0f);
			}

			Triangles.Add(Y * GridSizeX + X);
			Triangles.Add((Y+1) * GridSizeX + X);
			Triangles.Add(Y * GridSizeX + X+1);
			Triangles.Add(Y * GridSizeX + X+1);
			Triangles.Add((Y+1) * GridSizeX + X);
			Triangles.Add((Y+1) * GridSizeX + X+1);
		}
	}
}

void AMasterRoom::ClearGrid()
{
	Vertices.Empty();
	Triangles.Empty();
	UVCoords.Empty();
	UKismetSystemLibrary::FlushPersistentDebugLines(GetWorld());
}

FVector AMasterRoom::GetRandomPointInSquare(const FVector& UpperLeft, const FVector& LowerRight)
{
	float RandomX = FMath::FRandRange(UpperLeft.X, LowerRight.X);
	float RandomY = FMath::FRandRange(UpperLeft.Y, LowerRight.Y);

	return FVector(RandomX, RandomY, 0.f);
}

void AMasterRoom::PlacePointOnGrid()
{
	for (int32 i = 0; i < GridSizeX - 1; i++)
    {
        for (int32 j = 0; j < GridSizeY - 1; j++)
        {
            // Grid boundaries 
            FVector UpperLeft(i * VertexSpacing + ChestRadius, j * VertexSpacing + ChestRadius, GridHeight);
            FVector LowerRight((i * VertexSpacing + VertexSpacing) - ChestRadius,
                (j * VertexSpacing + VertexSpacing) - ChestRadius, GridHeight);

            // Random point for the objects 
            FVector RandomChestPoint = GetRandomPointInSquare(UpperLeft, LowerRight);
            FVector RandomBarrelPoint = GetRandomPointInSquare(UpperLeft, LowerRight);
            FVector RandomBrazierPoint = GetRandomPointInSquare(UpperLeft, LowerRight);
            float RandomYaw = FMath::FRandRange(0.f, 360.f);

            
            // Clamp the barrel and brazier positions within grid bounds to avoid overlapping with edges
            RandomBarrelPoint.X = FMath::Clamp(RandomBarrelPoint.X, TopLeft.X, BottomRight.X - BarrelRadius);
            RandomBarrelPoint.Y = FMath::Clamp(RandomBarrelPoint.Y, TopLeft.Y, BottomRight.Y - BarrelRadius);

            RandomBrazierPoint.X = FMath::Clamp(RandomBrazierPoint.X, TopLeft.X, BottomRight.X - BrazierRadius);
            RandomBrazierPoint.Y = FMath::Clamp(RandomBrazierPoint.Y, TopLeft.Y, BottomRight.Y - BrazierRadius);
            
            // Spawn objects on a random position 
            AActor* SpawnedChest = GetWorld()->SpawnActor<AActor>(ChestClass, RandomChestPoint, FRotator(0.f, RandomYaw, 0.f));
            if (SpawnedChest)
            {
                SpawnedObjects.Add(SpawnedChest);
            }
            AActor* SpawnedBarrel = GetWorld()->SpawnActor<AActor>(BarrelClass, RandomBarrelPoint, FRotator(0.f, RandomYaw, 0.f));
            if (SpawnedBarrel)
            {
                SpawnedObjects.Add(SpawnedBarrel);
            }
            AActor* SpawnedBrazier = GetWorld()->SpawnActor<AActor>(BrazierClass, RandomBrazierPoint, FRotator(0.f, RandomYaw, 0.f));
            if (SpawnedBrazier)
            {
                SpawnedObjects.Add(SpawnedBrazier);
            }
        }
    }
}
