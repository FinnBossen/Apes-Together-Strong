// Fill out your copyright notice in the Description page of Project Settings.


#include "SkyScrapperBlock.h"

#include <unordered_map>

#include "Net/UnrealNetwork.h"

#define GET_VARIABLE_NAME(Variable) (#Variable)

// Sets default values
ASkyScrapperBlock::ASkyScrapperBlock()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;

	CurrentMesh = nullptr;

	CurrentLives = StartLives;


	RootSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootScene"));
	RootComponent = RootSceneComponent;
	RootComponent->SetIsReplicated(true);

	BlockMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BlockMesh"));
	BlockMesh->AttachToComponent(RootSceneComponent, FAttachmentTransformRules::SnapToTargetIncludingScale);
	BlockMesh->SetIsReplicated(true);
}


// Called when the game starts or when spawned
void ASkyScrapperBlock::BeginPlay()
{
	Super::BeginPlay();
	int const Size = ChosenDestructMeshes.Num();

	float const LivesDivision = StartLives / Size;

	for (int i = 1; i < Size; i++)
	{
		ChangeMeshThresholds.insert(std::pair<float, int>{LivesDivision * i, i - 1});

	}
}

void ASkyScrapperBlock::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//Do replications on properties
	DOREPLIFETIME(ASkyScrapperBlock, CurrentMesh);
	DOREPLIFETIME(ASkyScrapperBlock, BlockMesh);
	DOREPLIFETIME(ASkyScrapperBlock, RootSceneComponent);
}


void ASkyScrapperBlock::ChangeDestructMesh()
{
	int const Size = ChosenDestructMeshes.Num();

	if (Size == 0)
	{
		return;
	}

	if (CurrentLives <= 0)
	{
		BlockMesh->SetStaticMesh(ChosenDestructMeshes[Size - 1]);
		return;
	}

	for (auto Threshold = ChangeMeshThresholds.rbegin(); Threshold  != ChangeMeshThresholds.rend(); ++Threshold) {

		const FString DamageStr = FString::SanitizeFloat(Threshold->first);

		if (ThresholdBefore.first == 0.0f)
		{
			ThresholdBefore.first = Threshold->first;
			ThresholdBefore.second = Threshold->second;
			const FString BeforeStr = FString::SanitizeFloat(ThresholdBefore.first);
			const FString NowStr = FString::SanitizeFloat(Threshold->first);
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green,
							FString::Printf(
								TEXT("Threshold before %s, threshold now %s"), *BeforeStr,
								*NowStr));
			ThresholdBefore.first = Threshold->first;
			ThresholdBefore.second = Threshold->second;
			return;
		}
		
		{
			if (CurrentLives >= Threshold->first && CurrentLives <= ThresholdBefore.first)
			{
				BlockMesh->SetStaticMesh(ChosenDestructMeshes[Threshold->second]);
				const FString BeforeStr = FString::SanitizeFloat(ThresholdBefore.first);
				const FString NowStr = FString::SanitizeFloat(Threshold->first);
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green,
								FString::Printf(
									TEXT("Threshold before %s, threshold now %s"), *BeforeStr,
									*NowStr));
				ThresholdBefore.first = Threshold->first;
				ThresholdBefore.second = Threshold->second;
				return;
			}
			if(ThresholdBefore.second == 0 && CurrentLives <= ThresholdBefore.first)
			{
				BlockMesh->SetStaticMesh(ChosenDestructMeshes[Threshold->second]);
				const FString BeforeStr = FString::SanitizeFloat(ThresholdBefore.first);
				const FString NowStr = FString::SanitizeFloat(Threshold->first);
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green,
								FString::Printf(
									TEXT("Last One Threshold before %s, threshold now %s"), *BeforeStr,
									*NowStr));
				ThresholdBefore.first = Threshold->first;
				ThresholdBefore.second = Threshold->second;
				return;
			}
			/*
			if( Threshold->first == ThresholdBefore.first && CurrentLives <= ThresholdBefore.first && !(CurrentLives >= Threshold->first))
			{
				BlockMesh->SetStaticMesh(ChosenDestructMeshes[ThresholdBefore.second]);
				const FString BeforeStr = FString::SanitizeFloat(ThresholdBefore.first);
				const FString NowStr = FString::SanitizeFloat(Threshold->first);
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green,
								FString::Printf(
									TEXT("Last One Threshold before %s, threshold now %s"), *BeforeStr,
									*NowStr));
				return;
			}
			*/
			
		}
	}
}

void SetThresholdBefore()
{
	
}


// Called every frame
void ASkyScrapperBlock::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

EDestructMeshes ASkyScrapperBlock::GetDestructType(const FString ObjectName)
{
	if (ObjectName.Equals(FString("RampageBuildingBlock1"), ESearchCase::CaseSensitive))
	{
		return EDestructMeshes::RampageBuildingBlock1;
	}

	if (ObjectName.Equals(FString("RampageBuildingBlock2"), ESearchCase::CaseSensitive))
	{
		return EDestructMeshes::RampageBuildingBlock2;
	}

	if (ObjectName.Equals(FString("RampageBuildingBlock3"), ESearchCase::CaseSensitive))
	{
		return EDestructMeshes::RampageBuildingBlock3;
	}

	if (ObjectName.Equals(FString("RampageBuildingBlock4"), ESearchCase::CaseSensitive))
	{
		return EDestructMeshes::RampageBuildingBlock4;
	}

	if (ObjectName.Equals(FString("RampageBuildingBlockDoor"), ESearchCase::CaseSensitive))
	{
		return EDestructMeshes::RampageBuildingBlockDoor;
	}

	return EDestructMeshes::RampageBuildingBlock1;
}

void ASkyScrapperBlock::IsHit_Implementation(float Damage)
{
	if (CurrentLives <= 0) return;
	CurrentLives = CurrentLives - Damage;
	const FString DamageStr = FString::SanitizeFloat(Damage);
	const FString LivesStr = FString::SanitizeFloat(CurrentLives);
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green,
	                                 FString::Printf(
		                                 TEXT("Has Lives lost Damage %s, now has only %s lives"), *DamageStr,
		                                 *LivesStr));

	ChangeDestructMesh();
}
