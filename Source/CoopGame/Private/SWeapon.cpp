// Fill out your copyright notice in the Description page of Project Settings.

#include "SWeapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Engine.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "CoopGame.h"
#include "TimerManager.h"
#include "Net/UnrealNetwork.h"

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

	/** Weapon Attach point name */
	MuzzleSocketName = "MuzzleSocket";

	TracerTargetName = "Target";

	/** Damage value */
	BaseDamage = 20.0f;

	BulletSpread = 2.0f;

	RateOfFire = 600;

	/** Spawn/Replicate this Actor on client */
	SetReplicates(true);

	NetUpdateFrequency = 66.f;
	MinNetUpdateFrequency = 33.f;
}

void ASWeapon::BeginPlay()
{
	Super::BeginPlay();

	TimeBetweenShots = 60 / RateOfFire;
}

void ASWeapon::Fire()
{
	/** If it's the client, call ServerFire function */
	if (Role < ROLE_Authority)
	{
		ServerFire();
	}


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

		/** Bullet Spread */
		float HalfRad = FMath::DegreesToRadians(BulletSpread);
		ShotDirection = FMath::VRandCone(ShotDirection, HalfRad, HalfRad);

		/** End Location of the ray */
		FVector TraceEnd = EyeLocation + (ShotDirection * 10000);

		/** Additional Parameters used for the trace */
		FCollisionQueryParams QueryParams;

		/** Actors for the Trace to Ignore */
		QueryParams.AddIgnoredActor(MyOwner);
		QueryParams.AddIgnoredActor(this);

		/** Trace against a complex collision */
		QueryParams.bTraceComplex = true;

		QueryParams.bReturnPhysicalMaterial = true;

		/** Particle "Target' Parameter */
		FVector TracerEndPoint = TraceEnd;

		EPhysicalSurface SurfaceType = SurfaceType_Default;

		FHitResult Hit;
		/**
		 * LineTraceSingleByChannel -> Traces a ray against the world by a channel and returns 
		 the first blocking hit
		 * Returns true if a blocking hit is found
		 */
		if (GetWorld()->LineTraceSingleByChannel(Hit, EyeLocation, TraceEnd, COLLISION_WEAPON, QueryParams))
		{
			/** Actor that will be damaged */
			AActor* HitActor = Hit.GetActor();

			/** default surface Type */

			GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Yellow, "Hit");

			/** Types of surfaces in the game */
			SurfaceType = UPhysicalMaterial::DetermineSurfaceType(Hit.PhysMaterial.Get());

			/** Damage value*/
			float ActualDamage = BaseDamage;

			if (SurfaceType == SURFACE_FLESHVULNERABLE)
			{
				/** Massive Damage when we hit a SURFACE_FLESHVULNERABLE */
				ActualDamage *= 4.0f;
			}

			/** Hurt the specified actor with the specified impact */
			UGameplayStatics::ApplyPointDamage(HitActor, ActualDamage, ShotDirection, Hit, MyOwner->GetInstigatorController(), MyOwner, DamageType);

			PlayImpactEffects(SurfaceType, Hit.ImpactPoint);

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

		if (Role == ROLE_Authority)
		{
			HitScanTrace.TraceTo = TracerEndPoint;
			HitScanTrace.SurfaceType = SurfaceType;
		}

		/** The last time a shot was fired */
		LastFireTime = GetWorld()->TimeSeconds;
	}
}

/** Called when HitScanTrace is replicated on the client */
void ASWeapon::OnRep_HitScanTrace()
{
	/** Play Cosmetic FX */
	PlayFireEffects(HitScanTrace.TraceTo);
	PlayImpactEffects(HitScanTrace.SurfaceType, HitScanTrace.TraceTo);
}

void ASWeapon::PlayImpactEffects(EPhysicalSurface SurfaceType, FVector ImpactPoint)
{

	UParticleSystem* SelectedEffects = nullptr;

	/** Emit DefaultImpactEffects on SurfaceTypes that's not the body and Emit
	FleshImpactEffect on surfaceType that is the Character */
	switch (SurfaceType)
	{
	case SURFACE_FLESHDEFAULT:
	case SURFACE_FLESHVULNERABLE:
		SelectedEffects = FleshImpactEffect;
		break;
	default:
		SelectedEffects = DefaultImpactEffect;
		break;
	}

	if (SelectedEffects)
	{
		/** Get Muzzle Location */
		FVector MuzzleLocation = MeshComp->GetSocketLocation(MuzzleSocketName);

		FVector ShotDiretion = ImpactPoint - MuzzleLocation;
		ShotDiretion.Normalize();
		/** Spawn Particle effect at the hit location */
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), SelectedEffects, ImpactPoint, ShotDiretion.Rotation());
	}
}

void ASWeapon::ServerFire_Implementation()
{
	Fire();
}

bool ASWeapon::ServerFire_Validate()
{
	return true;
}

/** Call Fire function every "TimeBetweenShots" */
void ASWeapon::StartFire()
{
	/** The Time before the first fire; Must be >= 0 */
	float FirstDelay = FMath::Max(LastFireTime + TimeBetweenShots - GetWorld()->TimeSeconds, 0.0f);

	GetWorldTimerManager().SetTimer(TimerHandle_TimeInBetweenShots, this, &ASWeapon::Fire, TimeBetweenShots, true, FirstDelay);
}

/** Stop Firing */
void ASWeapon::StopFire()
{
	/** Clear all Timer bound to the Fire Function */
	GetWorldTimerManager().ClearTimer(TimerHandle_TimeInBetweenShots);
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

	/**
	 * Get Owner (Cast to Pawn); Get PlayerController
	 * If PlayerController is not null, Play Camera Shake.
	 */
	APawn* MyOwner = Cast<APawn>(GetOwner());
	if (MyOwner)
	{
		APlayerController* PlayerController = Cast<APlayerController>(MyOwner->GetController());

		if (PlayerController)
		{
			PlayerController->ClientPlayCameraShake(FireCameraShake);
		}
	}
}

void ASWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(ASWeapon, HitScanTrace, COND_SkipOwner);
}