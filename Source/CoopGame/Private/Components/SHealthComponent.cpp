// Fill out your copyright notice in the Description page of Project Settings.

#include "SHealthComponent.h"


// Sets default values for this component's properties
USHealthComponent::USHealthComponent()
{
	/** Defualt health of the player */
	Health = 100; 
}


// Called when the game starts
void USHealthComponent::BeginPlay()
{
	Super::BeginPlay();

}


