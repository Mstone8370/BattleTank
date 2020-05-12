// Fill out your copyright notice in the Description page of Project Settings.

#include "../Public/TankTrack.h"

UTankTrack::UTankTrack() {
    PrimaryComponentTick.bCanEverTick = true;
}

void UTankTrack::BeginPlay() {
    Super::BeginPlay();
    
    OnComponentHit.AddDynamic(this, &UTankTrack::OnHit);
}

void UTankTrack::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    
    float SlippageSpeed = FVector::DotProduct(GetRightVector(), GetComponentVelocity());
    FVector CorrectionAcceleration = -SlippageSpeed / DeltaTime * GetRightVector();
    auto TankRoot = Cast<UStaticMeshComponent>(GetOwner()->GetRootComponent());
    FVector CorrectionForce = TankRoot->GetMass() * CorrectionAcceleration / 2;
    TankRoot->AddForce(CorrectionForce);
}

void UTankTrack::SetThrottle(float Throttle) {
    FVector ForceApplied = GetForwardVector() * TrackMaxDrivingForce * Throttle;
    FVector ForceLocation = GetComponentLocation();
    UPrimitiveComponent* TankRoot = Cast<UPrimitiveComponent>(GetOwner()->GetRootComponent());
    TankRoot->AddForceAtLocation(ForceApplied, ForceLocation);
}

void UTankTrack::OnHit(UPrimitiveComponent *HitComponent, AActor *OtherActor, UPrimitiveComponent *OtherComponent, FVector NormalImpulse, const FHitResult &Hit) {
    UE_LOG(LogTemp, Warning, TEXT("On Ground"));
}