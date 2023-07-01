// Fill out your copyright notice in the Description page of Project Settings.


#include "SAttributeComponent.h"
#include "Math/UnrealMathUtility.h"
#include "SGameModeBase.h"
#include "SCharacter.h"
#include <Net/UnrealNetwork.h>

static TAutoConsoleVariable<float> CVarDamageMultiplier(TEXT("su.DamageMultiplier"), 1.0f, TEXT("DamageMultiplier rate"), ECVF_Cheat);

static TAutoConsoleVariable<bool> CVarGodMode(TEXT("su.SetGodMode"), false, TEXT("0:Disable ; 1:Enable"), ECVF_Cheat);

// Sets default values for this component's properties
USAttributeComponent::USAttributeComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	Health = 100.0f;
	MaxHealth = 100.0f;
	Rage = 0.0f;
	MaxRage = 100.0f;

	SetIsReplicatedByDefault(true);
	// ...
}

void USAttributeComponent::BeginPlay()
{
	Super::BeginPlay();

}

USAttributeComponent* USAttributeComponent::GetAttributesComp(AActor* FromActor)
{
	if (FromActor)
	{
		return Cast<USAttributeComponent>(FromActor->GetComponentByClass(USAttributeComponent::StaticClass()));
	}

	return nullptr;
}

bool USAttributeComponent::IsActorAlive(AActor* Actor)
{
	USAttributeComponent* AttrComp = GetAttributesComp(Actor);
    if (AttrComp)
    {
		return AttrComp->IsAlive();
    }

	return false;
}

bool USAttributeComponent::ApplyHealthChange(AActor* InstigatorActor,float Delta)
{
	if (CVarGodMode.GetValueOnGameThread())
	{
		ASCharacter* Player = GetOwner<ASCharacter>();
		if (Player)
		{
			return false;
		}
	}
	
	if (Delta < 0)
	{
		float DamageMultiplier = CVarDamageMultiplier.GetValueOnGameThread();
		
		Delta *= DamageMultiplier;
	}
	float LastHealth = Health;
	Health = FMath::Clamp(Health + Delta, 0.0f, MaxHealth);

	float ActualDelta = Health - LastHealth;

	//Is Server?
	if (GetOwner()->HasAuthority())
	{
		//OnHealthChanged.Broadcast(InstigatorActor, this, Health, ActualDelta);
		//如果在客户端上运行Multicast函数，只会在本地运行
		if (ActualDelta != 0.0f)
		{
			MulticastHealthChanged(InstigatorActor, Health, ActualDelta);
		}
		
		//Died
		if (ActualDelta < 0 && Health <= 0.0f)
		{
			ASGameModeBase* GM = GetWorld()->GetAuthGameMode<ASGameModeBase>();
			if (GM)
			{
				GM->OnActorKilled(GetOwner(), InstigatorActor);
			}
		}
	}
	
	return ActualDelta != 0;
}

bool USAttributeComponent::ApplyRageChange(AActor* InstigatorActor, float Delta)
{
    if (GetOwner()->HasAuthority())
    {
		if (Delta > 0.0f)
		{
			float LastRage = Rage;
			Rage = FMath::Clamp(Rage + Delta, 0, MaxRage);

			float ActualRange = Rage - LastRage;
			MulticastRageChanged(InstigatorActor, Rage, Delta);

			return true;
		}
		else if (Delta < 0.0f && -Delta <= Rage)
		{
			Rage += Delta;
			MulticastRageChanged(InstigatorActor, Rage, Delta);

			return true;
		}
    }
		
	return false;
}

bool USAttributeComponent::IsAlive()
{
	return Health > 0.0f;
}

bool USAttributeComponent::Kill(AActor* InstigatorActor)
{
	return ApplyHealthChange(InstigatorActor, -MaxHealth);
}

float USAttributeComponent::GetMaxHealth()
{
	return MaxHealth;
}

bool USAttributeComponent::IsFullHealth()
{
	return (Health == MaxHealth);
}

bool USAttributeComponent::IsLowHealth()
{
	return (Health / MaxHealth) < 0.2;
}

void USAttributeComponent::MulticastHealthChanged_Implementation(AActor* InstigatorActor, float NewHealth, float Delta)
{
	OnHealthChanged.Broadcast(InstigatorActor, this, NewHealth, Delta);
}

void USAttributeComponent::MulticastRageChanged_Implementation(AActor* InstigatorActor, float NewRage, float Delta)
{
	OnRageChanged.Broadcast(InstigatorActor, this, NewRage, Delta);
}

void USAttributeComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(USAttributeComponent, Health);
	DOREPLIFETIME(USAttributeComponent, MaxHealth);
	DOREPLIFETIME(USAttributeComponent, Rage);
	DOREPLIFETIME(USAttributeComponent, MaxRage);
	//Condition Replication
	//DOREPLIFETIME_CONDITION(USAttributeComponent, MaxHealth, COND_InitialOnly);
}