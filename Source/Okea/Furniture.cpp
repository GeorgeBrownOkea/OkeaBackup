// Fill out your copyright notice in the Description page of Project Settings.


#include "Furniture.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AFurniture::AFurniture()
{
	baseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base Mesh"));
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AFurniture::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFurniture::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

TEnumAsByte<EPriceRange> AFurniture::GetPriceRange()
{
	return priceRange;
}

TEnumAsByte<ERoomType> AFurniture::GetRoomType()
{
	return roomType;
}

UStaticMeshComponent* AFurniture::GetBaseMesh()
{
	return baseMesh;
}

float AFurniture::GetWeigth()
{
	return weight;
}

bool AFurniture::GetIsCarried()
{
	return isCarried;
}

