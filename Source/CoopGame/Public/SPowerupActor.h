// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SPowerupActor.generated.h"

UCLASS()
class COOPGAME_API ASPowerupActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASPowerupActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/** Time between powerup ticks */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Powerup")
	float PowerupInterval;

	/** The total no of time we apply the powerup effect */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Powerup")
	int32 TotalNoOfTicks;

	/** Total no of ticks applied */
	int32 TicksProcessed;

	FTimerHandle TimerHandle_PowerupTick;

	UFUNCTION()
	void OnTickPowerup();

	void ActivatePowerup();

public:	

	UFUNCTION(BlueprintImplementableEvent, Category = "Powerup")
	void OnActivated();

	UFUNCTION(BlueprintImplementableEvent, Category = "Powerup")
	void OnExpired();

	UFUNCTION(BlueprintImplementableEvent, Category = "Powerup")
	void onPowerupTicked();

};