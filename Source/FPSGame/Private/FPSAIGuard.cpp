// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSAIGuard.h"
#include "Perception/PawnSensingComponent.h"
#include "DrawDebugHelpers.h"

// Sets default values
AFPSAIGuard::AFPSAIGuard()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComponent"));

	PawnSensingComp->OnSeePawn.AddDynamic(this, &AFPSAIGuard::OnPawnSeen);
	PawnSensingComp->OnHearNoise.AddDynamic(this, &AFPSAIGuard::OnHearNoise);
}

AFPSAIGuard::~AFPSAIGuard()
{
}

// Called when the game starts or when spawned
void AFPSAIGuard::BeginPlay()
{
	Super::BeginPlay();
	OriginalRotation = GetActorRotation();
}

void AFPSAIGuard::OnHearNoise(APawn* aInstigator, const FVector& Location, float Volume)
{
	FVector direction = Location - GetActorLocation();
	direction.Normalize();

	FRotator newRot = FRotationMatrix::MakeFromX(direction).Rotator();
	newRot.Pitch = 0.0f;
	newRot.Roll = 0.0f;
	SetActorRotation(newRot);
	DrawDebugSphere(GetWorld(), Location, 32.0f, 12, FColor::Red, false, 10.0f);

	
	GetWorldTimerManager().ClearTimer(TimerHandle_ResetOrientation);
	GetWorldTimerManager().SetTimer(TimerHandle_ResetOrientation, this, &AFPSAIGuard::ResetOrientation, 3.0f);
}

void AFPSAIGuard::ResetOrientation()
{
	SetActorRotation(OriginalRotation);

}

void AFPSAIGuard::OnPawnSeen(APawn * Pawn)
{
	if (Pawn == nullptr)
	{
		return;
	}
	DrawDebugSphere(GetWorld(), Pawn->GetActorLocation(), 32.0f, 12, FColor::Yellow, false, 10.0f);
}

// Called every frame
void AFPSAIGuard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}