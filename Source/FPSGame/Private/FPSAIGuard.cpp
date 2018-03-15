// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSAIGuard.h"
#include "Perception/PawnSensingComponent.h"
#include "DrawDebugHelpers.h"
#include "FPSGameMode.h"
#include "AI/Navigation/NavigationSystem.h"

// Sets default values
AFPSAIGuard::AFPSAIGuard()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComponent"));

	PawnSensingComp->OnSeePawn.AddDynamic(this, &AFPSAIGuard::OnPawnSeen);
	PawnSensingComp->OnHearNoise.AddDynamic(this, &AFPSAIGuard::OnHearNoise);

	GuardAIState = EAIState::Idle;
	goingUpPatrol = true;
}

AFPSAIGuard::~AFPSAIGuard()
{
}

// Called when the game starts or when spawned
void AFPSAIGuard::BeginPlay()
{
	Super::BeginPlay();
	OriginalRotation = GetActorRotation();

	if (bPatrol)
	{
		UNavigationSystem::SimpleMoveToActor(GetController(), PatrolPath[mCurrentPatrolPointId]);
	}
}

void AFPSAIGuard::OnHearNoise(APawn* aInstigator, const FVector& Location, float Volume)
{
	if (GuardAIState == EAIState::Alerted)
	{
		return;
	}

	FVector direction = Location - GetActorLocation();
	direction.Normalize();

	FRotator newRot = FRotationMatrix::MakeFromX(direction).Rotator();
	newRot.Pitch = 0.0f;
	newRot.Roll = 0.0f;
	SetActorRotation(newRot);
	DrawDebugSphere(GetWorld(), Location, 32.0f, 12, FColor::Red, false, 10.0f);

	
	GetWorldTimerManager().ClearTimer(TimerHandle_ResetOrientation);
	GetWorldTimerManager().SetTimer(TimerHandle_ResetOrientation, this, &AFPSAIGuard::ResetOrientation, 3.0f);


	SetAIState(EAIState::Suspicious);

	GetController()->StopMovement();
}

void AFPSAIGuard::SetAIState(EAIState newState)
{
	if (newState != GuardAIState)
	{
		GuardAIState = newState;
		OnStateChanged(GuardAIState);
	}

}
void AFPSAIGuard::ResetOrientation()
{
	if (GuardAIState == EAIState::Alerted)
	{
		return;
	}

	SetActorRotation(OriginalRotation);
	SetAIState(EAIState::Idle);
	if (bPatrol)
	{
		UNavigationSystem::SimpleMoveToActor(GetController(), PatrolPath[mCurrentPatrolPointId]);
	}
}

void AFPSAIGuard::OnPawnSeen(APawn * Pawn)
{
	if (Pawn == nullptr)
	{
		return;
	}

	SetAIState(EAIState::Alerted);
	DrawDebugSphere(GetWorld(), Pawn->GetActorLocation(), 32.0f, 12, FColor::Yellow, false, 10.0f);

	AFPSGameMode* gameMode = Cast<AFPSGameMode>(GetWorld()->GetAuthGameMode());
	if (gameMode)
	{
		gameMode->CompleteMission(Pawn, false);
	}

	GetController()->StopMovement();
}

// Called every frame
void AFPSAIGuard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (GuardAIState == EAIState::Idle)
	{
		if (bPatrol && PatrolPath.Num() >= 2)
		{
			/*
			FVector direction = PatrolPath[mCurrentPatrolPointId]->GetActorLocation() - GetActorLocation();
			float dist = direction.Size();
			direction.Normalize();

			FRotator newRot = FRotationMatrix::MakeFromX(direction).Rotator();
			newRot.Pitch = 0.0f;
			newRot.Roll = 0.0f;
			SetActorRotation(newRot);
			SetActorLocation(GetActorLocation() + GetActorForwardVector() * MovementSpeed);
			*/

			FVector direction = PatrolPath[mCurrentPatrolPointId]->GetActorLocation() - GetActorLocation();
			float dist = direction.Size();
			if (dist < DistanceTresholdReachedTarget)
			{
				if (goingUpPatrol)
				{
					if (mCurrentPatrolPointId == PatrolPath.Num() - 1)
					{
						mCurrentPatrolPointId--;
						goingUpPatrol = false;
					}
					else
					{
						mCurrentPatrolPointId++;
					}
				}
				else
				{
					if (mCurrentPatrolPointId == 0)
					{
						mCurrentPatrolPointId++;
						goingUpPatrol = true;
					}
					else
					{
						mCurrentPatrolPointId--;
					}
				}
				UNavigationSystem::SimpleMoveToActor(GetController(), PatrolPath[mCurrentPatrolPointId]);
			}
		}
	}
}
