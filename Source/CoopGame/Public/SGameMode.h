// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SGameMode.generated.h"

enum class EWaveState : uint8;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnActorKilled, AActor*, VictimActor, AActor*, KillerActor, AController*, KillerController);

UCLASS()
class COOPGAME_API ASGameMode : public AGameModeBase
{
	GENERATED_BODY()

protected:

	int32 NoOfBotsToSpawn;

	int32 WaveCount;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GameMode")
	float TimeBetweenWaves;

	FTimerHandle TimerHandle_BotSpawner;

	FTimerHandle TimerHandle_NextWaveStart;

	/** Hook for BP to spawn a single bot */
	UFUNCTION(BlueprintImplementableEvent, Category = "GameMode")
	void SpawnNewBot();

	// Start spawning bot
	void StartWave();

	void SpawnBotTimerElapsed();

	/** Stop Spawning bot */
	void EndWave();

	// Set timer for next startwave
	void PrepareForNextWave();

	void CheckWaveState();
	
	// Check whether any player is still alive
	void CheckAnyPlayerAlive();

	void GameOver();

	void SetWaveState(EWaveState NewState);

	void RespawnDeadPlayers();

public:

	 ASGameMode();

	virtual void StartPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	UPROPERTY(BlueprintAssignable, Category = "GameMode")
	FOnActorKilled OnActorKilled;
};  
