#include "ProceduralInterior.h"
#include "ProceduralMeshComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameFramework/Actor.h"

// Sets default values for this component's properties
UProceduralInterior::UProceduralInterior()
{
    PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UProceduralInterior::BeginPlay()
{
    Super::BeginPlay();
    
    SetBoundsToParent();
    CreateGrid();
    PlaceObjectsOnGrid();
}

// Dynamically set bounds based on parent actor's size
void UProceduralInterior::SetBoundsToParent()
{
    if (AActor* Owner = GetOwner())
    {
        FVector Origin;
        FVector BoxExtent;
        
        Owner->GetActorBounds(false, Origin, BoxExtent);  // Get the bounds of the parent actor
        
        TopLeft = Origin - BoxExtent;
        BottomRight = Origin + BoxExtent;
    }
}

void UProceduralInterior::TickComponent(float DeltaTime, ELevelTick TickType,
    FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UProceduralInterior::CreateGrid()
{
    for (int32 Y = 0; Y < GridSizeY; Y++)
    {
        for (int32 X = 0; X < GridSizeX; X++)
        {
            FVector VertexLocation = FVector(X * VertexSpacing, Y * VertexSpacing, 0.f);
            Vertices.Add(VertexLocation);
            UVCoords.Add(FVector2D(X, Y));

            Triangles.Add(Y * GridSizeX + X);
            Triangles.Add((Y + 1) * GridSizeX + X);
            Triangles.Add(Y * GridSizeX + X + 1);
            Triangles.Add(Y * GridSizeX + X + 1);
            Triangles.Add((Y + 1) * GridSizeX + X);
            Triangles.Add((Y + 1) * GridSizeX + X + 1);
        }
    }
}

void UProceduralInterior::PlaceObjectsOnGrid()
{
    for (int32 i = 0; i < GridSizeX - 1; i++)
    {
        for (int32 j = 0; j < GridSizeY - 1; j++)
        {
            FVector UpperLeft = FVector(i * VertexSpacing + ChestRadius, j * VertexSpacing + ChestRadius, GridHeight);
            FVector LowerRight = FVector((i * VertexSpacing + VertexSpacing) - ChestRadius,
                (j * VertexSpacing + VertexSpacing) - ChestRadius, GridHeight);

            FVector RandomChestPoint = GetRandomPointInSquare(UpperLeft, LowerRight);
            FVector RandomBarrelPoint = GetRandomPointInSquare(UpperLeft, LowerRight);
            FVector RandomBrazierPoint = GetRandomPointInSquare(UpperLeft, LowerRight);

            RandomBarrelPoint.X = FMath::Clamp(RandomBarrelPoint.X, TopLeft.X, BottomRight.X - BarrelRadius);
            RandomBarrelPoint.Y = FMath::Clamp(RandomBarrelPoint.Y, TopLeft.Y, BottomRight.Y - BarrelRadius);

            RandomBrazierPoint.X = FMath::Clamp(RandomBrazierPoint.X, TopLeft.X, BottomRight.X - BrazierRadius);
            RandomBrazierPoint.Y = FMath::Clamp(RandomBrazierPoint.Y, TopLeft.Y, BottomRight.Y - BrazierRadius);

            float RandomYaw = FMath::FRandRange(0.f, 360.f);

            SpawnObjectAtLocation(ChestClass, RandomChestPoint, RandomYaw);
            SpawnObjectAtLocation(BarrelClass, RandomBarrelPoint, RandomYaw);
            SpawnObjectAtLocation(BrazierClass, RandomBrazierPoint, RandomYaw);
        }
    }
}

void UProceduralInterior::SpawnObjectAtLocation(UClass* ItemToSpawn, const FVector& Location, float Yaw)
{
    if (!ItemToSpawn) return;

    FRotator Rotation(0.f, Yaw, 0.f);
    AActor* SpawnedObject = GetWorld()->SpawnActor<AActor>(ItemToSpawn, Location, Rotation);
    if (SpawnedObject)
    {
        SpawnedObjects.Add(SpawnedObject);
    }
}

FVector UProceduralInterior::GetRandomPointInSquare(const FVector& UpperLeft, const FVector& LowerRight)
{
    float RandomX = FMath::FRandRange(UpperLeft.X, LowerRight.X);
    float RandomY = FMath::FRandRange(UpperLeft.Y, LowerRight.Y);
    return FVector(RandomX, RandomY, 0.f);
}
