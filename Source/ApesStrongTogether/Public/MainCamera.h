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
	// Sets default values for main camera properties
	AMainCamera();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Interaction")
	float TraceOffset;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Interaction")
	float TraceDistance;
	
	UFUNCTION(BlueprintCallable, Category = "Camera")
	float CenterBetweenPlayers(const  FVector& Player1, const  FVector& Player2);

	UFUNCTION(BlueprintCallable, Category = "Camera",Server, Reliable)
    void CenterBetweenPlayersServer(const FVector& Player1, const FVector& Player2);

	UFUNCTION(BlueprintCallable, Category="Calculation")
	int GetPercentageDifferenceBetweenTwoFloats(float A, float B);

	UFUNCTION(BlueprintCallable, Category = "Camera")
	FVector GetCameraTopEdge( FMinimalViewInfo const CameraView) const;
	
	UFUNCTION(BlueprintCallable, Category = "Camera")
	FVector GetCameraDownEdge( FMinimalViewInfo const CameraView) const;

	virtual void Tick(float DeltaSeconds) override;

	private:

	static float Distance(const int X1, const int Z1, const int X2, const int Z2);
	void TraceForward();
	
	AActor* FocusedActor;
};
