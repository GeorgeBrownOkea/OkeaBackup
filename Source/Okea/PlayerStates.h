// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/UserDefinedEnum.h"
#include "PlayerStates.generated.h"

/**
 * 
 */
 UENUM(BlueprintType)
enum PlayerStates 
{
	FreeMovement UMETA(DisplayName = "FreeMovement"),
	DisabledMovement UMETA(DisplayName = "MovementDisabled"),
	Pushing UMETA(DisplayName = "PushingAndPulling"),
	Carrying UMETA(DisplayName = "Carrying"),
	Carting UMETA(DisplayName = "Carting"),

};
