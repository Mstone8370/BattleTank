// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "TankTurret.generated.h"

/**
 * 
 */
UCLASS( meta=(BlueprintSpawnableComponent), hidecategories=("Collision") )
class BATTLETANK_API UTankTurret : public UStaticMeshComponent
{
	GENERATED_BODY()
	
public:
	// -1.f <= RelativeSpeed <= 1.f
	void Rotate(float RelativeSpeed);

private:
	UPROPERTY(EditAnywhere, Category=Setup)
	float MaxDegreesPerSecond = 30.f;

};
