// Fill out your copyright notice in the Description page of Project Settings.


#include "TankPlayerController.h"

void ATankPlayerController::BeginPlay()
{
	Super::BeginPlay();
    
    auto ControlledTank = GetControlledTank();

    if(ControlledTank != nullptr) {
        UE_LOG(LogTemp, Warning, TEXT("ControlledTank: %s"), *ControlledTank->GetName());
    }
}

void ATankPlayerController::Tick(float DeltaTime) {
    Super::Tick(DeltaTime);

    AimTowardCrosshair();
}

ATank* ATankPlayerController::GetControlledTank() const {
    return Cast<ATank>(GetPawn());
}

void ATankPlayerController::AimTowardCrosshair() {
    if(!GetControlledTank()) { return; }

    FVector HitLocation;
    if(GetSightRayHitLocation(HitLocation)) {
        UE_LOG(LogTemp, Warning, TEXT("HitLocation: %s"), *HitLocation.ToString());
    }
}

bool ATankPlayerController::GetSightRayHitLocation(FVector &OutHitLocation) const {
    OutHitLocation = FVector(1.f);
    return true;
}