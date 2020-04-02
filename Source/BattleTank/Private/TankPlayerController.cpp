// Fill out your copyright notice in the Description page of Project Settings.


#include "../Public/TankPlayerController.h"

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
    int32 ViewportSizeX, ViewportSizeY;
    GetViewportSize(ViewportSizeX, ViewportSizeY);
    FVector2D ScreenLocation = FVector2D(ViewportSizeX * CrossHairXLocation, ViewportSizeY * CrossHairYLocation);
    // UE_LOG(LogTemp, Warning, TEXT("ScreenLocation: %s"), *ScreenLocation.ToString());

    FVector LookDirection;
    if(GetLookDirection(ScreenLocation, LookDirection)) {
        // UE_LOG(LogTemp, Warning, TEXT("Look Direction: %s"), *LookDirection.ToString());
        FVector HitLocation;
        if(GetLookVectorHitLocation(LookDirection, HitLocation)) {
            UE_LOG(LogTemp, Warning, TEXT("Hit Location: %s"), *HitLocation.ToString());
            DrawDebugPoint(GetWorld(), HitLocation, 100.f, FColor(255, 0, 0));
        }
    }

    return false;
}

bool ATankPlayerController::GetLookDirection(FVector2D ScreenLocation, FVector &OutWorldDirection) const {
    FVector CameraWorldLocation;
    return DeprojectScreenPositionToWorld(
        ScreenLocation.X,
        ScreenLocation.Y,
        CameraWorldLocation,
        OutWorldDirection
    );
}

bool ATankPlayerController::GetLookVectorHitLocation(FVector LookDirection, FVector &HitLocation) const {
    FHitResult HitResult;
    FVector StartLocation = PlayerCameraManager->GetCameraLocation();
    FVector EndLocation = StartLocation + (LookDirection * LineTraceRange);
    if(GetWorld()->LineTraceSingleByChannel(
        HitResult,
        StartLocation,
        EndLocation,
        ECC_Visibility
    )) {
        HitLocation = HitResult.Location;
        return true;
    }

    HitLocation = FVector(0.f);
    return false;
}