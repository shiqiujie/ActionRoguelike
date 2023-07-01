// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SAttributeComponent.generated.h"

// DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnHealthChanged, AActor*, InstigatorActor, USAttributeComponent*, OwningComp, float, NewHealth, float, Delta);
// DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnRageChanged, AActor*, InstigatorActor, USAttributeComponent*, OwningComp, float, NewRage, float, Delta);


DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnAttributeChanged, AActor*, InstigatorActor, USAttributeComponent*, OwningComp, float, NewValue, float, Delta);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACTIONROGUELIKE_API USAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USAttributeComponent();

	virtual void BeginPlay() override;

public:	

	UFUNCTION(BlueprintPure,Category ="Attributes")
	static USAttributeComponent* GetAttributesComp(AActor* FromActor);

	UFUNCTION(BlueprintPure , Category = "Attributes" , meta = (DisplayName = IsActorAlive))
	static bool IsActorAlive(AActor* Actor);

	UFUNCTION(BlueprintCallable)
	bool ApplyHealthChange(AActor* InstigatorActor, float Delta);

	UFUNCTION(BlueprintCallable)
	bool ApplyRageChange(AActor* InstigatorActor, float Delta);

	UFUNCTION(BlueprintPure)
    bool IsAlive();

	bool Kill(AActor* InstigatorActor);

	float GetMaxHealth();

	bool IsFullHealth();

	bool IsLowHealth();

protected:

	UFUNCTION(NetMulticast, Reliable)//@FIXME:抽离必须要reliable的内容
	void MulticastHealthChanged(AActor* InstigatorActor, float NewHealth, float Delta);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRageChanged(AActor* InstigatorActor, float NewRage, float Delta);

public:

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChanged OnHealthChanged;

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChanged OnRageChanged;

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Replicated, Category = "Attributes")
	float Health;

	//EditDefaultsOnly，蓝图的赋值重写在constructor之后执行
	UPROPERTY(EditDefaultsOnly, Replicated, BlueprintReadOnly, Category = "Attributes")
    float MaxHealth;

	UPROPERTY(BlueprintReadOnly, Replicated, Category = "Attributes")
	float Rage;

	UPROPERTY(EditDefaultsOnly, Replicated, BlueprintReadOnly, Category = "Attributes")
	float MaxRage;
		
};
