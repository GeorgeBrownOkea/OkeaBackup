// Fill out your copyright notice in the Description page of Project Settings.


#include "DropOffArea.h"

// Sets default values
ADropOffArea::ADropOffArea()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ADropOffArea::BeginPlay()
{
	Super::BeginPlay();
	
}

int ADropOffArea::ProcessPrice(TEnumAsByte<ERoomType> roomType, TEnumAsByte<EPriceRange> PriceRange)
{
	
	int price;

	switch (roomType)
	{
	case BEDROOM:
		switch (PriceRange)
		{
		case BIG:
			price = FMath::RandRange(500, 1000);
			break;
		case SMALL:
			price = FMath::RandRange(10, 100);
			break;
		case MEDIUM:
			price = FMath::RandRange(100, 500);
			break;
		default:
			break;
		}
		break;
	case RESTAURANT:
		switch (PriceRange)
		{
		case BIG:
			price = FMath::RandRange(500, 1000);
			break;
		case SMALL:
			price = FMath::RandRange(10, 100);
			break;
		case MEDIUM:
			price = FMath::RandRange(100, 500);
			break;
		default:
			break;
		}
		break;
		break;
	case LIVINGROOM:
		switch (PriceRange)
		{
		case BIG:
			price = FMath::RandRange(500, 1000);
			break;
		case SMALL:
			price = FMath::RandRange(10, 100);
			break;
		case MEDIUM:
			price = FMath::RandRange(100, 500);
			break;
		default:
			break;
		}
		break;
		break; 
	default:  
		break;
	}

	return price;
}

// Called every frame
void ADropOffArea::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

