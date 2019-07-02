// Fill out your copyright notice in the Description page of Project Settings.

#include "SWeapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Engine.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"

int32 DebugWeaponDrawing = 0;
FAutoConsoleVariableRef CVARDebugDrawing(TEXT("COOP.DebugWeapons"), DebugWeaponDrawing, TEXT("Draw Debug Lines For Weapons"), ECVF_Cheat);

// Sets default values
ASWeapon::ASWeapon()
{
	/**
	 * Create a Skeletal Mesh component
	 * Make MeshComp the RootComponent
	 */
	MeshComp = CreateDefaultSubobject <USkeletalMeshComponent>(TEXT("MeshComp"));
	RootComponent = MeshComp;

	MuzzleSocketName = "MuzzleSocket";
	TracerTargetName = "Target";
}

void ASWeapon::Fire()
{
	/** Trace the world, from pawn eyes to crosshair location */

	AActor* MyOwner = GetOwner();
	if (MyOwner)
	{
		FVector EyeLocation;
		FRotator EyeRotation;

		/** Returns the point of view of the actor */
		MyOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);

		/** Direction of the hit */
		FVector ShotDirection = EyeRotation.Vector();

		/** End Location of the ray */
		FVector TraceEnd = EyeLocation + (ShotDirection * 10000);

		/** Additional Parameters used for the trace */
		FCollisionQueryParams QueryParams;

		/** Actors for the Trace to Ignore */
		QueryParams.AddIgnoredActor(MyOwner);
		QueryParams.AddIgnoredActor(this);

		/** Trace against a complex collision */
		QueryParams.bTraceComplex = true;

		/** Particle "Target' Parameter */
		FVector TracerEndPoint = TraceEnd;

		FHitResult Hit;
		/**
		 * LineTraceSingleByChannel -> Traces a ray against the world by a channel and returns 
		 the first blocking hit
		 * Returns true if a blocking hit is found
		 */
		if (GetWorld()->LineTraceSingleByChannel(Hit, EyeLocation, TraceEnd, ECC_Visibility, QueryParams))
		{
			/** Actor that will be damaged */
			AActor* HitActor = Hit.GetActor();

			GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Yellow, "Hit");

			/** Hurt the specified actor with the specified impact */
			UGameplayStatics::ApplyPointDamage(HitActor, 20.0f, ShotDirection, Hit, MyOwner->GetInstigatorController(), this, DamageType);

			if (ImpactEffect)
			{
				/** Spawn Particle effect at the hit location */
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactEffect, Hit.ImpactPoint, Hit.ImpactNormal.Rotation());
			}

			TracerEndPoint = Hit.ImpactPoint;
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Yellow, "UnHit");
		}

		if (DebugWeaponDrawing > 0)
		{
			DrawDebugLine(GetWorld(), EyeLocation, TraceEnd, FColor::Red, false, 1.f, 0, 1.f);
		}

		PlayFireEffects(TracerEndPoint);
	}
}

void ASWeapon::PlayFireEffects(FVector TraceEnd)
{
	if (MuzzleEffect)
	{
		/** Spawn Particle effect at the Muzzle */
		UGameplayStatics::SpawnEmitterAttached(MuzzleEffect, MeshComp, MuzzleSocketName);
	}

	if (TracerEffects)
	{
		/** Get world-space socket or bone location */
		FVector MuzzleLocation = MeshComp->GetSocketLocation(MuzzleSocketName);

		UParticleSystemComponent* TracerComponent = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), TracerEffects, MuzzleLocation);

		if (TracerComponent)
		{
			TracerComponent->SetVectorParameter(TracerTargetName, TraceEnd);
		}
	}
}

