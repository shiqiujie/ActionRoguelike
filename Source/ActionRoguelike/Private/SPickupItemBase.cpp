// Fill out your copyright notice in the Description page of Project Settings.


#include "SPickupItemBase.h"

// Sets default values
ASPickupItemBase::ASPickupItemBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASPickupItemBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASPickupItemBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASPickupItemBase::Interact_Implementation(APawn* InstigatorPawn)
{
	
}

void ASPickupItemBase::OnInteractSuccess()
{
	MulticastHiddenActor(true);
	GetWorldTimerManager().SetTimer(CDTimerHandle, this, &ASPickupItemBase::CoolDown_Elapsed, CoolDown);
}

void ASPickupItemBase::CoolDown_Elapsed()
{
	MulticastHiddenActor(false);
}

void ASPickupItemBase::MulticastHiddenActor_Implementation(bool bIsHidden)
{
	SetActorEnableCollision(!bIsHidden);
	SetActorHiddenInGame(bIsHidden);
}

