// Fill out your copyright notice in the Description page of Project Settings.

#include "SCharacter.h"
#include "Components/InputComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/PawnMovementComponent.h"
//#include "Math/UnrealMathUtility.h"


// Sets default values
ASCharacter::ASCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	/**
	 * Create a SpringArmComponent
	 * Make SpringArmComponent the Root component
	 * Use the control rotation of the pawn where possible
	 */
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComponent->SetupAttachment(RootComponent);
	SpringArmComponent->bUsePawnControlRotation = true;

	/**
	 * Create a Camera component
	 *  Attach the Camera to the springArmComponent
	 */
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(SpringArmComponent);

	/** Enable support for Character Animation */
	GetMovementComponent()->GetNavAgentPropertiesRef().bCanCrouch = true;

	/** Camera's Zoomed field of view */
	ZoomedFOV = 65.0f;

	ZoomInterpSpeed = 20;
}

// Called when the game starts or when spawned
void ASCharacter::BeginPlay()
{
	Super::BeginPlay();

	/** Camera's Default field of view */
	DefaultFOV = CameraComponent->FieldOfView;

}

// Called every frame
void ASCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	/** Target field of view */
	float TargetFOV = bWantsToZoom ? ZoomedFOV : DefaultFOV;

	/** Interpolate from Current field of view to Target field of view */
	float NewFOV = FMath::FInterpTo(CameraComponent->FieldOfView, TargetFOV, DeltaTime, ZoomInterpSpeed);

	/** Set Camera's current field of view */
	CameraComponent->SetFieldOfView(NewFOV);

}


// Called to bind functionality to input
void ASCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	/**
	 * Bind the Inputs for the character movement
	 * MoveForward -> moves the character forward or backwards based on the value
	 * MoveRight -> moves the character right or left based on the value
	 */
	PlayerInputComponent->BindAxis("MoveForward", this, &ASCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ASCharacter::MoveRight);

	/** 
	 * Bind inputs for Mouse look
	 * AddControllerPitchInput -> enables the character to look up
	 * AddControllerYawInput enables the character to be able to look sideways
	 */
	PlayerInputComponent->BindAxis("LookUp", this, &ASCharacter::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("Turn", this, &ASCharacter::AddControllerYawInput);

	/**
	 * Bind Input for Character Animations
	 * Crouch -> Crouches character
	 * EndCrouch -> Stops character from crouching
	 * BeginJump -> Play character jump Animation
	 * EndJump -> Stop playing character jump Animation
	 * BeginZoom -> Zoom Camera
	 */
	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &ASCharacter::BeginCrouch);
	PlayerInputComponent->BindAction("Crouch", IE_Released, this, &ASCharacter::EndCrouch);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ASCharacter::BeginJump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ASCharacter::EndJump);
	PlayerInputComponent->BindAction("Zoom", IE_Pressed, this, &ASCharacter::BeginZoom);
	PlayerInputComponent->BindAction("Zoom", IE_Released, this, &ASCharacter::EndZoom);

}

/** returns pawn's eyes location */
FVector ASCharacter::GetPawnViewLocation() const
{
	if (CameraComponent)
	{
		return CameraComponent->GetComponentLocation();
	}

	return Super::GetPawnViewLocation();
}

/** Move character forward along the given world by getting the forward vector from this Actor in world space */
void ASCharacter::MoveForward(float value)
{
	if (value != 0.0f)
	{
		AddMovementInput(GetActorForwardVector() * value);
	}
}

/** Move character Right along the given world by getting the Right vector from this Actor in world space */
void ASCharacter::MoveRight(float value)
{
	if (value != 0.0f)
	{
		AddMovementInput(GetActorRightVector() * value);
	}
}

/** Crouch character */
void ASCharacter::BeginCrouch()
{
	Crouch();
}

/** Stop character from Crouching */
void ASCharacter::EndCrouch()
{
	UnCrouch();
}

/** Enable character jump Animation */
void ASCharacter::BeginJump()
{
	Jump();
}

/** Disable Character jump Animation */
void ASCharacter::EndJump()
{
	StopJumping();
}

void ASCharacter::BeginZoom()
{
	bWantsToZoom = true;
}

void ASCharacter::EndZoom()
{
	bWantsToZoom = false;
}

