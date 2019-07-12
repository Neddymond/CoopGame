// Fill out your copyright notice in the Description page of Project Settings.

#include "STrackerBot.h"
#include "Components/ActorComponent.h"
#include "Components/StaticMeshComponent.h"
#include "NavigationSystem.h"
#include "NavigationPath.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"




// Sets default values
ASTrackerBot::ASTrackerBot()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	/** Create a Component called MeshComponent */
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetCanEverAffectNavigation(false);
	
	/** Make MeshComponent the Root component */
	RootComponent = MeshComponent;
}

// Called when the game starts or when spawned
void ASTrackerBot::BeginPlay()
{
	Super::BeginPlay();
	
}


FVector ASTrackerBot::GetNextPathPoint()
{
	/** Hack to get player location */
	ACharacter* PlayerPawn = UGameplayStatics::GetPlayerCharacter(this, 0);

	UNavigationPath* NavPath = UNavigationSystemV1::FindPathToActorSynchronously(this, GetActorLocation(), PlayerPawn);

	if (NavPath->PathPoints.Num() > 0)
	{
		/** return the next point in the path */
		return NavPath->PathPoints[1];
	}

	/** Failed to find a path */
	return GetActorLocation();
}


// Called every frame
void ASTrackerBot::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

