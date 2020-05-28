// Fill out your copyright notice in the Description page of Project Settings.

#include "../Public/TankTrack.h"

UTankTrack::UTankTrack()
{
    PrimaryComponentTick.bCanEverTick = false;
    
	SetNotifyRigidBodyCollision(true);
}

void UTankTrack::BeginPlay()
{
    Super::BeginPlay();

    OnComponentHit.AddDynamic(this, &UTankTrack::OnHit);
}

void UTankTrack::SetThrottle(float Throttle)
{
    CurrentThrottle = FMath::Clamp<float>(CurrentThrottle + Throttle, -2.f, 2.f);
}

void UTankTrack::DriveTrack()
{
    FVector ForceApplied = GetForwardVector() * TrackMaxDrivingForce * CurrentThrottle;
    FVector ForceLocation = GetComponentLocation();
    UPrimitiveComponent* TankRoot = Cast<UPrimitiveComponent>(GetOwner()->GetRootComponent());
    TankRoot->AddForceAtLocation(ForceApplied, ForceLocation);
}

void UTankTrack::ApplySidewaysForce()
{
    float DeltaTime = GetWorld()->GetDeltaSeconds();

    float SlippageSpeed = FVector::DotProduct(GetRightVector(), GetComponentVelocity());
    FVector CorrectionAcceleration = -SlippageSpeed / DeltaTime * GetRightVector();

    auto TankRoot = Cast<UStaticMeshComponent>(GetOwner()->GetRootComponent());
    FVector CorrectionForce = TankRoot->GetMass() * CorrectionAcceleration / 2;
    TankRoot->AddForce(CorrectionForce);
}

void UTankTrack::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
    DriveTrack();
    ApplySidewaysForce();
    CurrentThrottle = 0.f;
}