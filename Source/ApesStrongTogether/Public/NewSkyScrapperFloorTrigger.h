// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TriggerInterface.h"
#include "NewSkyScrapperFloorTrigger.generated.h"

UCLASS()
class APESSTRONGTOGETHER_API ANewSkyScrapperFloorTrigger final : public AActor, public ITriggerInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ANewSkyScrapperFloorTrigger();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Trigger")
    void IsTriggered();
	virtual void IsTriggered_Implementation();

};
