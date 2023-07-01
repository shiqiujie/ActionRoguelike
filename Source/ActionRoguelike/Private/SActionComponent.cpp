// Fill out your copyright notice in the Description page of Project Settings.


#include "SActionComponent.h"
#include "SAction.h"


USActionComponent::USActionComponent()
{

	PrimaryComponentTick.bCanEverTick = true;

}

USActionComponent* USActionComponent::GetActionComponent(AActor* FromActor)
{
	if (FromActor)
	{
		return Cast<USActionComponent>(FromActor->GetComponentByClass(USActionComponent::StaticClass()));
	}

	return nullptr;
}

USAction* USActionComponent::GetAction(TSubclassOf<USAction> ActionClass)
{
  for (USAction* Action : Actions)
  {
   //IsA判断实例的基类是否是指定类
	  if (Action && Action->IsA(ActionClass))
	  {
	     return Action;
	  }
  }

  return nullptr;
}

void USActionComponent::BeginPlay()
{
	Super::BeginPlay();

	for (TSubclassOf<USAction> ActionClass : DefaultActions)
	{
		AddAction(GetOwner(),ActionClass);
	}

}

void USActionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//字符串的处理和引擎输出到屏幕上
	FString DebugMsg = GetNameSafe(GetOwner()) + " : " + ActiveGameplayTags.ToStringSimple();
	GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::White, DebugMsg);

}

void USActionComponent::AddAction(AActor* Instigator, TSubclassOf<USAction> ActionClass)
{
	if (!ensure(ActionClass))
	{
		return;
	}

	//Set it's Outer When New a Object
	USAction* NewAction = NewObject<USAction>(this, ActionClass);
	if (ensure(NewAction))
	{
		Actions.Add(NewAction);

		if (NewAction->bAutoStart && ensure(NewAction->CanStart(Instigator)))
		{
			NewAction->StartAction(Instigator);
		}
	}
}

void USActionComponent::RemoveAction(USAction* ActionToRemove)
{
	if (ensure(ActionToRemove && !ActionToRemove->IsRuning()))
	{
		Actions.Remove(ActionToRemove);
	}
}

bool USActionComponent::StartActionByName(AActor* Instigator, FName ActionName)
{
	for (USAction* Action : Actions)
	{
		if (Action && Action->ActionName == ActionName)
		{
			if (!Action->CanStart(Instigator))
			{
				FString FailedMsg = FString::Printf(TEXT("Failed to run Action:%s"),*ActionName.ToString());
				GEngine->AddOnScreenDebugMessage(-1,2.0f,FColor::Red,FailedMsg);
				continue;
			}
			//Is Client?
			if (!GetOwner()->HasAuthority())		
			{
			    ServerStartAction(Instigator, ActionName);
			}
				Action->StartAction(Instigator);
				return true;
		
		}
	}

	return false;
}

bool USActionComponent::StopActionByName(AActor* Instigator, FName ActionName)
{
	for (USAction* Action : Actions)
	{
		if (Action && Action->ActionName == ActionName)
		{
			if (Action->IsRuning())
			{
				// Is Client?
				if (!GetOwner()->HasAuthority())
				{
					ServerStopAction(Instigator, ActionName);
				}

				Action->StopAction(Instigator);
				return true;
			}		
		}
	}

	return false;
}

void USActionComponent::ServerStartAction_Implementation(AActor* Instigator, FName ActionName)
{
	StartActionByName(Instigator, ActionName);
}

void USActionComponent::ServerStopAction_Implementation(AActor* Instigator, FName ActionName)
{
	StopActionByName(Instigator, ActionName);
}

bool USActionComponent::HasGameplayTag(const FGameplayTag& InGameplayTag) const
{
	return ActiveGameplayTags.HasTagExact(InGameplayTag);
}


