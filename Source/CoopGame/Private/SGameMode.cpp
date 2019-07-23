// Fill out your copyright notice in the Description page of Project Settings.

#include "SGameMode.h"
#include "TimerManager.h"
#include "Engine/world.h"
#include "SHealthComponent.h"
#include "GameframeWork/Pawn.h"
#include "SGameState.h"


 ASGameMode::ASGameMode()
 {
	 TimeBetweenWaves = 2.0f;

	 PrimaryActorTick.bCanEverTick = true;
	 PrimaryActorTick.TickInterval = 1.0f;

	 // Default Gamestate class associated with this GameMode
	 GameStateClass = ASGameState::StaticClass();
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

	 CheckAnyPlayerAlive();
 }

void ASGameMode::StartWave()
{
	WaveCount++;

	NoOfBotsToSpawn = 2 * WaveCount;

	GetWorldTimerManager().SetTimer(TimerHandle_BotSpawner, this, &ASGameMode::SpawnBotTimerElapsed, 1.0f, true, 0.0f);

	SetWaveState(EWaveState::WaveInProgress);
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

	SetWaveState(EWaveState::waitingToComplete);
}

void ASGameMode::PrepareForNextWave()
{
	GetWorldTimerManager().SetTimer(TimerHandle_NextWaveStart, this, &ASGameMode::StartWave, TimeBetweenWaves, false);

	SetWaveState(EWaveState::waitingToStart);
}

void ASGameMode::CheckWaveState()
{
	// check whether we are still preparing for next wave
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
		SetWaveState(EWaveState::WaveComplete);

		PrepareForNextWave();
	}

}

// Check if any player is still alive in the world
void ASGameMode::CheckAnyPlayerAlive()
{
	// loop through the players in the world
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		APlayerController* PlayerController = It->Get();

		if (PlayerController && PlayerController->GetPawn())
		{
			APawn* MyPawn = PlayerController->GetPawn();

			// Get Players HealthComponent
			USHealthComponent* HealthComponent = Cast<USHealthComponent>(MyPawn->GetComponentByClass(USHealthComponent::StaticClass()));

			if (ensure(HealthComponent) && HealthComponent->GetHealth())
			{
				// Player's Alive
				return;
			}
		}
	}

	// No player alive, End Game
	GameOver();
}

void ASGameMode::GameOver()
{
	EndWave();

	UE_LOG(LogTemp, Log, TEXT("All Player's dead, so Game is over"));

	SetWaveState(EWaveState::GameOver);
}

void ASGameMode::SetWaveState(EWaveState NewState)
{
	ASGameState* GameState = GetGameState<ASGameState>();

	if (ensureAlways(GameState))
	{
		GameState->SetWaveState(NewState);
	}
}
