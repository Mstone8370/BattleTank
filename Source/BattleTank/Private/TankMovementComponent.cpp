// Fill out your copyright notice in the Description page of Project Settings.

#include "../Public/TankTrack.h"
#include "../Public/TankMovementComponent.h"

void UTankMovementComponent::Initialize(UTankTrack *LeftTrackToSet, UTankTrack *RightTrackToSet) {
    if(!ensure(LeftTrackToSet && RightTrackToSet)) { return; }

    LeftTrack = LeftTrackToSet;
    RightTrack = RightTrackToSet;
}

void UTankMovementComponent::RequestDirectMove(const FVector& MoveVelocity, bool bForceMaxSpeed) {
    FVector TankForward = GetOwner()->GetActorForwardVector().GetSafeNormal();
    FVector AIForwardIntention = MoveVelocity.GetSafeNormal();

    float ForwardThrow = FVector::DotProduct(TankForward, AIForwardIntention);
    IntendMoveForward(ForwardThrow);

    FVector Direction = FVector::CrossProduct(TankForward, AIForwardIntention);
    float TurnThrow;
    if(Direction.Z > 0.02f) {
        TurnThrow = 1.f;   
    } else if (Direction.Z < -0.02f) {
        TurnThrow = -1.f;
    } else {
        TurnThrow = 0.f;
    }
    IntendTurnRight(TurnThrow);
}

void UTankMovementComponent::IntendMoveForward(float Throw) {
    if(!ensure(LeftTrack && RightTrack)) { return; }

    LeftTrack->SetThrottle(Throw);
    RightTrack->SetThrottle(Throw);
}

void UTankMovementComponent::IntendTurnRight(float Throw) {
    if(!ensure(LeftTrack && RightTrack)) { return; }

    LeftTrack->SetThrottle(Throw);
    RightTrack->SetThrottle(-Throw);
}

void UTankMovementComponent::IntendTurnLeft(float Throw) {
    if(!ensure(LeftTrack && RightTrack)) { return; }

    LeftTrack->SetThrottle(-Throw);
    RightTrack->SetThrottle(Throw);
}
