// Fill out your copyright notice in the Description page of Project Settings.


#include "SPlayerState.h"
#include <Net/UnrealNetwork.h>

void ASPlayerState::ChangeCredits(float ChangeValue)
{
	Credits += ChangeValue;

	CreditsChangeDelegate.Broadcast(this, Credits, ChangeValue);
}

void ASPlayerState::OnRep_CreditsChangeds()
{
	CreditsChangeDelegate.Broadcast(this, Credits, 0.0f);
}

void ASPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASPlayerState,Credits);

}