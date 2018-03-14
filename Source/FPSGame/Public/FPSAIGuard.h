// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "FPSAIGuard.generated.h"

class UPawnSensingComponent;
class APawn;

UENUM(BlueprintType)
enum class EAIState : uint8
{
	Idle,
	Suspicious,
	Alerted,
};

UCLASS()
class FPSGAME_API AFPSAIGuard : public ACharacter
{
	GENERATED_BODY()

public:
	AFPSAIGuard();
	~AFPSAIGuard();
protected:
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UPawnSensingComponent * PawnSensingComp;
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnPawnSeen(APawn* Pawn);

	UFUNCTION()
	void OnHearNoise(APawn* aInstigator, const FVector& Location, float Volume);

	FRotator OriginalRotation;
	FTimerHandle TimerHandle_ResetOrientation;

	EAIState GuardAIState;

	void SetAIState(EAIState newState);

	UFUNCTION(BlueprintImplementableEvent, Category = "AI")
	void OnStateChanged(EAIState newState);

	UFUNCTION()
	void ResetOrientation();

public:	
	virtual void Tick(float DeltaTime) override;
};
