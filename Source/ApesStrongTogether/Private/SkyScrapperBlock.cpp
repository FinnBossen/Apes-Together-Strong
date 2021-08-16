// Fill out your copyright notice in the Description page of Project Settings.


#include "SkyScrapperBlock.h"

#include "Net/UnrealNetwork.h"

#define GET_VARIABLE_NAME(Variable) (#Variable)

// Sets default values
ASkyScrapperBlock::ASkyScrapperBlock()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;

	CurrentMesh = nullptr;

	Lives = 10;
}



// Called when the game starts or when spawned
void ASkyScrapperBlock::BeginPlay()
{
	Super::BeginPlay();
}

void ASkyScrapperBlock::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//Do replications on properties
	DOREPLIFETIME(ASkyScrapperBlock , CurrentMesh);
}

void ASkyScrapperBlock::ChangeDestructMesh()
{
	int const Size = ChosenDestructMeshes.Num();
	if(Size == 0)
	{
		return;
	}

	CurrentMesh = ChosenDestructMeshes[0];
}


// Called every frame
void ASkyScrapperBlock::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

EDestructMeshes ASkyScrapperBlock::GetDestructType(const FString ObjectName)
{
	
	if(ObjectName.Find("RampageBuildingBlock1"))
	{
		return EDestructMeshes::RampageBuildingBlock1;
	}

	if(ObjectName.Find("RampageBuildingBlock2"))
	{
		return EDestructMeshes::RampageBuildingBlock2;	
	}

	if(ObjectName.Find("RampageBuildingBlock3"))
	{
		return EDestructMeshes::RampageBuildingBlock3;
	}

	if(ObjectName.Find("RampageBuildingBlock4"))
	{
		return EDestructMeshes::RampageBuildingBlock4;
	}

	if(ObjectName.Find("RampageBuildingBlockDoor"))
	{
		return EDestructMeshes::RampageBuildingBlockDoor;
	}

	return EDestructMeshes::RampageBuildingBlock1;
}

void ASkyScrapperBlock::IsHit_Implementation(float Damage)
{
	if(Lives <= 0) return;
	Lives = Lives - Damage;
	const FString DamageStr = FString::SanitizeFloat(Damage);
	const FString LivesStr = FString::SanitizeFloat(Lives);
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, FString::Printf(TEXT("Has Lives lost Damage %s, now has only %s lives"), *DamageStr,*LivesStr));
	ChangeDestructMesh();
}



