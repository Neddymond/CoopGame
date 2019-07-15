// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "STrackerBot.generated.h"

class USHealthComponent;
class USphereComponent;
class USoundCue;

UCLASS()
class COOPGAME_API ASTrackerBot : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ASTrackerBot();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	FVector GetNextPathPoint();

	 UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent*  MeshComponent;

	 UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "TrackerBot")
		 USphereComponent* SphereComponent;

	 /** Next point in Navigation path */
	 FVector NextPathPoint;

	 UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "TrackerBot")
	 bool bUseVelocityChange;

	 UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "TrackerBot")
	 float MovementForce;

	 UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "TrackerBot")
	 float  RequiredDistanceToTarget;

	 UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "TrackerBot")
	 USHealthComponent* HealthComponent;

	 UFUNCTION()
	 void HandleTakeDamage(USHealthComponent* OwningHealthComponent, float Health, float HealthDelta, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	 /** Material instance to pulse on damage */
	 UMaterialInstanceDynamic* MaterialInstance;

	 UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "TrackerBot")
	 UParticleSystem* ExplosionEffects;

	 UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "TrackerBot")
	float ExplosionDamage;

	 UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "TrackerBot")
	float ExplosionRadius;

	 UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "TrackerBot")
		 bool bExploded;

	 UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "TrackerBot")
	 bool bStartedExplosion;

	 FTimerHandle TimerHandle_SelfDamage;

	 void SelfDestruct();

	 void DamageSelf();

	 UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "TrackerBot")
	 USoundCue* SelfDestructSound;

	 UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "TrackerBot")
	 USoundCue* ExplosionSound;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	
};
