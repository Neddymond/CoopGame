// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "SGameState.generated.h"


UENUM(BlueprintType)
enum class EWaveState : uint8
{
	waitingToStart,

	WaveInProgress,

	// No longer spawning bots, waiting for player to kill the remaining bots
	waitingToComplete,

	WaveComplete,

	GameOver
};

UCLASS()
class COOPGAME_API ASGameState : public AGameStateBase
{
	GENERATED_BODY()

protected:

	UFUNCTION()
	void OnRep_WaveState(EWaveState OldState);

	UFUNCTION(BlueprintImplementableEvent, Category = "GameState")
		void WaveStatechanged(EWaveState NewState, EWaveState OldState);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_WaveState, Category = "GameState")
		EWaveState WaveState;

	
public:

	void SetWaveState(EWaveState WaveState);
	
	
};
