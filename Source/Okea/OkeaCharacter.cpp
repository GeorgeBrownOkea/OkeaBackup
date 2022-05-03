// Copyright Epic Games, Inc. All Rights Reserved.

#include "OkeaCharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "DrawDebugHelpers.h"
#include "Furniture.h"


//////////////////////////////////////////////////////////////////////////
// AOkeaCharacter

AOkeaCharacter::AOkeaCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 640.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	// Create a camera boom...
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true); // Don't want arm to rotate when character does
	CameraBoom->TargetArmLength = 800.f;
	CameraBoom->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));
	CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

	// Create a camera...
	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	//creating physics constraint
	myPhysicsConstraint = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("ItemGrabingPhysicsContraint"));
	myPhysicsConstraint->SetupAttachment(RootComponent);

	//starting values
	state = FreeMovement;
	originalRotationRate = GetCharacterMovement()->RotationRate.Yaw;
	originalMaxSpeed = GetCharacterMovement()->MaxWalkSpeed;

	if(MaxRotationLoss > originalRotationRate)
		MaxRotationLoss = originalRotationRate ;

	if(MaxSpeedLoss > originalMaxSpeed)
		MaxSpeedLoss = originalMaxSpeed;

	
	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
}


//////////////////////////////////////////////////////////////////////////
// Input

void AOkeaCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward", this, &AOkeaCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AOkeaCharacter::MoveRight);

}



void AOkeaCharacter::Tick(float deltaTime)
{
	ACharacter::Tick(deltaTime);
	if(!inputDirection.IsNearlyZero())
	{
		if (state == Pushing || state == Carting)
			PushObject(inputDirection.GetUnsafeNormal() * pushForce);
		else
			AddMovementInput(inputDirection.GetUnsafeNormal() );
	}
	inputDirection = {0,0,0};
}

void AOkeaCharacter::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f) && state != DisabledMovement)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		inputDirection.X = (FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X) * Value).X;
	}
}

void AOkeaCharacter::MoveRight(float Value)
{
	if ( (Controller != nullptr) && (Value != 0.0f)  && state != DisabledMovement)
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		inputDirection.Y = (FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y) * Value).Y;

	}
}


void AOkeaCharacter::AlterMoveSpeedFromWeight()
{
	//change character speed/rotation based on weight
	MassScaler = heldObjectWeight/maxPushWeight;
	GetCharacterMovement()->RotationRate.Yaw -= MaxRotationLoss * MassScaler;
	GetCharacterMovement()->MaxWalkSpeed -= MaxSpeedLoss * MassScaler;
}

//grabs objects passed in by blueprint
//small objects are picked up, heavy objects are pushed
void AOkeaCharacter::GrabObject(UStaticMeshComponent* object, bool isCart)
{
	if(HeldObject != nullptr || GetCharacterMovement()->IsFalling() )
		return; //skip if already holding an object
	
	HeldObject = object;

	if(isCart)
	{
		ChangeState(Carting);
	}
	else
	{
		heldObjectActualMass = HeldObject->GetMass();
	
		AFurniture* furniture = Cast<AFurniture>(object->GetOwner());
		if(furniture!= nullptr)
		{
			heldObjectWeight = furniture->GetWeigth();

			//check if large object(pushable) or small (carried) ==> change state
			if(furniture->GetIsCarried())
			{
				ChangeState(Carrying);
				FVector socketLocation = GetMesh()->GetSocketLocation("HoldItemSocket"); 
				HeldObject->SetWorldLocation(socketLocation);
			}
			else
			{
				ChangeState(Pushing);
			}
		}
	}
	//attaches furniture to the player
	myPhysicsConstraint->SetConstrainedComponents(object, "", GetCapsuleComponent(), "HoldItemSocket");

}

//removes physics constraint from held object, dropping them
//set via the blueprint right now
void AOkeaCharacter::ReleaseObject()
{
	if(HeldObject == nullptr)
		return; //no item to drop

	HeldObject->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);

	HeldObject->SetCollisionResponseToChannel(ECC_GameTraceChannel5, ECR_Ignore);

	ChangeState(FreeMovement);
	myPhysicsConstraint->BreakConstraint();
}


//Pushing movement mode: Pushes the furniture at the location of the player
void AOkeaCharacter::PushObject(FVector impulse)
{
		impulse -= (impulse * 0.6f * MassScaler);

		HeldObject->AddImpulseAtLocation(impulse, GetActorLocation()); 
		HeldObject->SetPhysicsLinearVelocity(HeldObject->GetPhysicsLinearVelocity().GetClampedToMaxSize(MaxPushingVelocity));
}


void AOkeaCharacter::ChangeState(TEnumAsByte<PlayerStates> newState)
{
	state = newState;
	myPhysicsConstraint->SetLinearPositionDrive(true, true, true);
	myPhysicsConstraint->SetLinearXLimit(LCM_Limited, 50);
	myPhysicsConstraint->SetLinearYLimit(LCM_Limited, 50);
	myPhysicsConstraint->SetLinearZLimit(LCM_Limited, 50);
	myPhysicsConstraint->SetLinearVelocityDrive(true, true, true);
	switch(state)
	{
		case Carting:
			//allow physics and reduce limits (pushObject() and the physics constraint do most of the work for us)
			GetCapsuleComponent()->SetSimulatePhysics(true);
			myPhysicsConstraint->SetAngularTwistLimit(ACM_Limited, 45); 
			myPhysicsConstraint->SetLinearPositionDrive(false, false, false);
			//myPhysicsConstraint->SetLinearVelocityDrive(false, false, false);
			myPhysicsConstraint->SetLinearXLimit(LCM_Locked, 0);
			myPhysicsConstraint->SetLinearYLimit(LCM_Locked, 0);
			myPhysicsConstraint->SetLinearZLimit(LCM_Locked, 0);
			break;

		case Pushing:
			//allow physics and reduce limits (pushObject() and the physics constraint do most of the work for us)
			GetCapsuleComponent()->SetSimulatePhysics(true);
			
			AlterMoveSpeedFromWeight();

			myPhysicsConstraint->SetAngularSwing2Limit(ACM_Limited, 30); //pitch slightly limited
			myPhysicsConstraint->SetAngularTwistLimit(ACM_Limited, 30);      //roll unconstrained
			break;

		case Carrying:
		//prevent collision w player and lower mass so it doesnt send everything flying
			HeldObject->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
			
			HeldObject->SetMassOverrideInKg(NAME_None, HeldObjectTempMass);

			myPhysicsConstraint->SetLinearXLimit(LCM_Locked, 0);
			myPhysicsConstraint->SetLinearYLimit(LCM_Locked, 0);
			myPhysicsConstraint->SetLinearZLimit(LCM_Locked, 0);

			AlterMoveSpeedFromWeight();

			myPhysicsConstraint->SetAngularSwing2Limit(ACM_Locked, 0);    //pitch matched to player
			myPhysicsConstraint->SetAngularTwistLimit(ACM_Locked, 0);     //roll matched to player

			break;
		
		case FreeMovement:
		//returns all changes back to normal
			if(HeldObject != nullptr)
			{
				HeldObject->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
				HeldObject->SetMassOverrideInKg(NAME_None, heldObjectActualMass);
				HeldObject = nullptr;
			}
			GetCapsuleComponent()->SetSimulatePhysics(false);
		
			GetCharacterMovement()->RotationRate.Yaw = originalRotationRate;
			GetCharacterMovement()->MaxWalkSpeed = originalMaxSpeed;

			break;
	}
}