// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SGameplayInterface.h"
#include "SInteractItemBase.generated.h"

class USphereComponent;

UCLASS()
class ACTIONROGUELIKE_API ASInteractItemBase : public AActor, public ISGameplayInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's propesties
	ASInteractItemBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	USphereComponent* CollisionComp;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	UStaticMeshComponent* StatiMeshComp;

	//UFUNCTION(BlueprintCallable,BlueprintNativeEvent)
	//void Interact(APawn* InstigatorPawn);函数申明时用了BlueprintNativeEvent,所以接口函数用用_Implementation，不然就是正常的函数名
    void Interact_Implementation(APawn* InstigatorPawn) override;

};
