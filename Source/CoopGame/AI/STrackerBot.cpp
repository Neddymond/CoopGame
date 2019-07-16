// Fill out your copyright notice in the Description page of Project Settings.

#include "STrackerBot.h"
#include "Components/ActorComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "NavigationSystem.h"
#include "NavigationPath.h"
#include "GameFramework/Character.h"
#include "GameFrameWork/Actor.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "SHealthComponent.h"
#include "SCharacter.h"
#include "Sound/SoundCue.h"
#include "Net/UnrealNetwork.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "TimerManager.h"


// Sets default values
ASTrackerBot::ASTrackerBot()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	/** Create a Component called MeshComponent */
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetCanEverAffectNavigation(false);
	
	/** Make MeshComponent the Root component */
	RootComponent = MeshComponent;

	MeshComponent->SetSimulatePhysics(true);

	bUseVelocityChange = false;

	MovementForce = 1000;

	RequiredDistanceToTarget = 100;

	ExplosionRadius = 200;

	ExplosionDamage = 40;

	/** Create a health component */
	HealthComponent = CreateDefaultSubobject<USHealthComponent>(TEXT("HealthComponent"));

	HealthComponent->OnHealthChanged.AddDynamic(this, &ASTrackerBot::HandleTakeDamage);

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SphereComponent->SetSphereRadius(200);
	SphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	SphereComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	SphereComponent->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ASTrackerBot::BeginPlay()
{
	Super::BeginPlay();

	if (Role ==ROLE_Authority)
	{
		NextPathPoint = GetNextPathPoint();
	}

	/** Every Second we update our power level based on nearby bots */
	FTimerHandle TimerHandle_OnCheckPowerLevel;
	GetWorldTimerManager().SetTimer(TimerHandle_OnCheckPowerLevel, this, &ASTrackerBot::OnCheckNearbyBots, 1.0, true);
}


FVector ASTrackerBot::GetNextPathPoint()
{
	/** Hack to get player location */
	ACharacter* PlayerPawn = UGameplayStatics::GetPlayerCharacter(this, 0);

	UNavigationPath* NavPath = UNavigationSystemV1::FindPathToActorSynchronously(this, GetActorLocation(), PlayerPawn);

	if (NavPath->PathPoints.Num() > 0)
	{
		/** return the next point in the path */
		return NavPath->PathPoints[1];
	}

	/** Failed to find a path */
	return GetActorLocation();
}

void ASTrackerBot::HandleTakeDamage(USHealthComponent* OwningHealthComponent, float Health, float HealthDelta, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
	UE_LOG(LogTemp, Log, TEXT("Health %s of %s"), *FString::SanitizeFloat(Health), *GetName());

	/** if MaterialInstance is not set yet, we create and set one */
	if (MaterialInstance == nullptr)
	{
		MaterialInstance = MeshComponent->CreateAndSetMaterialInstanceDynamicFromMaterial(0, MeshComponent->GetMaterial(0));
	}

	if (MaterialInstance)
	{
		MaterialInstance->SetScalarParameterValue("LastTimeDamageTaken", GetWorld()->TimeSeconds);
	}

	/** Explode on hitpoints == 0 */
	if (Health <= 0.f)
	{
		SelfDestruct();
	}
}

void ASTrackerBot::SelfDestruct()
{
	if (bExploded)
	{
		return;
	}
	
	bExploded = true;

	/** Spawn Explosion Effects */
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionEffects, GetActorLocation());

	/** play Explosion Sound */
	UGameplayStatics::PlaySoundAtLocation(this, ExplosionSound, GetActorLocation());

	/** Disable visibility of the MeshComponent */
	MeshComponent->SetVisibility(false, true);

	MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	if (Role == ROLE_Authority)
	{
		TArray<AActor*> IgnoredActors;
		IgnoredActors.Add(this);

		//Increase damage based on the power level
		float ActualDamage = ExplosionDamage + (ExplosionDamage * PowerLevel);

		/** Apply Damage */
		UGameplayStatics::ApplyRadialDamage(this, ActualDamage, GetActorLocation(), ExplosionRadius, nullptr, IgnoredActors, this, GetInstigatorController(), true);

		DrawDebugSphere(GetWorld(), GetActorLocation(), ExplosionRadius, 12, FColor::Red, false, 2.f, 0.f, 1.f);

		/** Delete Actor after 2 seconds */
		SetLifeSpan(2.0f);
	}
}


