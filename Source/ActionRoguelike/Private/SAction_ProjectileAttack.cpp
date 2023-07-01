// Fill out your copyright notice in the Description page of Project Settings.


#include "SAction_ProjectileAttack.h"
#include "SCharacter.h"
#include "Kismet/GameplayStatics.h"

USAction_ProjectileAttack::USAction_ProjectileAttack()
{
	HandSocketName = "Muzzle_01";
}

void USAction_ProjectileAttack::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);

	ASCharacter* InstigatorCharacter = Cast<ASCharacter>(Instigator);
	if (InstigatorCharacter)
	{
		InstigatorCharacter->PlayAnimMontage(AttackMTG);
		if (MuzzleVFX)
		{
			UGameplayStatics::SpawnEmitterAttached(MuzzleVFX, InstigatorCharacter->GetMesh(), HandSocketName);
		}
		
		if (InstigatorCharacter->HasAuthority())
		{
			FTimerHandle AttackDelayTimer;
			//FTimerDelegate: can pass params
			FTimerDelegate Delegate;
			//此处绑定的函数跟多播一样，依然需要用UFUNCTION宏修饰
			Delegate.BindUFunction(this, "DelayAttack_Elapsed", InstigatorCharacter);

			GetWorld()->GetTimerManager().SetTimer(AttackDelayTimer, Delegate, AttackDelayTime, false);
		}		
	}
		
}

void USAction_ProjectileAttack::DelayAttack_Elapsed(ASCharacter* InstigatorCharacter)
{
	
	if (InstigatorCharacter)
	{		

		FVector HandLocation = InstigatorCharacter->GetMesh()->GetSocketLocation(HandSocketName);

			//assignment 2-1
		FHitResult Hit;

		FCollisionShape Shape;
		Shape.SetSphere(20.0f);

		FCollisionQueryParams CollisionParams;
		CollisionParams.AddIgnoredActor(InstigatorCharacter);

		FCollisionObjectQueryParams QueryParams;
		QueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);
		QueryParams.AddObjectTypesToQuery(ECC_WorldStatic);
		QueryParams.AddObjectTypesToQuery(ECC_Pawn);

		FVector CameraLocation;
		FVector FowardDirection = InstigatorCharacter->GetControlRotation().Vector();
		FowardDirection.Normalize();

		CameraLocation = InstigatorCharacter->GetPawnViewLocation();
		FVector EndLocation = CameraLocation + FowardDirection * 5000.0f;

		bool bIsHit = GetWorld()->SweepSingleByObjectType(Hit, CameraLocation, EndLocation, FQuat::Identity, QueryParams, Shape, CollisionParams);

		//bool bIsHit = GetWorld()->LineTraceSingleByObjectType(Hit, CameraLocation, EndLocation, QueryParams);
		if (bIsHit)
		{
			EndLocation = Hit.ImpactPoint;
		}

		FRotator FixedRotator;
		FixedRotator = (EndLocation - HandLocation).Rotation();
		FTransform SpawnTM = FTransform(FixedRotator, HandLocation);

		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParams.Instigator = InstigatorCharacter;
		//ensure(ptr) : if not ,assert,F5 to Continue
		//ensureAlways(ptr): assert everytime
		//ensureMsgf(ptr,TEXT):ensure + log
		//check(ptr): this is considered like an error and can't continue
		//above doesn't run in packaged games

		//Debug editor:more debug information,avoid program optimization
		if (ensure(ProjectileClass))
		{
			GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnTM, SpawnParams);
		}
	}

	StopAction(InstigatorCharacter);
}