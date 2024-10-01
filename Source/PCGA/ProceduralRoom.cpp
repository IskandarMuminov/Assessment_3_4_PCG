#include "ProceduralRoom.h"

// Sets default values
AProceduralRoom::AProceduralRoom()
{
    PrimaryActorTick.bCanEverTick = true;

    Floor = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Floor Component"));
    SetRootComponent(Floor);
}

void AProceduralRoom::BeginPlay()
{
    Super::BeginPlay();
}

void AProceduralRoom::Tick(float DeltaTime)
{
    
}

bool AProceduralRoom::ShouldTickIfViewportsOnly() const
{
    return true;
}

