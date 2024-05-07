// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "PickUp.generated.h"

class LightBikesCS450Character;

UCLASS()
class LIGHTBIKESCS450_API APickUp : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APickUp();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnCompHit(AActor* MyOverlappedActor, AActor* OtherActor);

private:
	class ALightBikesCS450Character* charRef;
};
