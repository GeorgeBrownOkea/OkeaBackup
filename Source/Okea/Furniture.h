// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "FurnaturePriceRange.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Furniture.generated.h"

UCLASS()
class OKEA_API AFurniture : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFurniture();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	


	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = Get)
	TEnumAsByte<EPriceRange> GetPriceRange();

	UFUNCTION(BlueprintCallable, Category = Get)
	TEnumAsByte<ERoomType> GetRoomType();

	UFUNCTION(BlueprintCallable, Category = Get)
	UStaticMeshComponent* GetBaseMesh();


	float GetWeigth();

	bool GetIsCarried();

private:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PriceState, meta = (AllowPrivateAccess = "true"))
	TEnumAsByte<EPriceRange> priceRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PriceState, meta = (AllowPrivateAccess = "true"))
	TEnumAsByte<ERoomType> roomType;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* baseMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement, meta = (AllowPrivateAccess = "true"))
		float weight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement, meta = (AllowPrivateAccess = "true"))
		bool isCarried;

};
