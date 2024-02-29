// Copyright Epic Games, Inc. All Rights Reserved.

#include "LightBikesCS450Character.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "LightBikesCS450GameMode.h"
#include "Kismet/GameplayStatics.h"
#include "InputActionValue.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// ALightBikesCS450Character

ALightBikesCS450Character::ALightBikesCS450Character()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Sets up collisions
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &ALightBikesCS450Character::OnCompHit);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	/*bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;*/

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 5000.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 1500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 1000.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 1000;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	
	CameraBoom->bUsePawnControlRotation = false; // Rotate the arm based on the controller
	
	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm
	
	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
}

void ALightBikesCS450Character::Tick(float DeltaTime)
{
	// Rotates the actor towards the target rotation
	if (!GetActorRotation().Equals(FRotator(0.f, TargetRotation, 0.f), 0.1f))
	{
		FRotator NewRotation = FMath::RInterpConstantTo(GetActorRotation(), FRotator(0.f, TargetRotation, 0.f), DeltaTime, 0.f);
		SetActorRotation(NewRotation);

		if (GetActorRotation().Equals(FRotator(0.f, TargetRotation, 0.f), 0.1f))
		{
			RotateActor = false;
		}
	}

	// add movement 
	AddMovementInput(GetActorForwardVector(), 15.0f);
}

void ALightBikesCS450Character::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	// Sets the camera position and rotation
	CameraBoom->TargetArmLength = -100.0f; // The camera follows at this distance behind the character	
	CameraBoom->SetRelativeLocation(FVector(0.0, 0.0f, 900.0f));
	FollowCamera->SetWorldRotation(FRotator(-90.0f, 0.0f, 0.0f));
}

//////////////////////////////////////////////////////////////////////////
// Input

void ALightBikesCS450Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Jumping
		/*EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);*/

		// Moving
		//EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ALightBikesCS450Character::Move);

		EnhancedInputComponent->BindAction(RightAction, ETriggerEvent::Triggered, this, &ALightBikesCS450Character::TurnRight);
		EnhancedInputComponent->BindAction(LeftAction, ETriggerEvent::Triggered, this, &ALightBikesCS450Character::TurnLeft);

		// Looking
		//EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ALightBikesCS450Character::Look);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void ALightBikesCS450Character::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		
		// add movement 
		//AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
		
	}
}

void ALightBikesCS450Character::TurnRight(const FInputActionValue& Value)
{
	TargetRotation = GetActorRotation().Yaw + 90.0f;

	// Failed experiments
	/*UE_LOG(LogTemp, Warning, TEXT("Right, %f"), newRot);
	UE_LOG(LogTemp, Warning, TEXT("original, %f"), GetActorRotation().Yaw);*/
	//SetActorRotation(FRotator(0.0, newRot, 0.0));
	//SetControlRotation(FRotator(GetActorRotation().Pitch, GetControlRotation().Yaw, GetActorRotation().Roll));
	//AddActorLocalRotation(FRotator(0.0, 100.f, 0.0f));
	//AddControllerYawInput(90.f);
	//AddActorWorldRotation(FRotator(0.0, 90.0f, 0.0f));
	//SetControlRotation(FQuat(FVector::UpVector, FMath::DegreesToRadians(90.0f)));	
}
void ALightBikesCS450Character::TurnLeft(const FInputActionValue& Value)
{
	TargetRotation = GetActorRotation().Yaw - 90.0f;

	// Failed experiments
	//SetActorRotation(FRotator(0.0, GetActorRotation().Yaw - 90, 0.0));
	//AddControllerYawInput(-90);
	//AddActorLocalRotation(FRotator(0.0, -100.f, 0.0f));
	//this->AddActorLocalRotation(FRotator(0.0f, 50.f, 0.0f), false);
	//AddActorWorldRotation(FRotator(0.0, -90.f, 0.0f));
	//SetActorRotation(FRotator(GetActorRotation().Add(0.0, -90, 0.0)), ETeleportType::None);
}

void ALightBikesCS450Character::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		//AddControllerYawInput(LookAxisVector.X);
		//AddControllerPitchInput(LookAxisVector.Y);
	}
}

void ALightBikesCS450Character::OnCompHit(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Restarts game when player hits the trail
	if ((OtherActor != NULL)  && (OtherComp != NULL) && (OtherActor->ActorHasTag("Trail")))
	{
		bool loaded = false;
		if (!loaded)
		{
			UGameplayStatics::OpenLevel(GetWorld(), "ThirdPersonMap");
			loaded = true;
		}
		
		//if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("I Hit: %s"), *OtherActor->GetName()));
	}
}

void ALightBikesCS450Character::GainPoint() {
	++points;
}

int ALightBikesCS450Character::GetPoints() {
	return points;
}