#include "FPSObjective.h"

#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "FPSCharacter.h"

AFPSObjective::AFPSObjective()
{
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RootComponent = MeshComp;

	SphereCompCollision = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCompCollision"));
	SphereCompCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereCompCollision->SetCollisionResponseToAllChannels(ECR_Ignore);
	SphereCompCollision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	SphereCompCollision->SetupAttachment(MeshComp);
}

void AFPSObjective::BeginPlay()
{
	Super::BeginPlay();
}

void AFPSObjective::PlayEffects()
{
	UGameplayStatics::SpawnEmitterAtLocation(this, PickupFX, GetActorLocation());
}

void AFPSObjective::NotifyActorBeginOverlap(AActor* other)
{
	Super::NotifyActorBeginOverlap(other);
	AFPSCharacter* testCharacter = Cast<AFPSCharacter>(other);

	if (testCharacter)
	{
		testCharacter->bIsCarryingObjective = true;
		PlayEffects();
		Destroy();
	}
	
}
