// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class ASWeapon;

UCLASS()
class COOPGAME_API ASCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/** 
	  * Functions for character movements
	  * Move character forward
	  * Move Character Right
	  */
	void MoveForward(float value);
	void MoveRight(float value);

	/** Third Person Camera component */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
		UCameraComponent* CameraComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	USpringArmComponent* SpringArmComponent;

	/** Animation Functions */
	void BeginCrouch();
	void EndCrouch();
	void BeginJump();
	void EndJump();
	void BeginZoom();
	void EndZoom();

	/** Character's current weapon */
	ASWeapon* CurrentWeapon;

	/** Fire a Weapon */
	void StartFire();

	/** Stop firing a weapon */
	void StopFire();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player")
	TSubclassOf<ASWeapon>StarterWeaponClass;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Player")
	FName WeaponAttachSocketName;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual FVector GetPawnViewLocation() const override;

	bool bWantsToZoom;

	/** Camera's Zoomed field of view */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player")
	float ZoomedFOV;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player", meta = (clampMin = 0.1, ClampMax = 100.0))
	float ZoomInterpSpeed;

	/** default field of view: Set during begin play */
	float DefaultFOV;
	
};
