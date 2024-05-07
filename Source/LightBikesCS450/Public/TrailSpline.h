// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LightTrail.h"
#include "Components/SplineComponent.h"
#include "TrailSpline.generated.h"

UCLASS()
class LIGHTBIKESCS450_API ATrailSpline : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATrailSpline();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Spline)
	USplineComponent* Spline;
	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/// <summary>
	/// Spawns a spline point on Spline
	/// </summary>
	/// <param name="position">Location of spline point</param>
	void SpawnSplinePoint(FVector& position);
};
