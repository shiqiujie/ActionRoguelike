// Fill out your copyright notice in the Description page of Project Settings.


#include "SPickupItem_AbilityGeter.h"
#include "SActionComponent.h"

void ASPickupItem_AbilityGeter::Interact_Implementation(APawn* InstigatorPawn)
{
  Super::Interact_Implementation(InstigatorPawn);

  USActionComponent* ActionComp = USActionComponent::GetActionComponent(InstigatorPawn);

  if (ActionComp)
  {
    if (ActionComp->GetAction(ActionClass))
    {
       FString DebugMsg = FString::Printf(TEXT("Action '%s' already exist!"),*GetNameSafe(ActionClass));
       GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, DebugMsg);
       return;
    }

    ActionComp->AddAction(InstigatorPawn, ActionClass);
    OnInteractSuccess();
  }

}
