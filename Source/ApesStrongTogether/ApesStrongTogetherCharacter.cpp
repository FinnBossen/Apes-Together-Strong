// Copyright Epic Games, Inc. All Rights Reserved.

#include "ApesStrongTogetherCharacter.h"
#include <mftransform.h>
#include <string>
#include <sstream>

#include "PaperFlipbookComponent.h"
#include "Components/CapsuleComponent.h"
#include "Net/UnrealNetwork.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/PlayerController.h"

DEFINE_LOG_CATEGORY_STATIC(SideScrollerCharacter, Log, All);

//////////////////////////////////////////////////////////////////////////
// AApesStrongTogetherCharacter

AApesStrongTogetherCharacter::AApesStrongTogetherCharacter()
{
	// Use only Yaw from the controller and ignore the rest of the rotation.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	// Set the size of our collision capsule.
	GetCapsuleComponent()->SetCapsuleHalfHeight(96.0f);
	GetCapsuleComponent()->SetCapsuleRadius(40.0f);
	UCharacterMovementComponent* Movement = GetCharacterMovement();
	Movement->SetMovementMode(MOVE_Flying);
	
	// Create a camera boom attached to the root (capsule)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 500.0f;
	CameraBoom->SocketOffset = FVector(0.0f, 0.0f, 75.0f);
	CameraBoom->SetUsingAbsoluteRotation(true);
	CameraBoom->bDoCollisionTest = false;
	CameraBoom->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
	

	// Create an orthographic camera (no perspective) and attach it to the boom
	SideViewCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("SideViewCamera"));
	SideViewCameraComponent->ProjectionMode = ECameraProjectionMode::Orthographic;
	SideViewCameraComponent->OrthoWidth = 2048.0f;
	SideViewCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);

	// Prevent all automatic rotation behavior on the camera, character, and camera component
	CameraBoom->SetUsingAbsoluteRotation(true);
	SideViewCameraComponent->bUsePawnControlRotation = false;
	SideViewCameraComponent->bAutoActivate = true;
	GetCharacterMovement()->bOrientRotationToMovement = false;

	// Configure character movement
	 GetCharacterMovement()->GravityScale = 0.0f;
	 GetCharacterMovement()->AirControl = 1.f;
	 GetCharacterMovement()->JumpZVelocity = 1000.f;
	 GetCharacterMovement()->GroundFriction = 3.0f;
	 GetCharacterMovement()->MaxWalkSpeed = 600.0f;
	GetCharacterMovement()->MaxFlySpeed = 600.0f;

	// Lock character motion onto the XZ plane, so the character can't move in or out of the screen
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->SetPlaneConstraintNormal(FVector(0.0f, -1.0f, 0.0f));

	// Behave like a traditional 2D platformer character, with a flat bottom instead of a curved capsule bottom
	// Note: This can cause a little floating when going up inclines; you can choose the tradeoff between better
	// behavior on the edge of a ledge versus inclines by setting this to true or false
	GetCharacterMovement()->bUseFlatBaseForFloorChecks = true;

    // 	TextComponent = CreateDefaultSubobject<UTextRenderComponent>(TEXT("IncarGear"));
    // 	TextComponent->SetRelativeScale3D(FVector(3.0f, 3.0f, 3.0f));
    // 	TextComponent->SetRelativeLocation(FVector(35.0f, 5.0f, 20.0f));
    // 	TextComponent->SetRelativeRotation(FRotator(0.0f, 90.0f, 0.0f));
    // 	TextComponent->SetupAttachment(RootComponent);

	// Enable replication on the Sprite component so animations show up when networked
	GetSprite()->SetIsReplicated(true);
	bReplicates = true;
	ACharacter::SetReplicateMovement(false);
	SetReplicates(true);


}

//////////////////////////////////////////////////////////////////////////
// Animation

