// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "SPlayerState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnCreditsChanged, APlayerState*, PS, float, NewCredits, float, Delta);


UCLASS()
class ACTIONROGUELIKE_API ASPlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable , Category = "State")
	void ChangeCredits(float ChangeValue);

protected:
    UFUNCTION()
	void OnRep_CreditsChangeds();

protected:

	UPROPERTY(BlueprintAssignable)
	FOnCreditsChanged CreditsChangeDelegate;

	UPROPERTY(ReplicatedUsing = "OnRep_CreditsChangeds", VisibleAnywhere, BlueprintReadOnly, Category = "State")
	float Credits = 0.0f;

    
};
