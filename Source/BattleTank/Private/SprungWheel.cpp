// Fill out your copyright notice in the Description page of Project Settings.


#include "../Public/SprungWheel.h"

// Sets default values
ASprungWheel::ASprungWheel()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PhysicsConstraint = CreateDefaultSubobject<UPhysicsConstraintComponent>(FName("Physics Constraint"));
	SetRootComponent(PhysicsConstraint);

	Wheel = CreateDefaultSubobject<UStaticMeshComponent>(FName("Wheel"));
	Wheel->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
}

// Called when the game starts or when spawned
void ASprungWheel::BeginPlay()
{
	Super::BeginPlay();
	
	SetupConstraint();
}

// Called every frame
void ASprungWheel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASprungWheel::SetupConstraint()
{
	AActor* ParentActor = GetAttachParentActor();
	if (!ParentActor)
	{
		return;
	}

	UPrimitiveComponent* BodyRoot = Cast<UPrimitiveComponent>(ParentActor->GetRootComponent());
	if (!BodyRoot)
	{
		return;
	}

	PhysicsConstraint->SetConstrainedComponents(
		BodyRoot, NAME_None,
		Wheel, NAME_None
	);
}
