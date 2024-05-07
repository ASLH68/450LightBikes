// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "LightTrail.generated.h"

UCLASS()
class LIGHTBIKESCS450_API ALightTrail : public AActor
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = LightTrail, meta = (AllowPrivateAccess = "true"))
	float LifeSpan;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = LightTrail, meta = (AllowPrivateAccess = "true"))
	float TimerDelay;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = LightTrail, meta = (AllowPrivateAccess = true)) 	     
	class UBoxComponent* Collider;
	
public:	
	// Sets default values for this actor's properties
	ALightTrail();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/// <summary>
	/// Enables collisions
	/// </summary>
	/// <param name="position">Location of spline point</param>
	void StartCollisionTimer();

	/// <summary>
	/// Enables collisions
	/// </summary>
	void EnableOverlapEvents();

private:

	/// <summary>
	/// Sets up the collider
	/// </summary>
	void SetupCollisions();

	FTimerHandle _collisionTimer;

};
