// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSExtractionZone.h"

#include "Components/BoxComponent.h"
#include "Components/PrimitiveComponent.h"
#include "Components/DecalComponent.h"
#include "FPSCharacter.h"
#include "FPSGameMode.h"
#include "Kismet/GameplayStatics.h"

AFPSExtractionZone::AFPSExtractionZone()
{
	TriggerZone = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerZone"));
	TriggerZone->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	TriggerZone->SetCollisionResponseToAllChannels(ECR_Ignore);
	TriggerZone->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	TriggerZone->SetBoxExtent(FVector(200));
	TriggerZone->SetHiddenInGame(false);

	RootComponent = TriggerZone;

	TriggerZone->OnComponentBeginOverlap.AddDynamic(this, &AFPSExtractionZone::OnHeroOverlap);

	DecalComp = CreateDefaultSubobject<UDecalComponent>(TEXT("Decal"));
	DecalComp->DecalSize = FVector(200);
	DecalComp->SetupAttachment(RootComponent);
}


void AFPSExtractionZone::OnHeroOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	

	AFPSCharacter* MyPawn = Cast<AFPSCharacter>(OtherActor);
	if (!MyPawn)
	{
		return;
	}

	UE_LOG(LogTemp, Log, TEXT("Overlapped with extraction zone"));

	if (MyPawn->bIsCarryingObjective)
	{
		AFPSGameMode* gameMode = Cast<AFPSGameMode>(GetWorld()->GetAuthGameMode());
		if (gameMode)
		{
			gameMode->CompleteMission(MyPawn);
		}
	}
	else
	{
		UGameplayStatics::PlaySound2D(this, ObjectiveMissingSound);
	}
}

