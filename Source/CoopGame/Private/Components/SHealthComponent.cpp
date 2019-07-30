// Fill out your copyright notice in the Description page of Project Settings.

#include "SHealthComponent.h"
#include "Engine/Engine.h"
#include "Net/UnrealNetwork.h"
#include "SGameMode.h"
#include "Engine/World.h"



// Sets default values for this component's properties
USHealthComponent::USHealthComponent()
{
	/** Default health of the player */
	DefaultHealth = 100;
	bIsDead = false;

	TeamNum = 255;
}


// Called when the game starts
void USHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	/** Only hook if we are on server */
	if (GetOwnerRole() == ROLE_Authority)
	{
		/** Get the Actor of the component */
		AActor* MyOwner = GetOwner();

		if (MyOwner)
		{
			/** When the Actor is damaged in any way */
			MyOwner->OnTakeAnyDamage.AddDynamic(this, &USHealthComponent::HandleTakeAnyDamage);
		}
	}

	Health = DefaultHealth;
}

void USHealthComponent::OnRep_Health(float OldHealth)
{
	float Damage = Health - OldHealth;

	OnHealthChanged.Broadcast(this, Health, Damage, nullptr, nullptr, nullptr);
}

/** Player's Health when damaged */
void USHealthComponent::HandleTakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
	if (Damage <= 0.0f || bIsDead)
	{
		return;
	}

	if (DamageCauser != DamagedActor && IsFriendly(DamagedActor, DamageCauser))
	{
		return;
	}

	Health = FMath::Clamp(Health - Damage, 0.0f, DefaultHealth);

	UE_LOG(LogTemp, Log, TEXT("Health Changed: %s"), *FString::SanitizeFloat(Health)); 

	bIsDead = Health <= 0.0f;

	OnHealthChanged.Broadcast(this, Health, Damage, DamageType, InstigatedBy, DamageCauser);

	if (bIsDead)
	{
		// return current GameMode instance during gamePlay on the server
		ASGameMode* gameMode = Cast<ASGameMode>(GetWorld()->GetAuthGameMode());

		if (gameMode)
		{
			gameMode->OnActorKilled.Broadcast(GetOwner(), DamageCauser, InstigatedBy);
		}
	}
}

void USHealthComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(USHealthComponent, Health);
}

/** Boost player Health */
void USHealthComponent::Heal(float HealAmount)
{
	if (HealAmount <= 0.0f || Health <= 0.0f)
	{
		return;
	}

	Health = FMath::Clamp(Health + HealAmount, 0.0f, DefaultHealth);
	UE_LOG(LogTemp, Log, TEXT("Health Changed: %s, (+%s)"), *FString::SanitizeFloat(Health), *FString::SanitizeFloat(HealAmount));

	OnHealthChanged.Broadcast(this, Health, -HealAmount, nullptr, nullptr, nullptr);
}

float USHealthComponent::GetHealth() const
{
	return Health;
}

bool USHealthComponent::IsFriendly(AActor* ActorA, AActor* ActorB)
{
	if (ActorA == nullptr || ActorB == nullptr)
	{
		// Assume friendly
		return true;
	}

	USHealthComponent* HealthCompA = Cast<USHealthComponent>(ActorA->GetComponentByClass(USHealthComponent::StaticClass()));
	USHealthComponent* HealthCompB = Cast<USHealthComponent>(ActorB->GetComponentByClass(USHealthComponent::StaticClass()));

	if (HealthCompA	== nullptr || HealthCompB == nullptr)
	{
		// Assume friendly if an Actor doesn't have a health component
		return true;
	}

	return HealthCompA->TeamNum == HealthCompB->TeamNum;
}

