// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCamera.h"

#include "Camera/CameraComponent.h"

#include "TriggerInterface.h"

AMainCamera::AMainCamera()
{
	PrimaryActorTick.bCanEverTick = true;
	GetCameraComponent()->GetCameraView(0.0f, CameraView);
}

void AMainCamera::CenterBetweenPlayers(const FVector& Player1, const FVector& Player2)
{
	FVector Transform = FVector(0.0f, GetActorLocation().Y, 0.0f);
	Transform.X = (Player1.X + Player2.X) / 2;
	Transform.Z = (Player1.Z + Player2.Z) / 2;
	SetActorLocation(Transform);

}

FVector AMainCamera::GetCameraTopEdge() const
{
	const float Width = CameraView.OrthoWidth;
	const float Height = Width / CameraView.AspectRatio;
	return FVector(CameraView.Location.X,CameraView.Location.Y,CameraView.Location.Z + Height /2.0f);
}

void AMainCamera::Tick(float DeltaSeconds)
{
		TraceForward();
}

void AMainCamera::CenterBetweenPlayersServer_Implementation(const FVector& Player1, const FVector& Player2)
{
	CenterBetweenPlayers(Player1, Player2);
}

void AMainCamera::TraceForward_Implementation()
{
	FRotator Rot = CameraView.Rotation;
	FHitResult Hit;

	FVector Start = GetCameraTopEdge();
	FVector End = Start + (Rot.Vector() * TraceDistance);

	FCollisionQueryParams TraceParams;
	bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, TraceParams );

	
	if(bHit)
	{
		AActor* Interactable = Hit.GetActor();

		if(Interactable)
		{
			if(Interactable != FocusedActor){
				ITriggerInterface* Interface = Cast<ITriggerInterface>(Interactable);
				if(Interface)
				{
					Interface->IsTriggered();
				}
				FocusedActor = Interactable;
			}
		}else
		{
			FocusedActor = nullptr;
		}
	}
}
