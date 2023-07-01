// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SAction.h"
#include "SAction_ProjectileAttack.generated.h"


class ASCharacter;
/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API USAction_ProjectileAttack : public USAction
{
	GENERATED_BODY()

	USAction_ProjectileAttack();
public:

	virtual void StartAction_Implementation(AActor* Instigator) override;

protected:

	UFUNCTION()
	void DelayAttack_Elapsed(ASCharacter* InstigatorCharacter);

protected:

	UPROPERTY(EditDefaultsOnly, Category = "Attack")
	TSubclassOf<AActor> ProjectileClass;

	UPROPERTY(EditAnywhere, Category = "Attack")
	UAnimMontage* AttackMTG;

	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	UParticleSystem* MuzzleVFX;

	UPROPERTY(VisibleAnywhere, Category = "Effects")
	FName HandSocketName;

	UPROPERTY(VisibleAnywhere, Category = "Effects")
	float AttackDelayTime = 0.2f;

};
