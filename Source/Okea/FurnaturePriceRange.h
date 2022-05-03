// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/UserDefinedEnum.h"
#include "FurnaturePriceRange.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum EPriceRange
{
	SMALL UMETA(DisplayName = "Small"),
	MEDIUM UMETA(DisplayName = "Medium"),
	BIG UMETA(DisplayName = "Big")
};

UENUM(BlueprintType)
enum ERoomType
{
	BEDROOM UMETA(DisplayName = "Bedroom"),
	RESTAURANT UMETA(DisplayName = "Restaurant"),
	LIVINGROOM UMETA(DisplayName = "Living Room")
};
