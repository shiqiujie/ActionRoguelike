// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SBTService_CheckAttackRange.h"
#include "AIModule/Classes/BehaviorTree/BlackboardComponent.h"
#include "AIModule/Classes/AIController.h"

void USBTService_CheckAttackRange::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	//此处comp传过来的是引用，通常我们都是用Pointer来传值，AI有所不同，经常用reference来传值
	UBlackboardComponent* BBComp = OwnerComp.GetBlackboardComponent();

	if (ensure(BBComp))
	{
		AActor* TargetActor = Cast<AActor>(BBComp->GetValueAsObject("TargetActor"));

		if (TargetActor)
		{
			AAIController* MyController = OwnerComp.GetAIOwner();
			if (ensure(MyController))
			{
				APawn* AIPawn = MyController->GetPawn();
				if (ensure(AIPawn))
				{
					float DistanceTo = FVector::Distance(TargetActor->GetActorLocation(), AIPawn->GetActorLocation());
					bool bWithinRange = DistanceTo < 2000.0f;
					bool bHasLOS = false;

					if (bWithinRange)
					{
						bHasLOS = MyController->LineOfSightTo(TargetActor);
					}

					BBComp->SetValueAsBool(AttackRangeKey.SelectedKeyName, (bWithinRange&& bHasLOS));
				}

			}
		}

	}
}
