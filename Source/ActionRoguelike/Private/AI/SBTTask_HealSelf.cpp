// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SBTTask_HealSelf.h"
#include "AIModule/Classes/AIController.h"
#include "AI/SAICharacter.h"
#include "SAttributeComponent.h"

EBTNodeResult::Type USBTTask_HealSelf::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIC = OwnerComp.GetAIOwner();
	if (AIC)
	{
		ASAICharacter* AICharacter = Cast<ASAICharacter>(AIC->GetPawn());

		USAttributeComponent* AttComp;
		if (AICharacter)
		{
			AttComp = AICharacter->GetAttributeComp();		

			AttComp->ApplyHealthChange(AICharacter,AttComp->GetMaxHealth());

			return EBTNodeResult::Succeeded;
		}
	}

	return EBTNodeResult::Failed;
}
