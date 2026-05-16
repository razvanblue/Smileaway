#include "FireZone.h"
#include "NiagaraComponent.h"

AFireZone::AFireZone()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickInterval = 0.25f;
	
	FireEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Fire Effect"));
	FireEffect->SetupAttachment(GetRootComponent());
}


void AFireZone::BeginPlay()
{
	Super::BeginPlay();
	
	CurrentProgress = TargetProgress;
	FireEffect->SetVariableFloat(TEXT("Progress"), FMath::Clamp(1.f - CurrentProgress, 0.f, 1.f));
	FireEffect->SetVariableFloat(TEXT("SpawnRate"), FireDensity * CurrentProgress);
}


void AFireZone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CurrentProgress == TargetProgress) return;
	
	CurrentProgress = FMath::FInterpConstantTo(CurrentProgress, TargetProgress, DeltaTime, RealExpansionRate);
	
	FireEffect->SetVariableFloat(TEXT("Progress"), 1.f - CurrentProgress);
	FireEffect->SetVariableFloat(TEXT("SpawnRate"), FireDensity * CurrentProgress);
}


void AFireZone::SetTargetProgress(float InTargetProgress)
{
	TargetProgress = FMath::Clamp(InTargetProgress, 0.001f, 1.f);
	RealExpansionRate = BaseExpansionRate * FMath::Max(1.f, FMath::Abs(TargetProgress - CurrentProgress) * 10.f);
}

