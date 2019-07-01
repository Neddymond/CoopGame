// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SWeapon.generated.h"

class USkeletlMeshComponent;
class UDamageType;

UCLASS()
class COOPGAME_API ASWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/** Visual element of our weapon */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USkeletalMeshComponent* MeshComp;

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	virtual void Fire();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	TSubclassOf<UDamageType> DamageType;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	FName MuzzleSocketName;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
		FName TracerTargetName;

	/** Muzzle Particle Effects */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
		UParticleSystem* MuzzleEffect;

	/** Impact Particle Effects */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
		UParticleSystem* ImpactEffect;
	/** Tracer particle effects */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
		UParticleSystem* TracerEffects;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	
};
