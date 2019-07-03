// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SWeapon.generated.h"

class USkeletlMeshComponent;
class UDamageType;
class UCameraShake;

UCLASS()
class COOPGAME_API ASWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASWeapon();

protected:

	/** Visual element of our weapon */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		USkeletalMeshComponent* MeshComp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	TSubclassOf<UDamageType> DamageType;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	FName MuzzleSocketName;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
		FName TracerTargetName;

	/** Muzzle Particle Effects */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
		UParticleSystem* MuzzleEffect;

	/** Default Impact Particle Effects */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
		UParticleSystem* DefaultImpactEffect;

	/** Flesh Impact Particle Effects */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
		UParticleSystem* FleshImpactEffect;

	/** Tracer particle effects */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
		UParticleSystem* TracerEffects;

	/** Effects played when a weapon is fired */
	void PlayFireEffects(FVector TraceEnd);

	UPROPERTY(EditAnywhere, Category = "Weapon")
	TSubclassOf<UCameraShake>FireCameraShake;

public:	

	UFUNCTION(BlueprintCallable, Category = "Weapon")
		virtual void Fire();
};