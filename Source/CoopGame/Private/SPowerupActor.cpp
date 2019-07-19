// Fill out your copyright notice in the Description page of Project Settings.

#include "SPowerupActor.h"
#include "TimerManager.h"
#include "Net/UnrealNetwork.h"


// Sets default values
ASPowerupActor::ASPowerupActor()
{
	PowerupInterval = 0.0f;

	TotalNoOfTicks = 0;

	bIsPowerupActive = false;

	/** Replicate to client */
	SetReplicates(true);
}

// Called when the game starts or when spawned
void ASPowerupActor::BeginPlay()
{
	Super::BeginPlay();
}

void ASPowerupActor::On_RepPowerupActive()
{
	OnPowerupStateChanged(bIsPowerupActive);
}

void ASPowerupActor::OnTickPowerup()
{
	TicksProcessed++;
	
	onPowerupTicked();

	if (TicksProcessed >= TotalNoOfTicks)
	{
		/** Blueprint immplemented method; Return to normal speed */
		OnExpired();

		bIsPowerupActive = false;

		/** Manually call this function as OnRep functions don't get called on the server */
		On_RepPowerupActive();

		//Delete Timer
		GetWorldTimerManager().ClearTimer(TimerHandle_PowerupTick);
	}
}

void ASPowerupActor::ActivatePowerup(AActor* ActivateFor)
{
	/** Blueprint implemented method; Increase the speed */
	OnActivated(ActivateFor);

	bIsPowerupActive = true;

	/** Manually call this function as OnRep functions don't get called on the server */
	On_RepPowerupActive();

	if (PowerupInterval > 0)
	{
		GetWorldTimerManager().SetTimer(TimerHandle_PowerupTick, this, &ASPowerupActor::OnTickPowerup, PowerupInterval, true);
	}
	else
	{
		OnTickPowerup();
	}
}

void ASPowerupActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASPowerupActor, bIsPowerupActive);
}