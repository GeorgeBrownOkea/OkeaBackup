// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "PlayerStates.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"
#include "Engine/StaticMesh.h"
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "OkeaCharacter.generated.h"

UCLASS(config=Game)
class AOkeaCharacter : public ACharacter
{
	GENERATED_BODY()

private:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Interactions, meta = (AllowPrivateAccess = "true"))
	TEnumAsByte<PlayerStates> state;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Interactions, meta = (AllowPrivateAccess = "true"))
	float pushForce = 3200;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Interactions, meta = (AllowPrivateAccess = "true"))
	//how heavy a piece of furniture can be
	float maxPushWeight = 100;

	FVector inputDirection;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Interactions, meta = (AllowPrivateAccess = "true"))
	//How much a carried object's weight can impact rotation (doesn't affect pushed objects)
	float MaxRotationLoss;

	UPROPERTY(EditAnywhere, BlueprintReadWrite,  Category = Interactions, meta = (AllowPrivateAccess = "true"))
	//How much a carried object's weight can impact walkSpeed (doesn't affect pushed objects)
	float MaxSpeedLoss;

	 float originalRotationRate;
	 float originalMaxSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Interactions, meta = (AllowPrivateAccess = "true"))
	float MaxPushingVelocity = 600;

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* TopDownCameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* HeldObject;

	void PushObject(FVector impulse);

	float heldObjectActualMass;

	float heldObjectWeight;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Interactions, meta = (AllowPrivateAccess = "true"))
	float HeldObjectTempMass = 0.1;
	float MassScaler;

public:
	AOkeaCharacter();

	 UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Interactions, meta = (AllowPrivateAccess = "true"))
	 UPhysicsConstraintComponent* myPhysicsConstraint;
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetCameraComponent() const { return TopDownCameraComponent; }

	UFUNCTION(BlueprintCallable, Category = Interactions)
	void GrabObject(UStaticMeshComponent* object, bool isCart);

	UFUNCTION(BlueprintCallable, Category = Interactions)
	void ReleaseObject();

	UFUNCTION(BlueprintCallable, Category = Interactions)
	void  ChangeState(TEnumAsByte<PlayerStates> newState);
protected:

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	void AlterMoveSpeedFromWeight();

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

		UFUNCTION(BlueprintCallable, Category = Default)
		virtual void Tick(float deltaTime);
};
