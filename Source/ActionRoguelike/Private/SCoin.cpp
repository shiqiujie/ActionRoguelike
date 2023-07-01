// Fill out your copyright notice in the Description page of Project Settings.


#include "SCoin.h"
#include "SAttributeComponent.h"
#include "SPlayerState.h"

// Sets default values
ASCoin::ASCoin()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CreditsCount = 20.0f;

}

void ASCoin::Interact_Implementation(APawn* InstigatorPawn)
{
	Super::Interact_Implementation(InstigatorPawn);

	ASPlayerState* PS = Cast<ASPlayerState>(InstigatorPawn->GetPlayerState());

	if (PS)
	{
		PS->ChangeCredits(CreditsCount);
		OnInteractSuccess();
	}

}
