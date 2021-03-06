// Fill out your copyright notice in the Description page of Project Settings.

#include "../Public/TankAIController.h"
#include "../Public/Tank.h"
#include "../Public/TankAimingComponent.h"

void ATankAIController::BeginPlay()
{
    Super::BeginPlay();
}

void ATankAIController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    UTankAimingComponent* ControlledPawnAimingComponent = 
        GetPawn()->FindComponentByClass<UTankAimingComponent>();
    APawn* PlayerPawn = GetWorld()->GetFirstPlayerController()->GetPawn();
    if (!ensure(ControlledPawnAimingComponent && PlayerPawn))
    {
        return;
    }
    
    // Move towards the player
    MoveToActor(PlayerPawn, AcceptanceRadius);

    // Aim towards the player
    ControlledPawnAimingComponent->AimAt(PlayerPawn->GetActorLocation());

    if (ControlledPawnAimingComponent->GetFiringStatus() == EFiringStatus::Locked)
    {
        ControlledPawnAimingComponent->Fire();
    }
}

void ATankAIController::SetPawn(APawn* InPawn)
{
    Super::SetPawn(InPawn);

    if (InPawn)
    {
        ATank* PossessedTank = Cast<ATank>(InPawn);
        if (!ensure(PossessedTank))
        {
            return;
        }

        PossessedTank->OnDeath.AddUniqueDynamic(this, &ATankAIController::OnPossessedTankDeath);
    }
}

void ATankAIController::OnPossessedTankDeath()
{
    if (!ensure(GetPawn()))
    {
        return;
    }

    GetPawn()->DetachFromControllerPendingDestroy();
}
