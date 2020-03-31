// Fill out your copyright notice in the Description page of Project Settings.


#include "TankAIController.h"

void ATankAIController::BeginPlay() {
    Super::BeginPlay();

    auto ControlledTank = GetPawn();

    if(ControlledTank != nullptr) {
        UE_LOG(LogTemp, Warning, TEXT("Log from TankAIController: %s"), *ControlledTank->GetName());
    }
}