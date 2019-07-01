// Fill out your copyright notice in the Description page of Project Settings.

#include "SProjectileWeapon.h"
#include "Engine/Engine.h"
#include "Components/SkeletalMeshComponent.h"


void ASProjectileWeapon::Fire()
{
	AActor* MyOwner = GetOwner();
	if (MyOwner && ProjectileClass)
	{
		FVector EyeLocation;
		FRotator EyeRotation;

		/** Returns the point of view of the actor */
		MyOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);

		FVector MuzzleLocation = MeshComp->GetSocketLocation(MuzzleSocketName);

		FActorSpawnParameters SpawnParameters;

		/** Resolve collision at the spawn point */
		SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		/** Spawn a Projectile */
		GetWorld()->SpawnActor<AActor>(ProjectileClass, MuzzleLocation, EyeRotation, SpawnParameters);
	}
}
