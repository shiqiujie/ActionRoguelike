// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SAction.h"
#include "SActionEffect.generated.h"

UCLASS()
class ACTIONROGUELIKE_API USActionEffect : public USAction
{
	GENERATED_BODY()
	
public:

	USActionEffect();

	virtual void StartAction_Implementation(AActor* Instigator) override;
	virtual void StopAction_Implementation(AActor* Instigator) override;

protected:

	UFUNCTION(BlueprintNativeEvent , Category = "Effect")
	void ExecutePeriodicEffect(AActor* Instigator);

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly,Category = "Effect")
	float Duration;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Effect")
	float Period;

	FTimerHandle DurationHandle;
	FTimerHandle PeriodHandle;


};
