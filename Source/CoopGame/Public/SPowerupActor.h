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

	/** Keeps state of the powerup */
	UPROPERTY(ReplicatedUsing = On_RepPowerupActive)
		bool bIsPowerupActive;

	UFUNCTION()
	void On_RepPowerupActive();

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

	UFUNCTION(BlueprintImplementableEvent, Category = "Powerup")
		void OnPowerupStateChanged(bool	bNewIsActive);

public:	

	void ActivatePowerup(AActor* ActivateFor);

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;

	UFUNCTION(BlueprintImplementableEvent, Category = "Powerup")
	void OnActivated(AActor* ActivateFor);

	UFUNCTION(BlueprintImplementableEvent, Category = "Powerup")
	void OnExpired();

	UFUNCTION(BlueprintImplementableEvent, Category = "Powerup")
	void onPowerupTicked();

};