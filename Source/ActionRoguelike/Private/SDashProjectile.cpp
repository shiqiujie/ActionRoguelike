// Fill out your copyright notice in the Description page of Project Settings.


#include "SDashProjectile.h"
#include "Components/SphereComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "SAttributeComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ASDashProjectile::ASDashProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called every frame
void ASDashProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called when the game starts or when spawned
void ASDashProjectile::BeginPlay()
{
	Super::BeginPlay();	
	GetWorldTimerManager().SetTimer(DelayToExplodeTimer,this,&ASDashProjectile::DelayToTeleport_TimeElapsed,DelayTime);
}

void ASDashProjectile::DelayToTeleport_TimeElapsed()
{
	Explode();
}

void ASDashProjectile::TeleportInstigator()
{
	AActor* TeleportActor = GetInstigator();
	if (TeleportActor)
	{
		bool bIsTeleportSuccess = TeleportActor->TeleportTo(GetActorLocation(), TeleportActor->GetActorRotation(),false,false);
		if (!bIsTeleportSuccess)
		{
			UE_LOG(LogTemp, Warning, TEXT("TeleportFailed!"));
		}
	}
}

void ASDashProjectile::Explode_Implementation()
{
	GetWorldTimerManager().ClearTimer(DelayToExplodeTimer);

	EffectComp->Deactivate();
	MovementComp->StopMovementImmediately();
	SetActorEnableCollision(false);

	if (ImpactVFX)
	{
		UGameplayStatics::SpawnEmitterAtLocation(this, ImpactVFX, GetActorLocation(), GetActorRotation());
	}

	FTimerHandle DelayToTeleportTimer;

	GetWorldTimerManager().SetTimer(DelayToTeleportTimer, this, &ASDashProjectile::TeleportInstigator, 0.2f);

//  �ضϸ����Explode��������ΪActor�ᱻ�ݻ٣�������Ҫ�ȵڶ���Timer��Ч
// 	Super::Explode_Implementation();
}



