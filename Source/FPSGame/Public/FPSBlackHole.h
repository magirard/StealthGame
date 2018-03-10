// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FPSBlackHole.generated.h"

class USphereComponent;
class UStaticMeshComponent;

UCLASS()
class FPSGAME_API AFPSBlackHole : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFPSBlackHole();

	UPROPERTY(EditAnywhere, Category = "Triggers")
	float strength = 10.0f;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "Triggers")
		USphereComponent* TriggerZone;
	UPROPERTY(EditDefaultsOnly, Category = "Triggers")
		USphereComponent* KillZone;
	UPROPERTY(EditDefaultsOnly, Category = "Triggers")
		UStaticMeshComponent* RootMesh;

public:	


	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
};
