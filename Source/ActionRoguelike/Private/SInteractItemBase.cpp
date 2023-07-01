// Fill out your copyright notice in the Description page of Project Settings.


#include "SInteractItemBase.h"
#include "Components/SphereComponent.h"

// Sets default values
ASInteractItemBase::ASInteractItemBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionComp = CreateDefaultSubobject<USphereComponent>("CollisionComp");
	RootComponent = CollisionComp;

	StatiMeshComp = CreateDefaultSubobject<UStaticMeshComponent>("BaseMesh");
	StatiMeshComp->SetupAttachment(CollisionComp);

	SetReplicates(true);
}

// Called when the game starts or when spawned
void ASInteractItemBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASInteractItemBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASInteractItemBase::Interact_Implementation(APawn* InstigatorPawn)
{

}

