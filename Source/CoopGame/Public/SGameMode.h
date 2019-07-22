// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SGameMode.generated.h"

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

public:

	 ASGameMode();

	virtual void StartPlay() override;
};
