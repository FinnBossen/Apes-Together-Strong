// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "SkyScrapperFloor.h"
#include "GameFramework/Actor.h"
#include "SkyScrapperBuilding.generated.h"

UCLASS()
class APESSTRONGTOGETHER_API ASkyScrapperBuilding final : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASkyScrapperBuilding();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;



public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	UFUNCTION(BlueprintCallable, Category="Building")
	void CreateNewFloor();

	UFUNCTION(BlueprintCallable, Category="Trigger")
	void CreateInitialFloors();

	UPROPERTY(EditAnywhere, Category= "Building")
	int8 StartFloors;

	UPROPERTY(EditAnywhere, Category= "Building")
	float FloorOffset;
	
	UPROPERTY(BlueprintReadOnly,VisibleAnywhere ,Category= "Building", Replicated)
	FVector LatestFloorBlueprintPosition;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category= "Building")
	TSubclassOf<ASkyScrapperFloor> SkyScrapperFloor;

	UFUNCTION(BlueprintCallable, Category="Trigger")
	void TriggerNewFloor();


	UFUNCTION(BlueprintNativeEvent,BlueprintCallable, Category="Trigger")
	void ChangeTriggerPosition();

};
