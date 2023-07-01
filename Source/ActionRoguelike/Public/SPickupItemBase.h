// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SGameplayInterface.h"
#include "SInteractItemBase.h"
#include "SPickupItemBase.generated.h"


UCLASS()
class ACTIONROGUELIKE_API ASPickupItemBase : public ASInteractItemBase
{
    GENERATED_BODY()
	
public:	
	// Sets default values for this actor's propesties
	ASPickupItemBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void OnInteractSuccess();
	virtual void CoolDown_Elapsed();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastHiddenActor(bool bIsHidden);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

    void Interact_Implementation(APawn* InstigatorPawn) override;

protected:

	UPROPERTY(EditDefaultsOnly)
	float CoolDown = 10.f;

private:

	FTimerHandle CDTimerHandle;
};
