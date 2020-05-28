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
}

// Called when the game starts
void UTankAimingComponent::BeginPlay()
{
	Super::BeginPlay();

	LastFireTime = FPlatformTime::Seconds();
}

// Called every frame
void UTankAimingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	if (RoundsLeft <= 0)
	{
		FiringStatus = EFiringStatus::OutOfAmmo;
	}
	else if ((FPlatformTime::Seconds() - LastFireTime) < ReloadTimeInSeconds)
	{
		FiringStatus = EFiringStatus::Reloading;
	}
	else if (IsBarrelMoving())
	{
		FiringStatus = EFiringStatus::Aiming;
	}
	else
	{
		FiringStatus = EFiringStatus::Locked;
	}
}

void UTankAimingComponent::Initialize(UTankBarrel* BarrelToSet, UTankTurret* TurretToSet, TSubclassOf<AProjectile> ProjectileToSet)
{
	if (!ensure(BarrelToSet && TurretToSet))
	{
		return;
	}

	Barrel = BarrelToSet;
	Turret = TurretToSet;

	ProjectileBlueprint = ProjectileToSet;
}

int32 UTankAimingComponent::GetRoundsLeft() const
{
	return RoundsLeft;
}

void UTankAimingComponent::Fire()
{
	if (FiringStatus == EFiringStatus::Reloading || FiringStatus == EFiringStatus::OutOfAmmo)
	{
		return;
	}
	if (!ensure(Barrel))
	{
		return;
	}

	AProjectile* Projectile = GetWorld()->SpawnActor<AProjectile>(
		ProjectileBlueprint,
		Barrel->GetSocketLocation(FName("Projectile")),
		Barrel->GetSocketRotation(FName("Projectile"))
	);
	Projectile->LaunchProjectile(LaunchSpeed);
	LastFireTime = FPlatformTime::Seconds();
	FiringStatus = EFiringStatus::Reloading;
	RoundsLeft--;
}

void UTankAimingComponent::AimAt(FVector HitLocation)
{
	if (!ensure(Barrel && Turret))
	{
		return;
	}

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

	if (bHaveAimSolution)
	{
		AimDirection = OutLaunchVelocity.GetSafeNormal();
		MoveBarrel(AimDirection);
		MoveTurret(AimDirection);
	}
	else
	{
		// float Time = GetWorld()->GetTimeSeconds();
		// UE_LOG(LogTemp, Warning, TEXT("%f: No aim solve found"), Time);
	}
}

bool UTankAimingComponent::IsBarrelMoving()
{
	if (!ensure(Barrel))
	{
		return false;
	}
	FVector BarrelForward = Barrel->GetForwardVector();

	return !(AimDirection.Equals(BarrelForward, 0.01f));
}

void UTankAimingComponent::MoveBarrel(FVector Direction)
{
	if (!ensure(Barrel))
	{
		return;
	}
	FRotator DeltaRotation = GetDeltaRotation(
		Direction,
		Barrel->GetForwardVector().Rotation()
	);

	Barrel->Elevate(DeltaRotation.Pitch);
}

void UTankAimingComponent::MoveTurret(FVector Direction)
{
	if (!ensure(Turret))
	{
		return;
	}
	FRotator DeltaRotation = GetDeltaRotation(
		Direction,
		Turret->GetForwardVector().Rotation()
	);
	if (FMath::Abs(DeltaRotation.Yaw) >= 180.f)
	{
		DeltaRotation.Yaw = 180.f - DeltaRotation.Yaw;
	}

	Turret->Rotate(DeltaRotation.Yaw);
}

FRotator UTankAimingComponent::GetDeltaRotation(FVector Direction, FRotator Rotation)
{
	return Direction.Rotation() - Rotation;
}

EFiringStatus UTankAimingComponent::GetFiringStatus() const
{
	return FiringStatus;
}
