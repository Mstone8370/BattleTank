// Fill out your copyright notice in the Description page of Project Settings.

#include "../Public/TankBarrel.h"
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

void UTankAimingComponent::SetBarrelReference(UTankBarrel *BarrelToSet) {
	Barrel = BarrelToSet;
}

void UTankAimingComponent::AimAt(FVector HitLocation, float LaunchSpeed) {
	// FString OurTankName = GetOwner()->GetName();
	// FString BarrelLocation = Barrel->GetComponentLocation().ToString();
	// UE_LOG(LogTemp, Warning, TEXT("%s is aiming at %s from %s, Speed: %f"), *OurTankName, *HitLocation.ToString(), *BarrelLocation, LaunchSpeed);

	if(Barrel == nullptr) { return; }

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

		float Time = GetWorld()->GetTimeSeconds();
		UE_LOG(LogTemp, Warning, TEXT("%f: Aim solution found"), Time);
	} else {
		float Time = GetWorld()->GetTimeSeconds();
		UE_LOG(LogTemp, Warning, TEXT("%f: No aim solve found"), Time);
	}
}

void UTankAimingComponent::MoveBarrel(FVector AimDirection) {
	FRotator BarrelRotation = Barrel->GetForwardVector().Rotation();
	FRotator AimAsRotation = AimDirection.Rotation();
	FRotator DeltaRotation = AimAsRotation - BarrelRotation;

	Barrel->Elevate(5.f);
}