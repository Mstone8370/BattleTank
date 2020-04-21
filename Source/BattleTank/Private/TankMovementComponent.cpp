// Fill out your copyright notice in the Description page of Project Settings.

#include "../Public/TankTrack.h"
#include "../Public/TankMovementComponent.h"

void UTankMovementComponent::Initialize(UTankTrack *LeftTrackToSet, UTankTrack *RightTrackToSet) {
    if(!(LeftTrackToSet && RightTrackToSet)) { return; }

    LeftTrack = LeftTrackToSet;
    RightTrack = RightTrackToSet;
}

void UTankMovementComponent::RequestDirectMove(const FVector& MoveVelocity, bool bForceMaxSpeed) {
    UE_LOG(LogTemp, Warning, TEXT("%s MoveVelocity: %s"), *GetName(), *MoveVelocity.ToString());
}

void UTankMovementComponent::IntendMoveForward(float Throw) {
    if(!(LeftTrack && RightTrack)) { return; }

    LeftTrack->SetThrottle(Throw);
    RightTrack->SetThrottle(Throw);
}

void UTankMovementComponent::IntendTurnRight(float Throw) {
    if(!(LeftTrack && RightTrack)) { return; }

    LeftTrack->SetThrottle(Throw);
    RightTrack->SetThrottle(-Throw);
}

void UTankMovementComponent::IntendTurnLeft(float Throw) {
    if(!(LeftTrack && RightTrack)) { return; }

    LeftTrack->SetThrottle(-Throw);
    RightTrack->SetThrottle(Throw);
}
