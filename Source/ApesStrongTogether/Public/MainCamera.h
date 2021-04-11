// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include <vector>

#include "Camera/CameraActor.h"
#include "MainCamera.generated.h"

/**
 * 
 */
UCLASS()
class APESSTRONGTOGETHER_API AMainCamera final : public ACameraActor
{
	GENERATED_BODY()
	public:
	UFUNCTION(BlueprintCallable, Category = "Camera")
	void CenterBetweenPlayers(const  FVector& Player1, const  FVector& Player2);

	UFUNCTION(BlueprintCallable, Category = "Camera",Server, Reliable)
    void CenterBetweenPlayersServer(const FVector& Player1, const FVector& Player2);

	
};