void AApesStrongTogetherCharacter::UpdateAnimation()
{
	const FVector PlayerVelocity = GetVelocity();
	const float PlayerSpeedSqr = PlayerVelocity.SizeSquared();

	// Are we moving or standing still?
	UPaperFlipbook* DesiredAnimation = (PlayerSpeedSqr > 0.0f) ? RunningAnimation : IdleAnimation;
	if( GetSprite()->GetFlipbook() != DesiredAnimation 	)
	{
		GetSprite()->SetFlipbook(DesiredAnimation);
	}
}

void AApesStrongTogetherCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (GetLocalRole() != ROLE_AutonomousProxy) // then we are not originating any movement
		{
		return;
		}
	UpdateCharacter();	
}

void AApesStrongTogetherCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}


//////////////////////////////////////////////////////////////////////////
// Input

void AApesStrongTogetherCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{


	PlayerInputComponent->BindAxis("MoveHorizontal", this, &AApesStrongTogetherCharacter::MoveHorizontal);

	if (GetNetMode() == ENetMode::NM_Client)
	{
		PlayerInputComponent->BindAxis("MoveVertical", this, &AApesStrongTogetherCharacter::MoveVerticalServer);
	}
	PlayerInputComponent->BindAxis("MoveVertical", this, &AApesStrongTogetherCharacter::MoveVertical);
	
	PlayerInputComponent->BindTouch(IE_Pressed, this, &AApesStrongTogetherCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &AApesStrongTogetherCharacter::TouchStopped);
}

void AApesStrongTogetherCharacter::MoveHorizontal(float Value)
{

	// Apply the input to the character motion
	if(CanMoveHorizontal){
	AddMovementInput(FVector(1.0f, 0.0f, 0.0f), Value);
	}
	
}

void AApesStrongTogetherCharacter::MoveVertical_Implementation(float Value)
{

	
	if (Value == 0.f|| CanMoveHorizontal) return;
	
	FVector NewLocation = GetActorLocation();

	if (Value > 0.1f) {
		NewLocation.Z += Speed;
		/*
		std::ostringstream oss;
		oss << "Bka Bla want to fly high high in the sky" << NewLocation.Z;
		std::string CoolText = oss.str();
		FString HappyString(CoolText.c_str());
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, HappyString);
		*/
	} else if (Value < -0.1f) {
		NewLocation.Z += -Speed;
		/*
		std::ostringstream oss;
		oss << "Bka Bla want to fly down down in the sky" << NewLocation.Z;
		std::string CoolText= oss.str();
		FString HappyString(CoolText.c_str());
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, HappyString);
		*/
	}
	SetActorLocation(NewLocation);
/*
	// Apply the input to the character motion
	if(!CanMoveHorizontal){
		AddMovementInput(FVector(0.0f, 0.0f, 1.0f), Value, false);
	}
	*/
}

void AApesStrongTogetherCharacter::MoveVerticalServer_Implementation(float Value)
{
	MoveVertical_Implementation(Value);
}


void AApesStrongTogetherCharacter::TouchStarted(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	// Jump on any touch
	Jump();
}

void AApesStrongTogetherCharacter::TouchStopped(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	// Cease jumping once touch stopped
	StopJumping();
}

void AApesStrongTogetherCharacter::UpdateCharacter()
{
	// Update animation to match the motion
	UpdateAnimation();

	// Now setup the rotation of the controller based on the direction we are travelling
	const FVector PlayerVelocity = GetVelocity();	
	float const TravelDirectionX = PlayerVelocity.X;
	float const TravelDirectionY = PlayerVelocity.Y;
	// Set the rotation so that the character faces his direction of travel.
	if (Controller != nullptr)
	{
		if (TravelDirectionX < 0.0f)
		{
			Controller->SetControlRotation(FRotator(0.0, 180.0f, 0.0f));
		}
		else if (TravelDirectionX > 0.0f)
		{
			Controller->SetControlRotation(FRotator(0.0f, 0.0f, 0.0f));
		}
		
	}
}
