// Fill out your copyright notice in the Description page of Project Settings.


#include "Door.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/TimelineComponent.h"
#include "Curves/CurveFloat.h"

// Sets default values
ADoor::ADoor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create Proximity Box
	ProximityBox = CreateDefaultSubobject<UBoxComponent>(TEXT("ProximityBox"));
	RootComponent = ProximityBox;

	// Create Door Mesh
	DoorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorMesh"));
	DoorMesh->SetupAttachment(RootComponent);

	// Create Timeline Component
	DoorTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("DoorTimeline"));

	// Bind overlap events
	ProximityBox->OnComponentBeginOverlap.AddDynamic(this, &ADoor::OnPlayerEnter);
	ProximityBox->OnComponentEndOverlap.AddDynamic(this, &ADoor::OnPlayerExit);

}

// Called when the game starts or when spawned
void ADoor::BeginPlay()
{
	Super::BeginPlay();

	// Initialize door state
	DoorProgress = 0.0f;

	if (DoorCurve)
	{
		FOnTimelineFloat TimelineProgress;
		TimelineProgress.BindUFunction(this, TEXT("OpenDoor"));
		DoorTimeline->AddInterpFloat(DoorCurve, TimelineProgress);
	}
	
}

// Called every frame
void ADoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADoor::OnPlayerEnter(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	// Check if player overlaps
	if (OtherActor && OtherActor != this)
	{
		OpenDoor();
		ChangeDoorColor(true);
	}
}

void ADoor::OnPlayerExit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	CloseDoor();
	ChangeDoorColor(false);
}

void ADoor::OpenDoor()
{
	// Start door animation
	DoorTimeline->PlayFromStart();
}

void ADoor::CloseDoor()
{
	// Reverse door animation
	DoorTimeline->Reverse();
}

void ADoor::ChangeDoorColor(bool bPlayerNear)
{
	if (bPlayerNear)
	{
		DoorMesh->SetMaterial(0, PlayerNearMaterial); // Change to "near" color
	}
	else
	{
		DoorMesh->SetMaterial(0, DefaultMaterial); // Revert to default
	}
}
