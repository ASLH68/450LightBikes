// Fill out your copyright notice in the Description page of Project Settings.


#include "TrailSpline.h"

// Sets default values
ATrailSpline::ATrailSpline()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Spline = CreateDefaultSubobject<USplineComponent>(TEXT("Spline Trail"));
	RootComponent = Spline;
}

// Called when the game starts or when spawned
void ATrailSpline::BeginPlay()
{
	Super::BeginPlay();	
}

void ATrailSpline::SpawnSplinePoint(FVector& position)
{
	Spline->AddSplinePoint(position, ESplineCoordinateSpace::World, true);
}

// Called every frame
void ATrailSpline::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

