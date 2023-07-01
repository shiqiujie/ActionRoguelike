// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SAICharacter.h"
#include "AIModule/Classes/Perception/PawnSensingComponent.h"
#include "AIModule/Classes/AIController.h"
#include "AIModule/Classes/BehaviorTree/BlackboardComponent.h"
#include "DrawDebugHelpers.h"
#include "SAttributeComponent.h"
#include "BrainComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "SActionComponent.h"
#include <Blueprint/UserWidget.h>
#include "SWorldUserWidget.h"

// Sets default values
ASAICharacter::ASAICharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>("PawnSensingComp");

	AttributeComp = CreateDefaultSubobject<USAttributeComponent>("AttributeComp");
	ActionComp = CreateDefaultSubobject<USActionComponent>("ActionComp");

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Ignore);
	GetMesh()->SetGenerateOverlapEvents(true);

	HitFlashTimeName = "StartTime";
}

void ASAICharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	PawnSensingComp->OnSeePawn.AddDynamic(this, &ASAICharacter::OnPawnSeen);

	AttributeComp->OnHealthChanged.AddDynamic(this, &ASAICharacter::OnHealthChanged);

}

USAttributeComponent* ASAICharacter::GetAttributeComp()
{
	return AttributeComp; 
}

void ASAICharacter::SetTargetActor(AActor* NewTarget)
{
	AAIController* AIC = Cast<AAIController>(GetController());

	TargetAcotr = NewTarget;

	if (AIC)
	{
		AIC->GetBlackboardComponent()->SetValueAsObject("TargetActor", NewTarget);;
	}
}

void ASAICharacter::OnPawnSeen(APawn* Pawn)
{
   if (Pawn != TargetAcotr && HasAuthority())
   {
	 MulticastOnPawnSeenEffects();
   }

	SetTargetActor(Pawn);	
}


void ASAICharacter::MulticastOnPawnSeenEffects_Implementation()
{
	USWorldUserWidget* SpottedUI = CreateWidget<USWorldUserWidget>(GetWorld(), SpottedUMGClass);
	if (SpottedUI)
	{
		SpottedUI->AttachedActor = this;
		SpottedUI->AddToViewport(10);
	}

	//DrawDebugString(GetWorld(), GetActorLocation(), "PLAYER SPOTTED", nullptr, FColor::Green, 4.0f, true);
}

void ASAICharacter::OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float Delta)
{
	if (Delta < 0.0f)
	{

		GetMesh()->SetScalarParameterValueOnMaterials(HitFlashTimeName, GetWorld()->GetTimeSeconds());

		if (InstigatorActor != this)
		{
			SetTargetActor(InstigatorActor);
		}

		if (NewHealth <= 0.0f)
		{
			OnDie();
		}
	}
}

void ASAICharacter::OnDie()
{
	AAIController* AIC = Cast<AAIController>(GetController());
	if (AIC)
	{
		AIC->GetBrainComponent()->StopLogic("Killed");
	}

	//ragdoll
	GetMesh()->SetAllBodiesSimulatePhysics(true);
	GetMesh()->SetCollisionProfileName("Ragdoll");

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCharacterMovement()->DisableMovement();

	//set lifespan
	SetLifeSpan(10.0f);
}

