// Fill out your copyright notice in the Description page of Project Settings.


#include "SkyScrapperBuilding.h"

#include "Net/UnrealNetwork.h"

// Sets default values
ASkyScrapperBuilding::ASkyScrapperBuilding()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	LatestFloorBlueprintPosition = FVector(0, 0, 0);
	
	
}


void ASkyScrapperBuilding::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//Do replications on properties
	DOREPLIFETIME(ASkyScrapperBuilding, LatestFloorBlueprintPosition);
}

// Called when the game starts or when spawned
void ASkyScrapperBuilding::BeginPlay()
{
	Super::BeginPlay();
}

void ASkyScrapperBuilding::CreateInitialFloors()
{
	ASkyScrapperFloor* SpawnedActor = static_cast<ASkyScrapperFloor*>(GetWorld()->SpawnActor(SkyScrapperFloor));
	SpawnedActor->SetActorRelativeLocation(LatestFloorBlueprintPosition);
	for (int i = 0; i < StartFloors; i++)
	{
		TriggerNewFloor();
	}
}

void ASkyScrapperBuilding::CreateNewFloor()
{
	LatestFloorBlueprintPosition.Z = LatestFloorBlueprintPosition.Z + FloorOffset;
	ASkyScrapperFloor* SpawnedActor = static_cast<ASkyScrapperFloor*>(GetWorld()->SpawnActor(SkyScrapperFloor));

	SpawnedActor->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
	SpawnedActor->SetActorRelativeLocation(LatestFloorBlueprintPosition);
}

void ASkyScrapperBuilding::TriggerNewFloor()
{
	CreateNewFloor();
	ChangeTriggerPosition();
}

// Called every frame
void ASkyScrapperBuilding::Tick(float const DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASkyScrapperBuilding::ChangeTriggerPosition_Implementation()
{
	
}
