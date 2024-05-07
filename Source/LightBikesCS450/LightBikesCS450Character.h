// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "Components/SplineComponent.h"
#include "TrailSpline.h"
#include "TimerManager.h"
#include "LightBikesCS450Character.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config=Game)
class ALightBikesCS450Character : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;
	
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* RightAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LeftAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Spline, meta = (AllowPrivateAccess = "true"))
	float SplineTimerInterval;

	int points = 0;

public:
	ALightBikesCS450Character();
	
	UFUNCTION(BlueprintCallable, category = "Game")
	void GainPoint();

	UFUNCTION(BlueprintCallable, category = "Game")
	int GetPoints();

protected:

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	void TurnRight(const FInputActionValue& Value);
	void TurnLeft(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	void Tick(float DeltaTime);

	bool RotateActor = false;
	float TargetRotation = 0.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spline")
	TSubclassOf<ATrailSpline> TrailSplineBP;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = LightTrailBP, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<ALightTrail> LightTrailBP;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UUserWidget> _endScreen;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	int _maxPoints;

	//// Instance of end-game screen
	UPROPERTY()
	UUserWidget* CurrentWidget;
			
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// To add mapping context
	virtual void BeginPlay();

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	UFUNCTION()
	void OnCompHit(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:

	FTimerHandle _splinePointTimer;
	ATrailSpline* _spawnedSpline;

	/// <summary>
	/// Creates the end game widget
	/// </summary>
	void CreateEndGameWidget();

	/// <summary>
	/// Initializes the spline component
	/// </summary>
	void SetupSpline();

	/// <summary>
	/// Starts a timer that spawns spline points
	/// </summary>
	void SplinePointTimer();

	/// <summary>
	/// Spawns a spline point
	/// </summary>
	void SpawnPoint();

	/// <summary>
	/// Spawns a light trail actor for player collisions
	/// </summary>
	void SpawnLightTrail(FTransform& spawnLoc);
};

