// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SInteractionComponent.generated.h"

class USWorldUserWidget;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACTIONROGUELIKE_API USInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USInteractionComponent();

protected:

	// Reliable - Will always arrive, eventually. Request will be re-sent unless an acknowledgment was received.Based on TCP.
    // Unreliable - Not guaranteed, packet can get lost and won't retry.Based on UDP.
	// ������ҵ�λ��ͬ������֡����ģ����Բ���Ҫʮ��׼ȷ����ʱ��Ӧ��ʹ��unreliable����������
	// ��������ѷ�����Ϣ�ĺ�����Ӧ����Relieble,��Ϊ�ǳ��ؼ������ݶ�ʧ��

	UFUNCTION(Server,Reliable)
	void ServerInteract(AActor* InActor);
	// Called when the game starts
	virtual void BeginPlay() override;

	void FindBestInteactable();

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void PrimaryInteract();

protected:

	UPROPERTY()
	AActor* FocusedActor;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<USWorldUserWidget> InteractUIClass;

	UPROPERTY()
	USWorldUserWidget* InteractUIInstance;

	UPROPERTY(EditDefaultsOnly, Category ="Trace")
	float TraceRadius;

	UPROPERTY(EditDefaultsOnly, Category = "Trace")
	float TraceDistance;

	//����ֱ��ʹ��ö����Ϊ��Ա��������Ҫ��TEnumAsByte��ת��
	UPROPERTY(EditDefaultsOnly, Category = "Trace")
	TEnumAsByte<ECollisionChannel> CollisionChannel;
		
};
