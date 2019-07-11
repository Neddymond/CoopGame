// Fill out your copyright notice in the Description page of Project Settings.

#include "SHealthComponent.h"
#include "Engine/Engine.h"
#include "Net/UnrealNetwork.h"


// Sets default values for this component's properties
USHealthComponent::USHealthComponent()
{
	/** Default health of the player */
	DefaultHealth = 100; 
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

/** Player's Health when damaged */
void USHealthComponent::HandleTakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
	if (Damage < 0.0f)
	{
		return;
	}

	Health = FMath::Clamp(Health - Damage, 0.0f, DefaultHealth);

	UE_LOG(LogTemp, Log, TEXT("Health Changed: %s"), *FString::SanitizeFloat(Health)); 

	OnHealthChanged.Broadcast(this, Health, Damage, DamageType, InstigatedBy, DamageCauser);
}

void USHealthComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(USHealthComponent, Health);
}

