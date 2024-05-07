// Fill out your copyright notice in the Description page of Project Settings.


#include "LightTrail.h"

// Sets default values
ALightTrail::ALightTrail()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SetupCollisions();
}

// Called when the game starts or when spawned
void ALightTrail::BeginPlay()
{
	Super::BeginPlay();

	StartCollisionTimer();

	SetLifeSpan(LifeSpan);
}

// Called every frame
void ALightTrail::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALightTrail::StartCollisionTimer()
{
	GetWorldTimerManager().SetTimer(_collisionTimer, this, &ALightTrail::EnableOverlapEvents, TimerDelay, false, TimerDelay);
}

void ALightTrail::EnableOverlapEvents()
{
	Collider->SetGenerateOverlapEvents(true);
}

void ALightTrail::SetupCollisions()
{
	Collider = CreateDefaultSubobject<UBoxComponent>(TEXT("Collider"));
	RootComponent = Collider;
	Collider->SetupAttachment(RootComponent, NAME_None);
	Collider->SetGenerateOverlapEvents(false);
	Collider->SetBoxExtent(FVector(32.f, 32.f, 32.f), false);
	Collider->SetCollisionProfileName(TEXT("Trigger"), false);
}

