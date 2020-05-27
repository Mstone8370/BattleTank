// Fill out your copyright notice in the Description page of Project Settings.

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
}

// Called to bind functionality to input
void ATank::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

float ATank::TakeDamage(float DamageAmount, struct FDamageEvent const & DamageEvent, class AController *EventInstigator, AActor *DamageCauser) {
	int32 DamagePoint = FPlatformMath::RoundToInt(DamageAmount);
	int32 DamageToApply = FMath::Clamp<int32>(DamagePoint, 0.f, CurrentHealth);

	CurrentHealth -= DamageToApply;

	// TODO delete log
	UE_LOG(LogTemp, Warning, TEXT("DamageAmount: %f, DamageToApply: %d"), DamageAmount, DamageToApply);
	if(CurrentHealth <= 0) {
		UE_LOG(LogTemp, Warning, TEXT("Tank is dead"));
	}

	return DamageToApply;
}

float ATank::GetHealthPercent() const {
	return (float)CurrentHealth / StartingHealth;
}
