// Copyright Epic Games, Inc. All Rights Reserved.

#include "ApesStrongTogetherGameMode.h"
#include "ApesStrongTogetherCharacter.h"

AApesStrongTogetherGameMode::AApesStrongTogetherGameMode()
{
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/2DSideScrollerCPP/Blueprints/BP_2DSideScrollerCharacter"));
   	DefaultPawnClass = PlayerPawnClassFinder.Class;
	
}
