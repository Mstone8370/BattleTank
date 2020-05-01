// Fill out your copyright notice in the Description page of Project Settings.

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
	
}

// Called every frame
void UTankAimingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UTankAimingComponent::Initialize(UTankBarrel *BarrelToSet, UTankTurret *TurretToSet) {
	if(!ensure(BarrelToSet && TurretToSet)) { return; }

	Barrel = BarrelToSet;
	Turret = TurretToSet;
}

void UTankAimingComponent::AimAt(FVector HitLocation, float LaunchSpeed) {
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

UTankBarrel *UTankAimingComponent::GetBarrel() const {
	return Barrel;
}

void UTankAimingComponent::MoveBarrel(FVector AimDirection) {
	FRotator DeltaRotation = GetDeltaRotation(
		AimDirection,
		Barrel->GetForwardVector().Rotation()
	);

	Barrel->Elevate(DeltaRotation.Pitch);
}

void UTankAimingComponent::MoveTurret(FVector AimDirection) {
	FRotator DeltaRotation = GetDeltaRotation(
		AimDirection,
		Turret->GetForwardVector().Rotation()
	);

	Turret->Rotate(DeltaRotation.Yaw);
}

FRotator UTankAimingComponent::GetDeltaRotation(FVector AimDirection, FRotator Rotation) {
	return AimDirection.Rotation() - Rotation;
}
