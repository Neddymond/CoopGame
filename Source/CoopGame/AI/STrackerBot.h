// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "STrackerBot.generated.h"

UCLASS()
class COOPGAME_API ASTrackerBot : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ASTrackerBot();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	FVector GetNextPathPoint();

	 UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent*  MeshComponent;

	 /** Next point in Navigation path */
	 FVector NextPathPoint;

	 UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "TrackerBot")
	 bool bUseVelocityChange;

	 UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "TrackerBot")
	 float MovementForce;

	 UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "TrackerBot")
	 float  RequiredDistanceToTarget;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


};
