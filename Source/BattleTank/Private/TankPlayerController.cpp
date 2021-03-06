// Fill out your copyright notice in the Description page of Project Settings.

#include "../Public/TankPlayerController.h"
#include "../Public/Tank.h"
#include "../Public/TankAimingComponent.h"

void ATankPlayerController::BeginPlay()
{
	Super::BeginPlay();

    AimingComponent = GetPawn()->FindComponentByClass<UTankAimingComponent>();
    if(ensure(AimingComponent)) {
        FoundAimingComponent(AimingComponent);
    } else {
        UE_LOG(LogTemp, Error, TEXT("Player controller can't find aiming component at BeginPlay"));
    }

    ATank* PlayerTank = Cast<ATank>(GetPawn());
    if(ensure(PlayerTank))
    {
        PlayerTank->OnDeath.AddDynamic(this, &ATankPlayerController::OnPlayerTankDeath);
    }
}

void ATankPlayerController::Tick(float DeltaTime) {
    Super::Tick(DeltaTime);

    AimTowardCrosshair();
}

void ATankPlayerController::AimTowardCrosshair() {
    if(!ensure(AimingComponent && GetPawn())) { return; }

    FVector HitLocation;
    bool bGotHitLocation = GetSightRayHitLocation(HitLocation);
    if(bGotHitLocation) {
        AimingComponent->AimAt(HitLocation);
    }
}

bool ATankPlayerController::GetSightRayHitLocation(FVector& OutHitLocation) const {
    int32 ViewportSizeX, ViewportSizeY;
    GetViewportSize(ViewportSizeX, ViewportSizeY);
    FVector2D ScreenLocation = FVector2D(ViewportSizeX * CrossHairXLocation, ViewportSizeY * CrossHairYLocation);

    FVector LookDirection;
    if(GetLookDirection(ScreenLocation, LookDirection)) {
        return GetLookVectorHitLocation(LookDirection, OutHitLocation);
    }

    return false;
}

bool ATankPlayerController::GetLookDirection(FVector2D ScreenLocation, FVector& OutWorldDirection) const {
    FVector CameraWorldLocation;
    return DeprojectScreenPositionToWorld(
        ScreenLocation.X,
        ScreenLocation.Y,
        CameraWorldLocation,
        OutWorldDirection
    );
}

bool ATankPlayerController::GetLookVectorHitLocation(FVector LookDirection, FVector& OutHitLocation) const {
    FHitResult HitResult;
    FVector StartLocation = PlayerCameraManager->GetCameraLocation();
    FVector EndLocation = StartLocation + (LookDirection * LineTraceRange);
    if(GetWorld()->LineTraceSingleByChannel(
        HitResult,
        StartLocation,
        EndLocation,
        ECC_Camera
    )) {
        OutHitLocation = HitResult.Location;
        return true;
    }

    OutHitLocation = FVector(0.f);
    return false;
}

void ATankPlayerController::OnPlayerTankDeath() {
    UE_LOG(LogTemp, Warning, TEXT("Player Received"));
    // StartSpectatingOnly();
}
