// Fill out your copyright notice in the Description page of Project Settings.

#include "../Public/Projectile.h"
#include "../Public/TankBarrel.h"
#include "../Public/TankTurret.h"
#include "../Public/TankAimingComponent.h"
#include "../Public/TankMovementComponent.h"
#include "../Public/Tank.h"

// Sets default values
ATank::ATank()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void ATank::BeginPlay()
{
	Super::BeginPlay();
	
	SetBarrelReference();
}

// Called to bind functionality to input
void ATank::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ATank::Fire() {
	bool isReloaded = (FPlatformTime::Seconds() - LastFireTime) > ReloadTimeInSeconds;
	if(!(Barrel && isReloaded)) { return; }

	AProjectile *Projectile = GetWorld()->SpawnActor<AProjectile>(
		ProjectileBlueprint,
		Barrel->GetSocketLocation(FName("Projectile")),
		Barrel->GetSocketRotation(FName("Projectile"))
	);
	Projectile->LaunchProjectile(LaunchSpeed);
	LastFireTime = FPlatformTime::Seconds();
}

void ATank::AimAt(FVector HitLocation) {
	UTankAimingComponent *TankAimingComponent = GetTankAimingComponent();
	if(!TankAimingComponent) { return; }
	TankAimingComponent->AimAt(HitLocation, LaunchSpeed);
}

void ATank::SetBarrelReference() {
	UTankAimingComponent *TankAimingComponent = GetTankAimingComponent();
	if(!TankAimingComponent) { return; }
	Barrel = TankAimingComponent->GetBarrel();
}

UTankAimingComponent *ATank::GetTankAimingComponent() const {
	UTankAimingComponent *TankAimingComponent = FindComponentByClass<UTankAimingComponent>();
	if(!TankAimingComponent) {
		UE_LOG(LogTemp, Error, TEXT("TankAimingComponent is missing in %s"), *GetName());
		return nullptr;
	}

	return TankAimingComponent;
}