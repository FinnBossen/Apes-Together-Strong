// Fill out your copyright notice in the Description page of Project Settings.


#include "SkyScrapperBlock.h"

#include "Net/UnrealNetwork.h"

#define GET_VARIABLE_NAME(Variable) (#Variable)

// Sets default values
ASkyScrapperBlock::ASkyScrapperBlock()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	


}

// Called when the game starts or when spawned
void ASkyScrapperBlock::BeginPlay()
{
	Super::BeginPlay();
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



