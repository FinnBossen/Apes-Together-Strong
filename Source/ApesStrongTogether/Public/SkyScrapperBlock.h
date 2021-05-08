// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SkyScrapperBlock.generated.h"

UCLASS()
class APESSTRONGTOGETHER_API ASkyScrapperBlock : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASkyScrapperBlock();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(Category="Custom", BlueprintReadWrite, EditAnywhere, AssetRegistrySearchable, meta=(DisplayName="Meshes"))
	TArray<UStaticMesh*> Meshes;

	
	UPROPERTY(Category="Custom", BlueprintReadWrite, EditAnywhere, AssetRegistrySearchable, meta=(DisplayName="Materials"))
	TArray<UMaterial*> Materials;

};
