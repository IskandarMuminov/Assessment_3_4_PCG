#include "ProceduralRoom.h"
#include "DrawDebugHelpers.h"
#include "KismetProceduralMeshLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values
AProceduralRoom::AProceduralRoom()
{
    PrimaryActorTick.bCanEverTick = true;

    Floor= CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("Floor Component"));
    SetRootComponent(Floor);
    
    TopLeft = FVector(0.f);
    BottomRight = FVector(1000.f, 1000.f , 0.f );
    GridHeight = 1.f;
    ChestRadius = 100.f;
}

void AProceduralRoom::BeginPlay()
{
    Super::BeginPlay();
}

void AProceduralRoom::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    
    //Code to regenerate the grid 
    if (bShouldRegenerate)
    {
        ClearGrid();
        CreateGrid();
        ClearObjects();
        PlacePointOnGrid();
        bShouldRegenerate = false;
    }
}

bool AProceduralRoom::ShouldTickIfViewportsOnly() const
{
    return true;
}

    //Spawn interior objects on random position
void AProceduralRoom::SpawnObject(UClass* ItemToSpawn)
{
    float XCoordinate = FMath::FRandRange(-500.f , 500.f);
    float YCoordinate = FMath::FRandRange(-500.f , 500.f);

    float Yaw = FMath::FRandRange(0.f , 360.f);
    
    FVector Location (XCoordinate, YCoordinate,0.f);
    FRotator Rotation (0.f,Yaw,0.f);
    GetWorld()->SpawnActor<AActor>(ItemToSpawn, Location, Rotation);
}
    //Clear objects
void AProceduralRoom::ClearObjects()
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


    //Create Grid
void AProceduralRoom::CreateGrid()
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
            
            //Draw the line on X axis
            if (X < GridSizeX - 1)
            {
                FVector NextInRow = FVector((X + 1) * VertexSpacing, Y * VertexSpacing, 0.f);
                //DrawDebugLine(GetWorld(), VertexLocation, NextInRow, FColor::Green, true, -1, 0, 5.0f);
            }
            //Draw the line on Y axis
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

    //Create procedural mesh on the grid
    if (Floor)
    {
        Floor->CreateMeshSection(0, Vertices, Triangles, TArray<FVector>(), UVCoords,
            TArray<FColor>(), TArray<FProcMeshTangent>(), true);
    }
}
    //Clear the grid
void AProceduralRoom::ClearGrid()
{
    Vertices.Empty();
    Triangles.Empty();
    UVCoords.Empty();
    if (Floor)
    {
        Floor->ClearMeshSection(0);
    }
    UKismetSystemLibrary::FlushPersistentDebugLines(GetWorld());
}

    //Get random point within the grid
FVector AProceduralRoom::GetRandomPointInSquare(const FVector& UpperLeft, const FVector& LowerRight)
{
    float RandomX = FMath::FRandRange(UpperLeft.X, LowerRight.X);
    float RandomY = FMath::FRandRange(UpperLeft.Y, LowerRight.Y);

    return FVector(RandomX, RandomY, 0.f);
}
    //Place the point to spawn a chest within the grid
void AProceduralRoom::PlacePointOnGrid()
{
    for(int32 i =0; i<GridSizeX - 1; i++)
    {
        for(int32 j = 0; j<GridSizeY - 1; j++)
        {
            FVector UpperLeft(i * VertexSpacing + ChestRadius, j * VertexSpacing + ChestRadius, GridHeight);
            FVector LowerRight((i* VertexSpacing + VertexSpacing)- ChestRadius,
                (j* VertexSpacing + VertexSpacing) - ChestRadius, GridHeight);

            FVector RandomPointInSquare = GetRandomPointInSquare(UpperLeft,LowerRight);

            float BarrelSpawnOffsetX = FMath::RandRange(100.f, 300.f);
            float BrazierSpawnOffsetX= FMath::RandRange(300.f, 400.f);
            float BarrelSpawnOffsetY= FMath::RandRange(100.f, 300.f);
            float BrazierSpawnOffsetY= FMath::RandRange(300.f, 400.f);
            
            FVector BarrelPosition = RandomPointInSquare + FVector(BarrelRadius + BarrelSpawnOffsetX, BarrelSpawnOffsetY, 0.f);  
            FVector BrazierPosition = RandomPointInSquare + FVector(-(BrazierRadius + BrazierSpawnOffsetX),BrazierSpawnOffsetY, 0.f);
            
            float RandomYaw = FMath::FRandRange(0.f,360.f);
            
            AActor* SpawnedChest = GetWorld()->SpawnActor<AActor>(ChestClass, RandomPointInSquare, FRotator(0.f, RandomYaw, 0.f));
            if (SpawnedChest)
            {
                SpawnedObjects.Add(SpawnedChest); //Add spawned chest to the list
            }

            //Spawn barrel at the calculated barrel position
            AActor* SpawnedBarrel = GetWorld()->SpawnActor<AActor>(BarrelClass, BarrelPosition, FRotator(0.f, RandomYaw, 0.f));
            if (SpawnedBarrel)
            {
                SpawnedObjects.Add(SpawnedBarrel); //Add spawned barrel to the list
            }

            //Spawn brazier at the calculated brazier position
            AActor* SpawnedBrazier = GetWorld()->SpawnActor<AActor>(BrazierClass, BrazierPosition, FRotator(0.f, RandomYaw, 0.f));
            if (SpawnedBrazier)
            {
                SpawnedObjects.Add(SpawnedBrazier); //Add spawned brazier to the list
            }
        }
    }
}

