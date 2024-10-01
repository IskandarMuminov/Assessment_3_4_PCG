#include "ProceduralRoom.h"
#include "DrawDebugHelpers.h"

// Sets default values
AProceduralRoom::AProceduralRoom()
{
    PrimaryActorTick.bCanEverTick = true;

    Floor= CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("Floor Component"));
    SetRootComponent(Floor);

    GridSizeX = 5;
    GridSizeY = 5;
    SquareWidth = 200;

    TopLeft = FVector(0.f);
    BottomRight = FVector(1000.f, 1000.f , 0.f );
    GridHeight = 1.f;

    RoomLength= 1000.f;
    RoomWidth = 1000.f;
}

void AProceduralRoom::BeginPlay()
{
    Super::BeginPlay();
    CreateGrid();
    SpawnObject(ChestClass);
    
}

void AProceduralRoom::Tick(float DeltaTime)
{
    
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
    for (int32 i = 0; i< GridSizeX + 1; i ++)
    {
        FVector Start = TopLeft + FVector(i * SquareWidth, 0.f, GridHeight);
        FVector End = Start * FVector(0.f, RoomLength, GridHeight);
        DrawDebugLine(GetWorld(), Start, End, FColor::Green, true);
    }
}

