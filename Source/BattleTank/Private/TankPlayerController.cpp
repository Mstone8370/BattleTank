// Fill out your copyright notice in the Description page of Project Settings.

#include "../Public/Tank.h"
#include "../Public/TankAimingComponent.h"
#include "../Public/TankPlayerController.h"

void ATankPlayerController::BeginPlay()
{
	Super::BeginPlay();
    
    auto ControlledTank = GetControlledTank();

    if(ControlledTank != nullptr) {
        UE_LOG(LogTemp, Warning, TEXT("ControlledTank: %s"), *ControlledTank->GetName());
    }

    auto AimingComponent = ControlledTank->GetTankAimingComponent();
    if(AimingComponent) {
        FoundAimingComponent(AimingComponent);
    } else {
        UE_LOG(LogTemp, Error, TEXT("Player controller can't find aiming component at BeginPlay"));
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
    if(!ensure(GetControlledTank())) { return; }

    FVector HitLocation;
    if(GetSightRayHitLocation(HitLocation)) {
        GetControlledTank()->AimAt(HitLocation);
    }
}

bool ATankPlayerController::GetSightRayHitLocation(FVector &OutHitLocation) const {
    int32 ViewportSizeX, ViewportSizeY;
    GetViewportSize(ViewportSizeX, ViewportSizeY);
    FVector2D ScreenLocation = FVector2D(ViewportSizeX * CrossHairXLocation, ViewportSizeY * CrossHairYLocation);

    FVector LookDirection;
    if(GetLookDirection(ScreenLocation, LookDirection)) {
        if(GetLookVectorHitLocation(LookDirection, OutHitLocation)) {
            // DrawDebugPoint(GetWorld(), OutHitLocation, 100.f, FColor(255, 0, 0));
            return true;
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

bool ATankPlayerController::GetLookVectorHitLocation(FVector LookDirection, FVector &OutHitLocation) const {
    FHitResult HitResult;
    FVector StartLocation = PlayerCameraManager->GetCameraLocation();
    FVector EndLocation = StartLocation + (LookDirection * LineTraceRange);
    if(GetWorld()->LineTraceSingleByChannel(
        HitResult,
        StartLocation,
        EndLocation,
        ECC_Visibility
    )) {
        OutHitLocation = HitResult.Location;
        return true;
    }

    OutHitLocation = FVector(0.f);
    return false;
}