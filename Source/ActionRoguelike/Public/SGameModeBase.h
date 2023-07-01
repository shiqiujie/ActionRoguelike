// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "AIModule/Classes/EnvironmentQuery/EnvQueryInstanceBlueprintWrapper.h"
#include "SGameModeBase.generated.h"

class UCurveFloat;
/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API ASGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:

	ASGameModeBase();

	virtual void StartPlay() override;

	virtual void OnActorKilled(AActor* VictimActor, AActor* Killer);


public:
	UFUNCTION(Exec)
	void KillAllAI();


protected:

	UFUNCTION()
	void SpawnBotTimerElapsed();

	UFUNCTION()
	void RespawnPlayerTimerElapsed(AController* Controller);

	UFUNCTION()
	void OnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus);

	UFUNCTION()
	void OnQueryItemCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus);

	UFUNCTION()
	void SpawnItemsTimerElapsed();

protected:

	FTimerHandle SpawnBotTimer;
	FTimerHandle SpawnItemTimer;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	float SpawnTimerInterval;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	bool bEnbaleSpawnBot;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UEnvQuery* SpawnBotQuery;

	UPROPERTY(EditDefaultsOnly, Category = "Item")
	UEnvQuery* SpawnItemsQuery;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UCurveFloat* DifficultySpawnCurve;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	TSubclassOf<AActor> SpawnMinionClass;

	UPROPERTY(EditDefaultsOnly, Category = "Item")
	TSubclassOf<AActor> SpawnHealPotionClass;

	UPROPERTY(EditDefaultsOnly, Category = "Item")
	TSubclassOf<AActor> SpawnCoinClass;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	float GrantCreditsPerMinion = 10.0f;
	
};
