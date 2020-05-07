// Fill out your copyright notice in the Description page of Project Settings.

#include "../Public/Projectile.h"
#include "../Public/TankBarrel.h"
#include "../Public/TankTurret.h"
#include "../Public/TankAimingComponent.h"

// Sets default values for this component's properties
UTankAimingComponent::UTankAimingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called when the game starts
void UTankAimingComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	LastFireTime = FPlatformTime::Seconds();
}

// Called every frame
void UTankAimingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	if((FPlatformTime::Seconds() - LastFireTime) > ReloadTimeInSeconds) {
		FiringStatus = EFiringStatus::Aiming;
	}
}

void UTankAimingComponent::Initialize(UTankBarrel *BarrelToSet, UTankTurret *TurretToSet) {
	if(!ensure(BarrelToSet && TurretToSet)) { return; }

	Barrel = BarrelToSet;
	Turret = TurretToSet;
}

void UTankAimingComponent::Fire() {
	if(FiringStatus == EFiringStatus::Reloading) { return; }
	if(!ensure(Barrel && ProjectileBlueprint)) { return; }

	AProjectile *Projectile = GetWorld()->SpawnActor<AProjectile>(
		ProjectileBlueprint,
		Barrel->GetSocketLocation(FName("Projectile")),
		Barrel->GetSocketRotation(FName("Projectile"))
	);
	Projectile->LaunchProjectile(LaunchSpeed);
	LastFireTime = FPlatformTime::Seconds();
	FiringStatus = EFiringStatus::Reloading;
}

void UTankAimingComponent::AimAt(FVector HitLocation) {
	if(!ensure(Barrel && Turret)) { return; }

	FVector OutLaunchVelocity;
	FVector StartLocation = Barrel->GetSocketLocation(FName("Projectile"));
	bool bHaveAimSolution = UGameplayStatics::SuggestProjectileVelocity(
		GetWorld(),
		OutLaunchVelocity,
		StartLocation,
		HitLocation,
		LaunchSpeed,
		false,
		0.f,
		0.f,
		ESuggestProjVelocityTraceOption::DoNotTrace,
		FCollisionResponseParams::DefaultResponseParam,
		TArray<AActor*>(),
		false
	);

	if(bHaveAimSolution) {
		FVector AimDirection = OutLaunchVelocity.GetSafeNormal();
		MoveBarrel(AimDirection);
		MoveTurret(AimDirection);
	} else {
		// float Time = GetWorld()->GetTimeSeconds();
		// UE_LOG(LogTemp, Warning, TEXT("%f: No aim solve found"), Time);
	}
}

void UTankAimingComponent::MoveBarrel(FVector AimDirection) {
	if(!ensure(Barrel)) { return; }
	FRotator DeltaRotation = GetDeltaRotation(
		AimDirection,
		Barrel->GetForwardVector().Rotation()
	);

	Barrel->Elevate(DeltaRotation.Pitch);
}

void UTankAimingComponent::MoveTurret(FVector AimDirection) {
	if(!ensure(Turret)) { return; }
	FRotator DeltaRotation = GetDeltaRotation(
		AimDirection,
		Turret->GetForwardVector().Rotation()
	);

	Turret->Rotate(DeltaRotation.Yaw);
}

FRotator UTankAimingComponent::GetDeltaRotation(FVector AimDirection, FRotator Rotation) {
	return AimDirection.Rotation() - Rotation;
}
