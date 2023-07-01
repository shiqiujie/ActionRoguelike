// Fill out your copyright notice in the Description page of Project Settings.


#include "SGameModeBase.h"
#include "AIModule/Classes/EnvironmentQuery/EnvQueryManager.h"
#include "EngineUtils.h"
#include "AI/SAICharacter.h"
#include "SAttributeComponent.h"
#include "SCharacter.h"
#include "GameFramework/Actor.h"
#include "SPlayerState.h"

static TAutoConsoleVariable<bool> CVarSpawnBots(TEXT("su.SpawnBots"), true, TEXT("Enable spawning of bots via timer."), ECVF_Cheat);

ASGameModeBase::ASGameModeBase()
{
	SpawnTimerInterval = 2.0f;
	bEnbaleSpawnBot = true;
}

void ASGameModeBase::StartPlay()
{
	Super::StartPlay();

	GetWorldTimerManager().SetTimer(SpawnBotTimer,this,&ASGameModeBase::SpawnBotTimerElapsed,SpawnTimerInterval,true);

	GetWorldTimerManager().SetTimer(SpawnItemTimer, this, &ASGameModeBase::SpawnItemsTimerElapsed, 2.0f, false);
}

void ASGameModeBase::OnActorKilled(AActor* VictimActor, AActor* Killer)
{
	ASCharacter* Player = Cast<ASCharacter>(VictimActor);
	if (Player)
	{
		FTimerHandle TimerHandle_RespawnDelay;

        //FTimerDelegate: Compare with &ASGameModeBase::FunctionName, it can pass params.
		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, "RespawnPlayerTimerElapsed", Player->GetController());

		GetWorldTimerManager().SetTimer(TimerHandle_RespawnDelay,Delegate,2.0f,false);
	}

	APawn* KillerPawn = Cast<APawn>(Killer);
	if (VictimActor && KillerPawn)
	{
		ASPlayerState* PS = Cast<ASPlayerState>(KillerPawn->GetPlayerState());
		if (PS)
		{
			PS->ChangeCredits(GrantCreditsPerMinion);
		}
	}
}

void ASGameModeBase::KillAllAI()
{
	for (TActorIterator<ASAICharacter> It(GetWorld()); It; ++It)
	{
		ASAICharacter* Bot = *It;

		USAttributeComponent* AttrComp = USAttributeComponent::GetAttributesComp(Bot);

		if (AttrComp && AttrComp->IsAlive())
		{
			AttrComp->Kill(this);
		}
	}
}

void ASGameModeBase::SpawnBotTimerElapsed()
{
	if (!CVarSpawnBots.GetValueOnGameThread() || !bEnbaleSpawnBot)
	{
		UE_LOG(LogTemp, Warning, TEXT("Disable Spawn Bot!"));
		return;
	}
	
	int32 AliveBotCount = 0;

	for (TActorIterator<ASAICharacter> It(GetWorld()); It; ++It)
	{
		ASAICharacter* Bot = *It;

		USAttributeComponent* AttrComp = USAttributeComponent::GetAttributesComp(Bot);

		if (AttrComp && AttrComp->IsAlive())
		{
			AliveBotCount++;
		}
	}

	float MaxBotCount = 10.0f;

	if (DifficultySpawnCurve)
	{
		MaxBotCount = DifficultySpawnCurve->GetFloatValue(GetWorld()->GetTimeSeconds());
	}

	if (AliveBotCount >= MaxBotCount)
	{
		return;
	}

	UEnvQueryInstanceBlueprintWrapper* QueryInstance = UEnvQueryManager::RunEQSQuery(this, SpawnBotQuery, this, EEnvQueryRunMode::RandomBest5Pct, nullptr);
	if (ensure(QueryInstance))
	{
		QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this,&ASGameModeBase::OnQueryCompleted);
	}
}

void ASGameModeBase::RespawnPlayerTimerElapsed(AController* Controller)
{
	if (ensure(Controller))
	{
		Controller->UnPossess();

		RestartPlayer(Controller);
	}
}

void ASGameModeBase::OnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus)
{
	if (QueryStatus != EEnvQueryStatus::Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("Spanw Bot EQS Query failed!"));
		return;
	}

	TArray<FVector> Locations =QueryInstance->GetResultsAsLocations();

	if (Locations.IsValidIndex(0))
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>(SpawnMinionClass, Locations[0], FRotator::ZeroRotator,SpawnParams);

		if (!SpawnedActor)
		{
			UE_LOG(LogTemp, Warning, TEXT("Spawn Bot Failed"));
		}
	}
}

void ASGameModeBase::OnQueryItemCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus)
{
	if (QueryStatus != EEnvQueryStatus::Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("Spanw Item EQS Query failed!"));
		return;
	}

	TArray<FVector> Locations = QueryInstance->GetResultsAsLocations();

	int32 RandIndex = FMath::RandRange(0, Locations.Num()-1);
	if (Locations.IsValidIndex(RandIndex))
	{
		Locations.RemoveAt(RandIndex);
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>(SpawnHealPotionClass, Locations[RandIndex], FRotator::ZeroRotator, SpawnParams);

		if (!SpawnedActor)
		{
			UE_LOG(LogTemp, Warning, TEXT("Spawn Potion Failed"));
		}
	}

	RandIndex = FMath::RandRange(0, Locations.Num() - 1);

	if (Locations.IsValidIndex(RandIndex))
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>(SpawnCoinClass, Locations[RandIndex], FRotator::ZeroRotator, SpawnParams);

		if (!SpawnedActor)
		{
			UE_LOG(LogTemp, Warning, TEXT("Spawn Coin Failed"));
		}
	}
}

void ASGameModeBase::SpawnItemsTimerElapsed()
{
	UEnvQueryInstanceBlueprintWrapper* QueryInstance = UEnvQueryManager::RunEQSQuery(this, SpawnItemsQuery, this, EEnvQueryRunMode::AllMatching, nullptr);
	if (ensure(QueryInstance))
	{
		QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this, &ASGameModeBase::OnQueryItemCompleted);
	}
}
