// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SPickupItemBase.h"
#include "SCoin.generated.h"

UCLASS()
class ACTIONROGUELIKE_API ASCoin : public ASPickupItemBase
{
    GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASCoin();

public:

	void Interact_Implementation(APawn* InstigatorPawn) override;

protected:

	UPROPERTY(EditDefaultsOnly)
	float CreditsCount;
};
