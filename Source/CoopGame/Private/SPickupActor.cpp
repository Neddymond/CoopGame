// Fill out your copyright notice in the Description page of Project Settings.

#include "SPickupActor.h"
#include "Components/SphereComponent.h"
#include "Components/DecalComponent.h"
#include "GameFramework/Actor.h"
#include "Math/Rotator.h"

// Sets default values
ASPickupActor::ASPickupActor()
{
	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	RootComponent = SphereComponent;
	SphereComponent->SetSphereRadius(75.f);

	DecalComponent = CreateDefaultSubobject<UDecalComponent>(TEXT("DecalComponent"));
	DecalComponent->SetupAttachment(SphereComponent);
	DecalComponent->SetRelativeRotation(FRotator(90, 0.f, 0.f));
	DecalComponent->DecalSize = FVector(64, 75, 75);
}

// Called when the game starts or when spawned
void ASPickupActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASPickupActor::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

}

