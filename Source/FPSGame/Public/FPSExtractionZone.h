// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FPSExtractionZone.generated.h"


class UBoxComponent;
class UPrimitiveComponent;
class UDecalComponent;
class USoundBase;

UCLASS()
class FPSGAME_API AFPSExtractionZone : public AActor
{
	GENERATED_BODY()
	
public:	
	AFPSExtractionZone();

protected:


	UPROPERTY(EditDefaultsOnly, Category = "Triggers")
		UBoxComponent* TriggerZone;

	UPROPERTY(EditDefaultsOnly, Category = "Triggers")
		UDecalComponent* DecalComp;

	UFUNCTION()
	void OnHeroOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UPROPERTY(EditDefaultsOnly, Category = "Sounds")
	USoundBase* ObjectiveMissingSound;
};
