// Fill out your copyright notice in the Description page of Project Settings.

#include "../Public/TankBarrel.h"

void UTankBarrel::Elevate(float RelativeSpeed)
{
    // RelativeSpeed = FMath::Clamp<float>(RelativeSpeed, -1.f, 1.f);
    // float ElevationChange = RelativeSpeed * MaxDegreesPerSecond * GetWorld()->GetDeltaSeconds();
    // float RawNewElevation = RelativeRotation.Pitch + ElevationChange;
    // float Elevation = FMath::Clamp<float>(
    //     RawNewElevation,
    //     MinElevationDegrees,
    //     MaxElevationDegrees
    // );

    // SetRelativeRotation(FRotator(Elevation, 0.f, 0.f));
}