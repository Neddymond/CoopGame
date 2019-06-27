// Fill out your copyright notice in the Description page of Project Settings.

#include "SCharacter.h"
#include "Components/InputComponent.h"
#include "Camera/CameraComponent.h"


// Sets default values
ASCharacter::ASCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	/** Create a Camera component */
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));

	/** Use the control rotation of the pawn where possible */
	CameraComponent->bUsePawnControlRotation = true;
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

}

// Called when the game starts or when spawned
void ASCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

/** Move character forward along the given world by getting the forward vector from this Actor in world space */
void ASCharacter::MoveForward(float value)
{
	AddMovementInput(GetActorForwardVector() * value);
}

/** Move character Right along the given world by getting the Right vector from this Actor in world space */
void ASCharacter::MoveRight(float value)
{
	AddMovementInput(GetActorRightVector() * value);
}

// Called every frame
void ASCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


