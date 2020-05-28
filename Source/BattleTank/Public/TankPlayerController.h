// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "Math/Vector.h"
#include "Math/Vector2D.h"
#include "TankPlayerController.generated.h"

/**
 * 
 */

class ATank;
class UTankAimingComponent;

UCLASS()
class BATTLETANK_API ATankPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

protected:
	UFUNCTION(BlueprintImplementableEvent, Category="Setup")
	void FoundAimingComponent(UTankAimingComponent* AimCompRef);

private:
	void AimTowardCrosshair();
	bool GetSightRayHitLocation(FVector &OutHitLocation) const;
	bool GetLookDirection(FVector2D ScreenLocation, FVector& OutWorldDirection) const;
	bool GetLookVectorHitLocation(FVector LookDirection, FVector& OutHitLocation) const;

	UFUNCTION()
	void OnPlayerTankDeath();

	UPROPERTY(EditDefaultsOnly)
	float CrossHairXLocation = 0.5;

	UPROPERTY(EditDefaultsOnly)
	float CrossHairYLocation = 0.5;

	UPROPERTY(EditDefaultsOnly)
	float LineTraceRange = 1000000.f;

	UTankAimingComponent* AimingComponent = nullptr;
};
