// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomUserWidget.h"

#include "Blueprint/WidgetLayoutLibrary.h"

bool UCustomUserWidget::GetScreenPositionScaledByDPI(APlayerController* Player,float const LocationX, float const LocationY, float& Scaled_X, float& Scaled_Y)
{
	if ( Player )
	{
		float const Scale = UWidgetLayoutLibrary::GetViewportScale(Player);
		Scaled_X= LocationX / Scale;
		Scaled_Y = LocationY / Scale;

		return true;
	}

	return false;
}
