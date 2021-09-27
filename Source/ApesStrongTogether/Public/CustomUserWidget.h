// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CustomUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class APESSTRONGTOGETHER_API UCustomUserWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintPure, BlueprintCosmetic, Category="Viewport")
	static bool GetScreenPositionScaledByDPI(APlayerController* Player, float LocationX, float LocationY,
	                                         float& Scaled_X, float& Scaled_Y);
};
