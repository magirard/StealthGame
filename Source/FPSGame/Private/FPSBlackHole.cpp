// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSBlackHole.h"

#include "Components/SphereComponent.h"
#include "Components/PrimitiveComponent.h"
#include "Components/ActorComponent.h"

// Sets default values
AFPSBlackHole::AFPSBlackHole()
{
	PrimaryActorTick.bCanEverTick = true;

	RootMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RootMesh"));
	RootMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RootComponent = RootMesh;

	TriggerZone = CreateDefaultSubobject<USphereComponent>(TEXT("TriggerZone"));
	TriggerZone->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	TriggerZone->SetCollisionResponseToAllChannels(ECR_Overlap);

	KillZone = CreateDefaultSubobject<USphereComponent>(TEXT("KillZone"));
	KillZone->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	KillZone->SetCollisionResponseToAllChannels(ECR_Overlap);

	TriggerZone->SetupAttachment(RootMesh);
	KillZone->SetupAttachment(RootMesh);
}

// Called when the game starts or when spawned
void AFPSBlackHole::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFPSBlackHole::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TArray<UPrimitiveComponent*> OverlappingComponents;
	TriggerZone->GetOverlappingComponents(OverlappingComponents);

	for(int i = 0; i < OverlappingComponents.Num(); ++i)
	{
		UPrimitiveComponent* current = OverlappingComponents[i];
		if (current && current->IsSimulatingPhysics())
		{
			FVector offset = GetActorLocation() - current->GetOwner()->GetActorLocation();
			current->AddForce(offset * offset.Size() * strength);
		}
	}
	
	TArray<UPrimitiveComponent*> OverlappingComponentsKillZone;
	KillZone->GetOverlappingComponents(OverlappingComponentsKillZone);

	for (int i = 0; i < OverlappingComponentsKillZone.Num(); ++i)
	{
		UPrimitiveComponent* current = OverlappingComponentsKillZone[i];

		//Destroy everything but myself
		if (Cast<AFPSBlackHole>(current->GetOwner()) == nullptr)
		{
			current->GetOwner()->Destroy();
		}
	}
	
}
