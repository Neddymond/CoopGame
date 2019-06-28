// Fill out your copyright notice in the Description page of Project Settings.

#include "SWeapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "DrawDebugHelpers.h"

// Sets default values
ASWeapon::ASWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	/**
	 * Create a Skeletal Mesh component
	 * Make MeshComp the RootComponent
	 */
	MeshComp = CreateDefaultSubobject < USkeletalMeshComponent>(TEXT("MeshComp"));
	RootComponent = MeshComp;

}

// Called when the game starts or when spawned
void ASWeapon::BeginPlay()
{
	Super::BeginPlay();
	
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

		/** End Location of the ray */
		FVector TraceEnd = EyeLocation + (EyeRotation.Vector() * 10000);

		/** Additional Parameters used for the trace */
		FCollisionQueryParams QueryParams;

		/** Actors for the Trace to Ignore */
		QueryParams.AddIgnoredActor(MyOwner);
		QueryParams.AddIgnoredActor(this);

		/** Trace against a complex collision */
		QueryParams.bTraceComplex = true;

		FHitResult Hit;
		/**
		 * LineTraceSingleByChannel -> Traces a ray against the world by a channel and returns 
		 the first blocking hit
		 * Returns true if a blocking hit is found
		 */
		if (GetWorld()->LineTraceSingleByChannel(Hit, EyeLocation, TraceEnd, ECC_Visibility, QueryParams))
		{
			/** Process damage */

		}

		DrawDebugLine(GetWorld(), EyeLocation, TraceEnd, FColor::White, false, 1.f, 0, 1.f);
	}
}

// Called every frame
void ASWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