// Called every frame
void ASTrackerBot::Tick(float DeltaTime)
{

	Super::Tick(DeltaTime);

	if (Role == ROLE_Authority && !bExploded)
	{
		/** find the distance to the Target */
		float DistanceToTarget = (GetActorLocation() - NextPathPoint).Size();

		if (DistanceToTarget <= RequiredDistanceToTarget)
		{
			NextPathPoint = GetNextPathPoint();

			DrawDebugString(GetWorld(), GetActorLocation(), "Target Reached");
		}
		else
		{
			/** Keep moving towards Next direction */
			FVector ForceDirection = NextPathPoint - GetActorLocation();
			ForceDirection.Normalize();

			ForceDirection *= MovementForce;

			MeshComponent->AddForce(ForceDirection, NAME_None, bUseVelocityChange);

			DrawDebugDirectionalArrow(GetWorld(), GetActorLocation(), GetActorLocation() + ForceDirection, 32, FColor::Yellow, false, 0.0f, 0, 1);
		}
	}
}

void ASTrackerBot::DamageSelf()
{
	UGameplayStatics::ApplyDamage(this, 20, GetInstigatorController(), this, nullptr);
}

void ASTrackerBot::NotifyActorBeginOverlap(AActor* OtherActor)
{
	if (!bStartedExplosion && !bExploded)
	{
		ASCharacter* PlayerPawn = Cast<ASCharacter>(OtherActor);

		if (PlayerPawn)
		{
			if (Role == ROLE_Authority)
			{
				/** We overlapped with a player; Start Self destruction Sequence */
				GetWorldTimerManager().SetTimer(TimerHandle_SelfDamage, this, &ASTrackerBot::DamageSelf, 0.5f, true, 0.f);
			}
			bStartedExplosion = true;

			UGameplayStatics::SpawnSoundAttached(SelfDestructSound, RootComponent);
		}
	}
}

void ASTrackerBot::OnCheckNearbyBots()
{
	/** distance to check for nearby bots */
	const float Radius = 600;

	/** Only find Pawns (e.g Players and AI bots) */
	FCollisionObjectQueryParams QueryParams;
	QueryParams.AddObjectTypesToQuery(ECC_PhysicsBody);
	QueryParams.AddObjectTypesToQuery(ECC_Pawn);

	/** Create Temporary collision shape for overlaps */
	FCollisionShape CollShape;
	CollShape.SetSphere(Radius);

	TArray<FOverlapResult> Overlaps;
	GetWorld()->OverlapMultiByObjectType(Overlaps, GetActorLocation(), FQuat::Identity, QueryParams, CollShape);

	int32 NoOfBots = 0;

	/** Loop over the Results using a "Range-based for loop" */
	for (FOverlapResult Result : Overlaps)
	{
		//check if we overlapped with another TrackerBot (ignoring players and self)
		ASTrackerBot* Bot = Cast<ASTrackerBot>(Result.GetActor());
		
		/** Ignore this instance */
		if (Bot && Bot != this)
		{
			NoOfBots++;
		}
	}

	const int32 MaxPowerLevel = 4;

	//clamp between min=0 and max=4
	PowerLevel = FMath::Clamp(NoOfBots, 0, MaxPowerLevel);

	//update material instance
	if (MaterialInstance == nullptr)
	{
		MaterialInstance = MeshComponent->CreateAndSetMaterialInstanceDynamicFromMaterial(0, MeshComponent->GetMaterial(0));
	}

	if (MaterialInstance)
	{
		float Alpha = PowerLevel / (float)MaxPowerLevel;

		MaterialInstance->SetScalarParameterValue("PowerLevelAlpha", Alpha);
	}
}
