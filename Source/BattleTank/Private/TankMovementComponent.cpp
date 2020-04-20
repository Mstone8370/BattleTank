// Fill out your copyright notice in the Description page of Project Settings.

#include "../Public/TankTrack.h"
#include "../Public/TankMovementComponent.h"

void UTankMovementComponent::Initialize(UTankTrack *LeftTrackToSet, UTankTrack *RightTrackToSet) {
    if(!(LeftTrackToSet && RightTrackToSet)) { return; }

    LeftTrack = LeftTrackToSet;
    RightTrack = RightTrackToSet;
}

void UTankMovementComponent::IntendMoveForward(float Throw) {
    UE_LOG(LogTemp, Warning, TEXT("Intend move forward throw: %f"), Throw);
    
    LeftTrack->SetThrottle(Throw);
    RightTrack->SetThrottle(Throw);
}