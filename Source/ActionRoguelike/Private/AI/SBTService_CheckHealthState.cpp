// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SBTService_CheckHealthState.h"
#include "AIModule/Classes/AIController.h"
#include "AI/SAICharacter.h"
#include "AIModule/Classes/BehaviorTree/BlackboardComponent.h"
#include "SAttributeComponent.h"

void USBTService_CheckHealthState::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	AAIController* AIC = OwnerComp.GetAIOwner();
	if (AIC)
	{

		ASAICharacter* AICharacter = Cast<ASAICharacter>(AIC->GetPawn());
		USAttributeComponent* AttComp = nullptr;
		if (AICharacter)
		{
			AttComp = AICharacter->GetAttributeComp();
		}

		UBlackboardComponent* BBComp = OwnerComp.GetBlackboardComponent(); 
		if (AICharacter && AttComp && BBComp)
		{
			BBComp->SetValueAsBool(LowHealthKey.SelectedKeyName, AttComp->IsLowHealth());
		}	
	}
}
 