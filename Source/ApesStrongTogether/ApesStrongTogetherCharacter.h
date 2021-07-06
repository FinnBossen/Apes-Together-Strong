// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "MainCamera.h"
#include "PaperCharacter.h"
#include "ApesStrongTogetherCharacter.generated.h"

class UTextRenderComponent;

/**
 * This class is the default character for ApesStrongTogether, and it is responsible for all
 * physical interaction between the player and the world.
 *
 * The capsule component (inherited from ACharacter) handles collision with the world
 * The CharacterMovementComponent (inherited from ACharacter) handles movement of the collision capsule
 * The Sprite component (inherited from APaperCharacter) handles the visuals
 */

UENUM(BlueprintType)
enum class EAnimationCycles : uint8
{
	Idle UMETA(DisplayName = "Idle"),
	Climbing UMETA(DisplayName = "Climbing"),
};

UENUM(BlueprintType)
enum class EOneTimeAnimation : uint8
{
	Beating UMETA(DisplayName = "Beating"),
	Kicking UMETA(DisplayName = "Kicking"),
	Jumping UMETA(DisplayName = "Jumping"),
};

UCLASS(config=Game)
class AApesStrongTogetherCharacter : public APaperCharacter
{
	GENERATED_BODY()

	/** Side view camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera, meta=(AllowPrivateAccess="true"))
	class UCameraComponent* SideViewCameraComponent;

	/** Camera boom positioning the camera beside the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	UTextRenderComponent* TextComponent;
	virtual void Tick(float DeltaSeconds) override;

	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;


protected:
	// The animation to play while running around
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Animations)
	class UPaperFlipbook* RunningAnimation;

	// The animation to play while idle (standing still)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
	class UPaperFlipbook* IdleAnimation;

	/** Called to choose the correct animation to play based on the character's movement state */
	void UpdateAnimation();

	/** Called for side to side input */
	void MoveHorizontal(float Value);

	/** Called for Vertical input */
	UFUNCTION(NetMulticast, Reliable)
	void MoveVertical(float Value);

	/** Called for Vertical input */
	UFUNCTION(Server, Reliable)
	void MoveVerticalServer(float Value);

	/** Called for Vertical input */
	UFUNCTION(NetMulticast, Reliable)
	void CanWalkDirection(bool Up, bool Down);

	/* Handle to manage the voxel Animation timer */
	FTimerHandle VoxelAnimationTimerHandle;

	UFUNCTION()
	void VoxelAnimation();

	UPROPERTY()
	EAnimationCycles CurrentAnimationCycle;

	/** Called for Vertical input */
	UFUNCTION(Server, Reliable)
	void CanWalkDirectionServer(bool Up, bool Down);

	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* HitComp, class AActor* OtherActor,
	                    class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	                    const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
	                  class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void UpdateCharacter();

	/** Handle touch inputs. */
	void TouchStarted(const ETouchIndex::Type FingerIndex, const FVector Location);

	/** Handle touch stop event. */
	void TouchStopped(const ETouchIndex::Type FingerIndex, const FVector Location);

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	// End of APawn interface
	bool CanMoveHorizontal = false;

	bool CanWalkUp = true;

	bool CanWalkDown = true;

	int CurrentVoxelFrame = 0;

	UCapsuleComponent* TriggerCapsule;
	
	UPROPERTY(Instanced, VisibleAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess = "true"))
    UStaticMeshComponent* ApeMesh;
public:
	AApesStrongTogetherCharacter();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Custom")
	float Speed = 20.f;
	/** Returns SideViewCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetSideViewCameraComponent() const { return SideViewCameraComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, AssetRegistrySearchable, meta=(DisplayName="WalkCycle"),Category="VoxelAnimation")
	TArray<UStaticMesh*> WalkCycle;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, AssetRegistrySearchable, meta=(DisplayName="Idle"),Category="VoxelAnimation")
	TArray<UStaticMesh*> Idle;

	UPROPERTY( EditAnywhere)
	float  VoxelAnimationSpeed;


	UFUNCTION(BlueprintCallable, Category="VoxelAnimation")
	void SetVoxelMaterial(UStaticMesh* UStaticMesh, FName MaterialSlotName);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="VoxelAnimation")
	void ChangeCurrentAnimCycle(EAnimationCycles EAnimationCyclesEnum);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="VoxelAnimation")
	void TriggerOneTimeAnim(EOneTimeAnimation EOneTimeAnimationEnum);
};
