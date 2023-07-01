// Fill out your copyright notice in the Description page of Project Settings.


#include "SInteractionComponent.h"
#include "SGameplayInterface.h"
#include "DrawDebugHelpers.h"
#include "SCharacter.h"
#include <Blueprint/UserWidget.h>
#include "SWorldUserWidget.h"
#include <Engine/Engine.h>

static TAutoConsoleVariable<bool> CVarDrawDebug(TEXT("su.EnableDrawDebug"), false, TEXT("Enable Draw Debug."), ECVF_Cheat);

// Sets default values for this component's properties
USInteractionComponent::USInteractionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	TraceRadius = 30.0f;
	TraceDistance = 500.f;
	CollisionChannel = ECollisionChannel::ECC_WorldDynamic;
}

// Called when the game starts
void USInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

// Called every frame
void USInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	APawn* MyPawn = Cast<APawn>(GetOwner());
	if (MyPawn->IsLocallyControlled())
	{
		FindBestInteactable();
	}
	// ...
}

void USInteractionComponent::FindBestInteactable()
{
	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(CollisionChannel);
	ASCharacter* OwnerPawn = Cast<ASCharacter>(GetOwner());

	FVector CameraLocation;
	FRotator CameraRotation;

	if (OwnerPawn)
	{
		CameraLocation = OwnerPawn->GetPawnViewLocation();
		CameraRotation = OwnerPawn->GetControlRotation();
	}

	FCollisionShape TraceShape;
	TraceShape.SetSphere(TraceRadius);

	FVector End = CameraLocation + CameraRotation.Vector() * TraceDistance;

	FHitResult Hit;
	//GetWorld()->LineTraceSingleByObjectType(Hit, Eyelocation, End, ObjectQueryParams);
	//用指定shape的滑动检测

	FocusedActor = nullptr;
	GetWorld()->SweepSingleByObjectType(Hit, CameraLocation, End, FQuat::Identity, ObjectQueryParams, TraceShape);

	AActor* HitActor = Hit.GetActor();

	if (HitActor)
	{
		//判断HitActor是否有模板接口的实现，注意U和I的区别
		if (HitActor->Implements<USGameplayInterface>())
		{
			FocusedActor = HitActor;

			if (CVarDrawDebug.GetValueOnGameThread())
			{
				DrawDebugSphere(GetWorld(), Hit.ImpactPoint, TraceRadius, 16, FColor::Green, false, 2.0f);
			}

		}
	}

	if (FocusedActor)
	{
		if (InteractUIInstance == nullptr && ensure(InteractUIClass))
		{
			InteractUIInstance = CreateWidget<USWorldUserWidget>(GetWorld(), InteractUIClass);
		}

		if (InteractUIInstance)
		{
			InteractUIInstance->AttachedActor = FocusedActor;
			if (!InteractUIInstance->IsInViewport())
			{
				InteractUIInstance->AddToViewport();
			}
		}
	}
	else
	{
		if (InteractUIInstance)
		{
			InteractUIInstance->RemoveFromParent();
		}		
	}

}

void USInteractionComponent::ServerInteract_Implementation(AActor* InActor)
{
	if (!InActor)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, "No Interact Actor!");
		return;
	}

	APawn* MyPawn = Cast<APawn>(GetOwner());
	ISGameplayInterface::Execute_Interact(InActor, MyPawn);

/*Another way to implemete interface*/
// 	const ISGameplayInterface* ASI = Cast<ISGameplayInterface>(InActor);
// 	ASI->Interact(MyPawn);

}

void USInteractionComponent::PrimaryInteract()
{
	//将FocusedActor转化成ID传给Server,然后从内存中获取
	ServerInteract(FocusedActor);
}
