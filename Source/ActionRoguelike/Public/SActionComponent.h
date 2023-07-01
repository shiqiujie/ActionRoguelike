// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"
#include "SActionComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACTIONROGUELIKE_API USActionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USActionComponent();

	UFUNCTION(BlueprintPure, Category = "Attributes")
	static USActionComponent* GetActionComponent(AActor* FromActor);

	UFUNCTION(BlueprintCallable, Category = "Action")
	USAction* GetAction(TSubclassOf<USAction> ActionClass);

	UFUNCTION(BlueprintCallable , Category = "Action")
	void AddAction(AActor* Instigator, TSubclassOf<USAction> ActionClass);

	UFUNCTION(BlueprintCallable , Category = "Action")
	void RemoveAction(USAction* ActionToRemove);

	UFUNCTION(BlueprintCallable, Category = "Action")
	bool StartActionByName(AActor* Instigator,FName ActionName);

	UFUNCTION(BlueprintCallable, Category = "Action")
	bool StopActionByName(AActor* Instigator, FName ActionName);

	UFUNCTION(BlueprintCallable, Category = "Action")
	bool HasGameplayTag(const FGameplayTag& InGameplayTag) const;

protected:

	UFUNCTION(Server, Reliable)
	void ServerStartAction(AActor* Instigator, FName ActionName);

	UFUNCTION(Server, Reliable)
	void ServerStopAction(AActor* Instigator, FName ActionName);

	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:

	//包含FGameplayTagContainer时不能再用Class申明，因为编译器必须知道结构体的大小，所以必须包含头文件
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tags")
	FGameplayTagContainer ActiveGameplayTags;

protected:

	UPROPERTY(EditAnywhere,Category = "Default")
	TArray<TSubclassOf<USAction>> DefaultActions;

    UPROPERTY()
	TArray<USAction*> Actions;
};
