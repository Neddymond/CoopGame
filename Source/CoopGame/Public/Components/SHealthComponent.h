// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SHealthComponent.generated.h"


UCLASS( ClassGroup=(COOP), meta=(BlueprintSpawnableComponent) )
class COOPGAME_API USHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USHealthComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	/** Actor's Health */
	UPROPERTY(BlueprintReadOnly, Category = "HealthComponents")
	float Health;

	/** Actor's default Health */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HealthComponents")
	float DefaultHealth;
	
	UFUNCTION() 
	void HandleTakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

public:	
		
	
};
