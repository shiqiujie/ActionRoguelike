// Fill out your copyright notice in the Description page of Project Settings.


#include "SAction.h"
#include "SActionComponent.h"
#include "SAttributeComponent.h"

USAction::USAction()
{
	bIsCostRage = false;
	RageCost = 0.0f;
}
bool USAction::CanStart_Implementation(AActor* Instigator)
{
	USActionComponent* Comp = GetOwningComponent();
	
	//Comp->ActiveGameplayTags.HasAllExact(BlockedTags);精确到最后一位查询
	//HasAny:不准确查询，A.B中依然可以查询到A

	if (Comp->ActiveGameplayTags.HasAny(BlockedTags))
	{
		return false;
	}

	if (bIsCostRage)
	{
		USAttributeComponent* AttributeComp = USAttributeComponent::GetAttributesComp(Instigator);
		bool CostSuccess = AttributeComp->ApplyRageChange(Instigator ,-RageCost);
		if (!CostSuccess)
		{
			FString FailedMsg = FString::Printf(TEXT("Lack of Rage!"));
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FailedMsg);
			return false;
		}
	}
	return true;
}

void USAction::StartAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp, Log, TEXT("Runing: %s"), *GetNameSafe(this));

	USActionComponent* Comp = GetOwningComponent();
	Comp->ActiveGameplayTags.AppendTags(GrantsTags);

	bIsRuning = true;
}

void USAction::StopAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp, Log, TEXT("Stop: %s"), *GetNameSafe(this));

	ensureAlways(bIsRuning);

	USActionComponent* Comp = GetOwningComponent();
	Comp->ActiveGameplayTags.RemoveTags(GrantsTags);

	bIsRuning = false;
}

USActionComponent* USAction::GetOwningComponent() const
{
	return Cast<USActionComponent>(GetOuter());
}



UWorld* USAction::GetWorld() const
{
	USActionComponent* ActionComp = Cast<USActionComponent>(GetOuter());
	if (ActionComp)
	{
		AActor* OwnerActor = ActionComp->GetOwner();

		if (OwnerActor)
		{
			return OwnerActor->GetWorld();
		}
	}

	return nullptr;
}

bool USAction::IsRuning()
{
	return bIsRuning;
}

