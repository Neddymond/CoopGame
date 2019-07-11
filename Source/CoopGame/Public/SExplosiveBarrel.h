// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SExplosiveBarrel.generated.h"

class USHealthComponent;
class URadialForceComponent;
class UMaterialInstance;

UCLASS()
class COOPGAME_API ASExplosiveBarrel : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASExplosiveBarrel();

protected:


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
		UStaticMeshComponent* MeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
		USHealthComponent* HealthComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
		URadialForceComponent* RadialForceComponent;

	UFUNCTION()
		void OnHealthChanged(USHealthComponent* OwningHealthComponent, float Health, float HealthDelta, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;

	/** Particle to play when health reaches zero */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FX")
	UParticleSystem* ExplosionEffects;

	/** Material used to replace the original one once it explodes */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FX")
	UMaterialInterface* ExplodedMaterial;

	/** Impulse applied to the barrel to boost it up a little when it explodes */
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category = "FX")
	float ExplosionImpulse;

	/** True when Exploded */
	UPROPERTY(ReplicatedUsing=OnRep_Explode)
	bool bExploded;

	/** Called when bExploded is true */
	UFUNCTION()
		void OnRep_Explode();

public:	

};
