// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SPickupItemBase.h"
#include "SAction.h"
#include "SPickupItem_AbilityGeter.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API ASPickupItem_AbilityGeter : public ASPickupItemBase
{
	GENERATED_BODY()

public:

	void Interact_Implementation(APawn* InstigatorPawn) override;

protected:

	UPROPERTY(EditDefaultsOnly, Category = "Config")
	TSubclassOf<USAction> ActionClass;
};
