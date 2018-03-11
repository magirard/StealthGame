// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "FPSGameMode.h"
#include "FPSHUD.h"
#include "FPSCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"

AFPSGameMode::AFPSGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/Blueprints/BP_Player"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = AFPSHUD::StaticClass();
}

void AFPSGameMode::CompleteMission(APawn* instigatorPawn)
{
	if (instigatorPawn)
	{
		instigatorPawn->DisableInput(nullptr);

		if (SpectatingViewPointClass)
		{
			APlayerController* PC = Cast<APlayerController>(instigatorPawn->GetController());
			if (PC)
			{
				AActor* NewViewTarget = nullptr;
				TArray<AActor*> ReturnedActors;
				UGameplayStatics::GetAllActorsOfClass(this, SpectatingViewPointClass, ReturnedActors);
				if (ReturnedActors.Num() > 0)
				{
					NewViewTarget = ReturnedActors[0];
					FViewTargetTransitionParams params;
					params.BlendTime = 1.0f;
					params.BlendFunction = EViewTargetBlendFunction::VTBlend_Cubic;
					PC->SetViewTarget(NewViewTarget, params);
				}

			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("SpectatingViewPointClass is nullptr, Please update gamemode class with valid subclass"));
		}
	}
	OnMissionCompleted(instigatorPawn);


}
