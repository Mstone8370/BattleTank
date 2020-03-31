// Fill out your copyright notice in the Description page of Project Settings.


#include "TankAIController.h"

void ATankAIController::BeginPlay() {
    Super::BeginPlay();

    auto ControlledTank = GetControlledTank();
    if(ControlledTank != nullptr) {
        UE_LOG(LogTemp, Warning, TEXT("Log from TankAIController: %s"), *ControlledTank->GetName());
    }

    ATank* PlayerTank = GetPlayerTank();
    if(PlayerTank != nullptr) {
        UE_LOG(LogTemp, Warning, TEXT("PlayerTankkkkk: %s"), *PlayerTank->GetName());
    }
}

ATank* ATankAIController::GetControlledTank() const {
    return Cast<ATank>(GetPawn());
}

ATank* ATankAIController::GetPlayerTank() const {
    auto PlayerPawn = GetWorld()->GetFirstPlayerController()->GetPawn();
    if(PlayerPawn == nullptr) {
        return nullptr;
    }

    return Cast<ATank>(PlayerPawn);
}