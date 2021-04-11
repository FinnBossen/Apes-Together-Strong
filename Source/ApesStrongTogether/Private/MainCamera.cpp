// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCamera.h"

void AMainCamera::CenterBetweenPlayers(const FVector& Player1, const FVector& Player2)
{
	FVector Transform = FVector(0.0f, GetActorLocation().Y, 0.0f);
	Transform.X = (Player1.X + Player2.X) / 2;
	Transform.Z = (Player1.Z + Player2.Z) / 2;
	SetActorLocation(Transform);

}

void AMainCamera::CenterBetweenPlayersServer_Implementation(const FVector& Player1, const FVector& Player2)
{
	CenterBetweenPlayers(Player1, Player2);
}
