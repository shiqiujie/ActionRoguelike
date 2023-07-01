// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SInteractItemBase.h"
#include "SItemChest.generated.h"

class UStaticMeshComponent;

UCLASS()
class ACTIONROGUELIKE_API ASItemChest : public ASInteractItemBase
{
	GENERATED_BODY()

public:	
	// Sets default values for this actor's properties
	ASItemChest();

	void Interact_Implementation(APawn* InstigatorPawn) override;

protected:

	//OnRep only executed on client
	UFUNCTION()
	void OnRep_bLidOpened();

protected:

	//ReplicatedUsing = "FunctionName" ���������仯ʱ�����󶨺���
	UPROPERTY(ReplicatedUsing = "OnRep_bLidOpened")
	bool bLidOpened;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* LidMesh;

	UPROPERTY(EditAnywhere)
	float TargetPitch;

};
