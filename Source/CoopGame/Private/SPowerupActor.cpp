// Fill out your copyright notice in the Description page of Project Settings.

#include "SPowerupActor.h"
#include "TimerManager.h"


// Sets default values
ASPowerupActor::ASPowerupActor()
{
	PowerupInterval = 0.0f;

	TotalNoOfTicks = 0;
}

// Called when the game starts or when spawned
void ASPowerupActor::BeginPlay()
{
	Super::BeginPlay();
}

void ASPowerupActor::OnTickPowerup()
{
	TicksProcessed++;
	
	if (TicksProcessed >= TotalNoOfTicks)
	{
		OnExpired();

		//Delete Timer
		GetWorldTimerManager().ClearTimer(TimerHandle_PowerupTick);
	}
}

void ASPowerupActor::ActivatePowerup()
{
	if (PowerupInterval > 0)
	{
		GetWorldTimerManager().SetTimer(TimerHandle_PowerupTick, this, &ASPowerupActor::OnTickPowerup, PowerupInterval, true, 0.0f);
	}
	else
	{
		OnTickPowerup();
	}
}
