// Fill out your copyright notice in the Description page of Project Settings.

#include "SExplosiveBarrel.h"
#include "Components/StaticMeshComponent.h"
#include "SHealthComponent.h"
#include "Kismet/GameplayStatics.h"
#include "PhysicsEngine/RadialForceComponent.h"
#include "Engine/Engine.h"
#include "Net/UnrealNetwork.h"


// Sets default values
ASExplosiveBarrel::ASExplosiveBarrel()
{
	/** Create a static mesh component */
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MesComponent"));
	RootComponent = MeshComponent;
	MeshComponent->SetSimulatePhysics(true);

	/** Set collision Channel that this object uses to physicBody
	to let radial component affect us */
	MeshComponent->SetCollisionObjectType(ECC_PhysicsBody);

	/** Create a Health Component */
	HealthComponent = CreateDefaultSubobject<USHealthComponent>(TEXT("HealthComponent"));

	HealthComponent->OnHealthChanged.AddDynamic(this, &ASExplosiveBarrel::OnHealthChanged);

	/** Create a RadialForceComponent */
	RadialForceComponent = CreateDefaultSubobject<URadialForceComponent>(TEXT("RadialForceComponent"));
	RadialForceComponent->SetupAttachment(MeshComponent);
	RadialForceComponent->Radius = 250;

	RadialForceComponent->bImpulseVelChange = true;

	/** Prevent component from Ticking, Only use fireImpulse() instead */
	RadialForceComponent->bAutoActivate = false;

	/** Ignore self */
	RadialForceComponent->bIgnoreOwningActor = true;

	ExplosionImpulse = 400;

	SetReplicates(true);
	SetReplicateMovement(true);

}


void ASExplosiveBarrel::OnHealthChanged(USHealthComponent* OwningHealthComponent, float Health, float HealthDelta, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
	if (bExploded)
	{
		return;
	}

	if (Health <= 0.0f)
	{
		/** Explode */
		bExploded = true;

		OnRep_Explode();

		/** Boost the Barrel Upwards */
		FVector BoostIntensity = FVector::UpVector * ExplosionImpulse;

		/** Add an impulse to the barrel to blow it up */
		MeshComponent->AddImpulse(BoostIntensity, NAME_None, true);

		/** Blast away nearby Actors */
		RadialForceComponent->FireImpulse();

	}
}

/**
 * Called when bExploded is true
 * Replicate Visual effects on Client
 */
void ASExplosiveBarrel::OnRep_Explode()
{
	/** Play Explosion Effects */
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionEffects, GetActorLocation());

	/** Override Mesh color to black */
	MeshComponent->SetMaterial(0, ExplodedMaterial);
}

void ASExplosiveBarrel::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ASExplosiveBarrel, bExploded);
}



