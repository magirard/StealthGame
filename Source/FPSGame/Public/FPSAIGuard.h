// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "FPSAIGuard.generated.h"

class UPawnSensingComponent;
class APawn;

UCLASS()
class FPSGAME_API AFPSAIGuard : public ACharacter
{
	GENERATED_BODY()

public:
	AFPSAIGuard();

protected:
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UPawnSensingComponent * PawnSensingComp;
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnPawnSeen(APawn* Pawn);

public:	
	virtual void Tick(float DeltaTime) override;
};
