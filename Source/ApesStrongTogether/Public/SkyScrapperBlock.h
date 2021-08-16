// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Chaos/AABB.h"
#include "GameFramework/Actor.h"
#include "SkyScrapperBlock.generated.h"

UENUM(BlueprintType)
enum class EDestructMeshes : uint8
{
	RampageBuildingBlock1 UMETA(DisplayName = "RampageBuildingBlock1"),
	RampageBuildingBlock2 UMETA(DisplayName = "RampageBuildingBlock2"),
	RampageBuildingBlock3 UMETA(DisplayName = "RampageBuildingBlock3"),
	RampageBuildingBlock4 UMETA(DisplayName = "RampageBuildingBlock4"),
	RampageBuildingBlockDoor UMETA(DisplayName = "RampageBuildingBlockDoor"),
};

UCLASS()
class APESSTRONGTOGETHER_API ASkyScrapperBlock : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASkyScrapperBlock();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Health")
	float Lives;
	
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category= "Mesh", Replicated)
	UStaticMesh* CurrentMesh;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	virtual void GetLifetimeReplicatedProps
(
	TArray< FLifetimeProperty > & OutLifetimeProps
) const override;

	UFUNCTION(Category="Custom", BlueprintCallable)
	void ChangeDestructMesh();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(Category="Custom", BlueprintReadWrite, EditAnywhere, AssetRegistrySearchable, meta=(DisplayName="Meshes"))
	TArray<UStaticMesh*> Meshes;
	
	UPROPERTY(Category="Custom", BlueprintReadWrite, EditAnywhere, AssetRegistrySearchable, meta=(DisplayName="Materials"))
	TArray<UMaterial*> Materials;

	UPROPERTY(Category="Custom", BlueprintReadWrite, EditAnywhere, AssetRegistrySearchable)
	TArray<UStaticMesh*> ChosenDestructMeshes;
	
	UFUNCTION(Category="Custom", BlueprintCallable)
	static EDestructMeshes GetDestructType(FString ObjectName);

	


	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Damage")
	void IsHit(float Damage);

};


