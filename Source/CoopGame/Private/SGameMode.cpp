// Fill out your copyright notice in the Description page of Project Settings.

#include "SGameMode.h"
#include "TimerManager.h"
#include "Engine/world.h"
#include "SHealthComponent.h"
#include "GameframeWork/Pawn.h"


 ASGameMode::ASGameMode()
 {
	 TimeBetweenWaves = 2.0f;

	 PrimaryActorTick.bCanEverTick = true;
	 PrimaryActorTick.TickInterval = 1.0f;
 }

 void ASGameMode::StartPlay()
 {
	 Super::StartPlay();

	 PrepareForNextWave();
 }

 void ASGameMode::Tick(float DeltaSeconds)
 {
	 Super::Tick(DeltaSeconds);

	 CheckWaveState();
 }

void ASGameMode::StartWave()
{
	WaveCount++;

	NoOfBotsToSpawn = 2 * WaveCount;

	GetWorldTimerManager().SetTimer(TimerHandle_BotSpawner, this, &ASGameMode::SpawnBotTimerElapsed, 1.0f, true, 0.0f);
}

void ASGameMode::SpawnBotTimerElapsed()
{
	SpawnNewBot();

	NoOfBotsToSpawn--;

	if (NoOfBotsToSpawn <= 0)
	{
		EndWave();
	}
}

void ASGameMode::EndWave()
{
	GetWorldTimerManager().ClearTimer(TimerHandle_BotSpawner);
}

void ASGameMode::PrepareForNextWave()
{
	GetWorldTimerManager().SetTimer(TimerHandle_NextWaveStart, this, &ASGameMode::StartWave, TimeBetweenWaves, false);
}

void ASGameMode::CheckWaveState()
{
	bool bPreparingForWave = GetWorldTimerManager().IsTimerActive(TimerHandle_NextWaveStart);

	if (NoOfBotsToSpawn > 0 || bPreparingForWave)
	{
		return;
	}


	bool bIsAnyBotAlive = false;

	/** loop through all the Pawns in the world */
	for (FConstPawnIterator It = GetWorld()->GetPawnIterator(); It; ++It)
	{
		APawn* TestPawn = It->Get();

		if (TestPawn == nullptr || TestPawn->IsPlayerControlled())
		{
			continue;
		}

		USHealthComponent* HealthCompnent = Cast<USHealthComponent>(TestPawn->GetComponentByClass(USHealthComponent::StaticClass()));

		if (HealthCompnent && HealthCompnent->GetHealth() > 0.0f)
		{
			bIsAnyBotAlive = true;
			break;
		}
	}

	if (!bIsAnyBotAlive)
	{
		PrepareForNextWave();
	}

}
