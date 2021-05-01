// Fill out your copyright notice in the Description page of Project Settings.


#include "NewSkyScrapperFloorTrigger.h"

// Sets default values
ANewSkyScrapperFloorTrigger::ANewSkyScrapperFloorTrigger()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ANewSkyScrapperFloorTrigger::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ANewSkyScrapperFloorTrigger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ANewSkyScrapperFloorTrigger::IsTriggered_Implementation()
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Orange, TEXT("New Floor is created"));
}

