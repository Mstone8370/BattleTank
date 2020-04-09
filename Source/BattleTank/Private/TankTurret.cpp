// Fill out your copyright notice in the Description page of Project Settings.

#include "../Public/TankTurret.h"

void UTankTurret::Rotate(float RelativeSpeed) {
    RelativeSpeed = FMath::Clamp<float>(RelativeSpeed, -1.f, 1.f);
    float RotationChange = RelativeSpeed * MaxDegreesPerSecond * GetWorld()->GetDeltaSeconds();
    float Rotation = RelativeRotation.Yaw + RotationChange;

    SetRelativeRotation(FRotator(0.f, Rotation, 0.f));
}