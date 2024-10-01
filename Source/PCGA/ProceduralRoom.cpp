#include "ProceduralRoom.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Cell.h"

// Sets default values
AProceduralRoom::AProceduralRoom()
{
    PrimaryActorTick.bCanEverTick = true;

    ProceduralMesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("Procedural Mesh"));
    SetRootComponent(ProceduralMesh);
}

void AProceduralRoom::BeginPlay()
{
    Super::BeginPlay();
}

void AProceduralRoom::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    if (bShouldRegenerate)
    {
        ClearRoom();
        GenerateRoom();
        bShouldRegenerate = false;
    }
}

bool AProceduralRoom::ShouldTickIfViewportsOnly() const
{
    return true;
}

void AProceduralRoom::ClearRoom()
{
    Vertices.Empty();
    Triangles.Empty();
    UVCoords.Empty();

    if (ProceduralMesh)
    {
        ProceduralMesh->ClearMeshSection(0);
    }
    UKismetSystemLibrary::FlushPersistentDebugLines(GetWorld());
}

void AProceduralRoom::GenerateRoom()
{
    for (int32 Y = 0; Y < RoomHeight; Y++)
    {
        for (int32 X = 0; X < RoomWidth; X++)
        {
            CellSideTag CellSide = DetermineCellSide(X, Y);
            Cell NewCell(FVector(X * TileSize, Y * TileSize, 0.0f), CellSide); // Use Cell class here
            AddTile(NewCell);  
        }
    }

    if (ProceduralMesh)
    {
        TArray<FVector> Normals;
        TArray<FProcMeshTangent> Tangents;
        ProceduralMesh->CreateMeshSection(0, Vertices, Triangles, Normals, UVCoords, TArray<FColor>(), Tangents, true);
    }
}

void AProceduralRoom::AddTile(const Cell& InCell)
{
    int32 VertexIndex = Vertices.Num();

    FVector Pos = InCell.Position;  // cell pos
    
    Vertices.Add(Pos);                                                           // bottom-left
    Vertices.Add(Pos + FVector(TileSize, 0.0f, 0.0f));            // bottom-right
    Vertices.Add(Pos + FVector(0.0f, TileSize, 0.0f));            // top-left
    Vertices.Add(Pos + FVector(TileSize, TileSize, 0.0f));        // top-right

    //add triangles
    Triangles.Add(VertexIndex + 0);
    Triangles.Add(VertexIndex + 2);
    Triangles.Add(VertexIndex + 1);
    Triangles.Add(VertexIndex + 1);
    Triangles.Add(VertexIndex + 2);
    Triangles.Add(VertexIndex + 3);
    
    UVCoords.Add(FVector2D(0.0f, 0.0f));
    UVCoords.Add(FVector2D(1.0f, 0.0f));
    UVCoords.Add(FVector2D(0.0f, 1.0f));
    UVCoords.Add(FVector2D(1.0f, 1.0f));
    
    DrawDebugLineBetweenVertices(InCell.Position.X / TileSize, InCell.Position.Y / TileSize);
}



CellSideTag AProceduralRoom::DetermineCellSide(int32 X, int32 Y) const
{
    if (Y == 0) return CellSideTag::South;
    if (Y == RoomHeight - 1) return CellSideTag::North;
    if (X == 0) return CellSideTag::West;
    if (X == RoomWidth - 1) return CellSideTag::East;

    return CellSideTag::Center; //return middle cell
}

void AProceduralRoom::DrawDebugLineBetweenVertices(int32 X, int32 Y) const
{
    FVector Start = FVector(X * TileSize, Y * TileSize, 0.0f);
    FVector End = FVector((X + 1) * TileSize, Y * TileSize, 0.0f);
    DrawDebugLine(GetWorld(), Start, End, FColor::Green, true, -1.0f, 0, 5.0f);
}
