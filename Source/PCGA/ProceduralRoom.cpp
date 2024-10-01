#include "ProceduralRoom.h"
#include "Kismet/KismetSystemLibrary.h"

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

