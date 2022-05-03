// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FurnaturePriceRange.h"
#include "DropOffArea.generated.h"

UCLASS()
class OKEA_API ADropOffArea : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADropOffArea();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = ProcessPrice)
	int ProcessPrice(TEnumAsByte<ERoomType> roomType, TEnumAsByte<EPriceRange> PriceRange);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;



};
