// Fill out your copyright notice in the Description page of Project Settings.

#include "../Public/Tank.h"
#include "../Public/TankAIController.h"

void ATankAIController::BeginPlay() {
    Super::BeginPlay();
}

void ATankAIController::Tick(float DeltaTime) {
    Super::Tick(DeltaTime);

    ATank* ControlledTank = Cast<ATank>(GetPawn());
    auto PlayerPawn = GetWorld()->GetFirstPlayerController()->GetPawn();
    if(ControlledTank == nullptr || PlayerPawn == nullptr) { return; }
    ATank* PlayerTank = Cast<ATank>(PlayerPawn);
    
    // Move towards the player
    MoveToActor(PlayerTank, AcceptanceRadius);

    // Aim towards the player
    ControlledTank->AimAt(PlayerTank->GetActorLocation());
    ControlledTank->Fire();
}
