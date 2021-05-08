// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCamera.h"

#include "Camera/CameraComponent.h"

#include <string> 


#include "SkyScrapperBuilding.h"
#include "TriggerInterface.h"

AMainCamera::AMainCamera()
{
	PrimaryActorTick.bCanEverTick = true;
}

float AMainCamera::CenterBetweenPlayers(const FVector& Player1, const FVector& Player2)
{
	FVector Transform = FVector(0.0f, GetActorLocation().Y, 0.0f);
	Transform.X = (Player1.X + Player2.X) / 2;
	Transform.Z = (Player1.Z + Player2.Z) / 2;
	SetActorLocation(Transform);
	return Distance(Player1.X,Player1.Z,Player2.X,Player2.Z);

}

void AMainCamera::TraceForward()
{
	FMinimalViewInfo CameraView;
	GetCameraComponent()->GetCameraView(0.0f, CameraView);
	FRotator Rot = CameraView.Rotation;
	FHitResult Hit;

	FVector Start = GetCameraTopEdge(CameraView) + FVector(0,0,TraceOffset);
	FVector End = Start + (Rot.Vector() * TraceDistance);
	std::string X1 = std::to_string(Start.X);
	std::string Y1 = std::to_string(Start.Y);
	std::string Z1 = std::to_string(Start.Z);
	std::string StartString ="X:"+ X1 +"Y:"+ Y1 +"Z:"+ Z1;
	FString FStartString(StartString .c_str());
	std::string X2 = std::to_string(End.X);
	std::string Y2 = std::to_string(End.Y);
	std::string Z2 = std::to_string(End.Z);
	std::string EndString ="X:"+ X2 +"Y:"+ Y2 +"Z:"+ Z2;
	FString FEndString(EndString .c_str());

	std::string X3 = std::to_string(Rot.Vector().X);
	std::string Y3 = std::to_string(Rot.Vector().Y);
	std::string Z3 = std::to_string(Rot.Vector().Z);
	std::string RotString ="X:"+ X3 +"Y:"+ Y3 +"Z:"+ Z3;
	FString FRotString(RotString .c_str());

	FCollisionQueryParams TraceParams;
	bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, TraceParams );
//	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, FString::Printf(TEXT("%s"), *FStartString));
//	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, FString::Printf(TEXT("%s"), *FEndString));
//	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("%s"), *FRotString));

	if(bHit)
	{
		AActor* Interactable = Hit.GetActor();
		if(Interactable)
		{
			if(Interactable != FocusedActor){
			
				ITriggerInterface* Interface = Cast<ITriggerInterface>(Interactable);
				if(Interface)
				{
					//Interface->Execute_IsTriggered(Interactable);
					ASkyScrapperBuilding* Building = Cast<ASkyScrapperBuilding>(Interactable->GetParentActor());
					Building->TriggerNewFloor_Implementation();
					
				}
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, FString::Printf(TEXT("Focussed: %s"), *Interactable->GetName()));
				FocusedActor = Interactable;
			}
		}else
		{
			FocusedActor = nullptr;
		}
	}
}

FVector AMainCamera::GetCameraTopEdge( FMinimalViewInfo const CameraView) const
{
	const float Width = CameraView.OrthoWidth;
	const float Height = Width / CameraView.AspectRatio;
	return FVector(CameraView.Location.X,CameraView.Location.Y,CameraView.Location.Z + Height /2.0f);
}

int AMainCamera::GetPercentageDifferenceBetweenTwoFloats(float A, float B)
{
	return ((B - A) * 100) / A;;
}

void AMainCamera::Tick(float const DeltaSeconds)
{
		Super::Tick(DeltaSeconds);
		TraceForward();
}

float AMainCamera::Distance(const int X1, const int Z1, const int X2, const int Z2)
{
	// Calculating distance
	return sqrt(pow(X2 - X1, 2) +
                pow(Z2 - Z1, 2) * 1.0);
}

void AMainCamera::CenterBetweenPlayersServer_Implementation(const FVector& Player1, const FVector& Player2)
{
	CenterBetweenPlayers(Player1, Player2);
}

