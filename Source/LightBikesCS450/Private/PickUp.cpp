// Fill out your copyright notice in the Description page of Project Settings.

#include <LightBikesCS450/LightBikesCS450Character.h>
#include "PickUp.h"

// Sets default values
APickUp::APickUp()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Sets up collisions
	OnActorBeginOverlap.AddDynamic(this, &APickUp::OnCompHit);
}

// Called when the game starts or when spawned
void APickUp::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void APickUp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APickUp::OnCompHit(AActor* MyOverlappedActor, AActor* OtherActor)
{
	charRef = Cast<ALightBikesCS450Character>(OtherActor);

	/*if (Cast<ALightBikesCS450Character>(OtherActor))
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("on actor begin overlap works!")));
	}*/

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("on actor begin overlap works!")));

	if (charRef)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("char collisions")));

		charRef->GainPoint();

		K2_DestroyActor();
	}
	
	//// Restarts game when player hits the trail
	//if ((OtherActor != NULL) && (OtherComp != NULL) && (OtherActor->ActorHasTag("Trail")) /*Cast<ALightTrail>(OtherActor) != NULL */)
	//{
	//	bool loaded = false;
	//	if (!loaded)
	//	{
	//		UGameplayStatics::OpenLevel(GetWorld(), "ThirdPersonMap");
	//		loaded = true;
	//	}

	//	//if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("I Hit: %s"), *OtherActor->GetName()));
	//}
}

