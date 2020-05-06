// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/StaticMeshComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "TankAimingComponent.generated.h"

UENUM()
enum class EFiringStatus : uint8 {
	Locked,
	Aiming,
	Reloading
};

class UTankBarrel;
class UTankTurret;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BATTLETANK_API UTankAimingComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTankAimingComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadOnly, Category="State")
	EFiringStatus FiringStatus = EFiringStatus::Aiming;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	UFUNCTION(BlueprintCallable, Category="Setup")
	void Initialize(UTankBarrel *BarrelToSet, UTankTurret *TurretToSet);
	void AimAt(FVector HitLocation);
	UTankBarrel *GetBarrel() const;

private:
	UPROPERTY(EditDefaultsOnly, Category="Firing")
	float LaunchSpeed = 4000.f;

	UTankBarrel *Barrel = nullptr;
	UTankTurret *Turret = nullptr;

	void MoveBarrel(FVector AimDirection);
	void MoveTurret(FVector AimDirection);
	FRotator GetDeltaRotation(FVector AimDirection, FRotator Rotation);
};
