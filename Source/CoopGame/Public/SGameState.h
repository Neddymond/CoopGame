// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "SGameState.generated.h"


UENUM(BlueprintType)
enum class EWaveStart : uint8
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
	void OnRep_WaveState(EWaveStart OldState);

	UFUNCTION(BlueprintImplementableEvent, Category = "GameState")
		void WaveStatechanged(EWaveStart NewState, EWaveStart OldState);

	
public:

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_WaveState, Category = "GameState")
	EWaveStart WaveState;
	
	
};
