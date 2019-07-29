// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SWeapon.generated.h"

class USkeletlMeshComponent;
class UDamageType;
class UCameraShake;

/** Contains information of a single hitscan linetrace */
USTRUCT()
struct FHitScanTrace
{
	GENERATED_BODY()
		  
public:

	UPROPERTY()
	TEnumAsByte<EPhysicalSurface> SurfaceType;

	UPROPERTY()
	FVector_NetQuantize TraceTo;

};

UCLASS()
class COOPGAME_API ASWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASWeapon();

protected:

	virtual void BeginPlay() override;

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

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;

	UPROPERTY(EditAnywhere, Category = "Weapon")
	TSubclassOf<UCameraShake>FireCameraShake;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	float BaseDamage;

	virtual void Fire();

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerFire();

	FTimerHandle TimerHandle_TimeInBetweenShots;

	/** The last time a shot was fired */
	float LastFireTime;

	/** RPM - Bullets per minute fired by weapon */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	float RateOfFire;

	float TimeBetweenShots;

	// Bullet Spread
	UPROPERTY(EditAnywhere, Category = "Weapon", meta = (ClampMin = 0.0f))
	float BulletSpread;

	/** Trigger a function (OnRep_HitScanTrace) every time this property
	gets replicated */
	UPROPERTY(ReplicatedUsing=OnRep_HitScanTrace)
	FHitScanTrace HitScanTrace;

	UFUNCTION()
		void OnRep_HitScanTrace();

	void PlayImpactEffects(EPhysicalSurface SurfaceType, FVector ImpactPoint);

public:	

	void StartFire();

	void StopFire();
};