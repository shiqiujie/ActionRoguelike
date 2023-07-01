// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameplayTagContainer.h"
#include "SAction.generated.h"

class UWorld;
class USActionComponent;
/**
 * 
 */

 //派生自UObject,添加Blueprintable之后，蓝图里的新建类向导才能显示
UCLASS(Blueprintable)
class ACTIONROGUELIKE_API USAction : public UObject
{
	GENERATED_BODY()

public:

	USAction();

	virtual UWorld* GetWorld() const override;

	bool IsRuning();

	UFUNCTION(BlueprintNativeEvent, Category = "Action")
	bool CanStart(AActor* Instigator);

	//BlueprintNativeEvent:owning override function
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Action")
	void StartAction(AActor* Instigator);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Action")
	void StopAction (AActor* Instigator);
	 
	UPROPERTY(EditDefaultsOnly , Category ="Action")
	FName ActionName;

	UPROPERTY(EditDefaultsOnly, Category = "Action")
	bool bAutoStart;

protected:

	UFUNCTION(BlueprintPure , Category = "Action")
	USActionComponent* GetOwningComponent() const;
	
protected:

	bool bIsRuning;

	UPROPERTY(EditDefaultsOnly, Category = "Tags")
	FGameplayTagContainer GrantsTags;

	UPROPERTY(EditDefaultsOnly, Category = "Tags")
	FGameplayTagContainer BlockedTags;

	UPROPERTY(EditDefaultsOnly, Category = "Rage")
	bool bIsCostRage;

	UPROPERTY(EditDefaultsOnly, Category = "Rage", meta = (EditCondition = bIsCostRage))
	float RageCost;

	
};
