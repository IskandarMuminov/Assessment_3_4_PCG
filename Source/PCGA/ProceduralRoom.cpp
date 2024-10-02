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

    RoomLength= 1000.f;
    RoomWidth = 1000.f;
}

void AProceduralRoom::BeginPlay()
{
    Super::BeginPlay();
    SpawnObject(ChestClass);
    PlacePointOnGrid();
}

void AProceduralRoom::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (bShouldRegenerate)
    {
        ClearGrid();
        CreateGrid();
        
        bShouldRegenerate = false;
    }
}

bool AProceduralRoom::ShouldTickIfViewportsOnly() const
{
    return true;
}

void AProceduralRoom::SpawnObject(UClass* ItemToSpawn)
{
    float XCoordinate = FMath::FRandRange(-500.f , 500.f);
    float YCoordinate = FMath::FRandRange(-500.f , 500.f);

    float Yaw = FMath::FRandRange(0.f , 360.f);
    
    FVector Location (XCoordinate, YCoordinate,0.f);
    FRotator Rotation (0.f,Yaw,0.f);
    GetWorld()->SpawnActor<AActor>(ItemToSpawn, Location, Rotation);
}

void AProceduralRoom::CreateGrid()
{
    for(int32 Y = 0; Y < GridSizeY; Y++)
    {
        for (int32 X= 0; X < GridSizeX; X ++)
        {
            FVector VertexLocation = FVector(X* VertexSpacing, Y*VertexSpacing, 0.f);
            Vertices.Add(VertexLocation);
            UVCoords.Add(FVector2D(X,Y));
            DrawDebugSphere(GetWorld(), VertexLocation, 10.0f, 3, FColor::Blue, true, -1, 0, 5.0f);

            if (X < GridSizeX - 1)
            {
                FVector NextInRow = FVector((X + 1) * VertexSpacing, Y * VertexSpacing, 0.f);
                DrawDebugLine(GetWorld(), VertexLocation, NextInRow, FColor::Green, true, -1, 0, 5.0f);
            }
            
            if (Y < GridSizeY - 1)
            {
                FVector NextInColumn = FVector(X * VertexSpacing, (Y + 1) * VertexSpacing, 0.f);
                DrawDebugLine(GetWorld(), VertexLocation, NextInColumn, FColor::Green, true, -1, 0, 5.0f);
            }

            Triangles.Add(Y * GridSizeX + X);
            Triangles.Add((Y+1) * GridSizeX + X);
            Triangles.Add(Y * GridSizeX + X+1);
            Triangles.Add(Y * GridSizeX + X+1);
            Triangles.Add((Y+1) * GridSizeX + X);
            Triangles.Add((Y+1) * GridSizeX + X+1);
        }
    }


    if (Floor)
    {
        Floor->CreateMeshSection(0, Vertices, Triangles, TArray<FVector>(), UVCoords,
            TArray<FColor>(), TArray<FProcMeshTangent>(), true);
    }
}

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

FVector AProceduralRoom::GetRandomPointInSquare(const FVector& UpperLeft, const FVector& LowerRight)
{
    float RandomX = FMath::FRandRange(UpperLeft.X, LowerRight.X);
    float RandomY = FMath::FRandRange(UpperLeft.Y, LowerRight.Y);

    return FVector(RandomX, RandomY, 0.f);
}

void AProceduralRoom::PlacePointOnGrid()
{
    for(int32 i =0; i<GridSizeX; i++)
    {
        for(int32 j = 0; i<GridSizeY; j++)
        {
            FVector UpperLeft(i*VertexSpacing, j *VertexSpacing, GridHeight);
            FVector LowerRight(i*VertexSpacing + VertexSpacing, j *VertexSpacing, GridHeight);

            FVector RandomPointInSquare = GetRandomPointInSquare(UpperLeft,LowerRight);
            DrawDebugPoint(GetWorld(),RandomPointInSquare, 5.f, FColor::Red, true);
        }
    }
}

