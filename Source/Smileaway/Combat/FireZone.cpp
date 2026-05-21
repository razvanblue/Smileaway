#include "FireZone.h"
#include "NiagaraComponent.h"
#include "Components/AudioComponent.h"
#include "Smileaway/Character/SmileawayCharacter.h"

AFireZone::AFireZone()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickInterval = 0.25f;
	
	FireEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Fire Effect"));
	FireEffect->SetupAttachment(GetRootComponent());
	
	AudioRotationRoot = CreateDefaultSubobject<USceneComponent>(TEXT("AudioRotationRoot"));
	AudioRotationRoot->SetupAttachment(GetRootComponent());
}


void AFireZone::InitializeFireEffect()
{
	CurrentProgress = TargetProgress;
	FireEffect->SetVariableFloat(TEXT("Progress"), FMath::Clamp(1.f - CurrentProgress, 0.f, 1.f));
	FireEffect->SetVariableFloat(TEXT("SpawnRate"), FireDensity * CurrentProgress);
	FireEffect->SetVariableFloat(TEXT("OuterRadius"), OuterRadius);
	
	const float Coverage = FMath::Clamp(1.f - FMath::Square(InnerRadius / OuterRadius), 0.f, 1.f);
	FireEffect->SetVariableFloat(TEXT("DiscCoverage"), Coverage);
}


void AFireZone::BeginPlay()
{
	Super::BeginPlay();
	
	InitializeAudioRing();
	
	Target = Cast<ASmileawayCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());
}


void AFireZone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CurrentProgress == TargetProgress) return;
	
	const float ExpansionRate =
		BaseExpansionRate * FMath::Max(1.f, FMath::Abs(TargetProgress - CurrentProgress) * 10.f);
	CurrentProgress = FMath::FInterpConstantTo(CurrentProgress, TargetProgress, DeltaTime, ExpansionRate);
	
	FireEffect->SetVariableFloat(TEXT("Progress"), 1.f - CurrentProgress);
	FireEffect->SetVariableFloat(TEXT("SpawnRate"), FireDensity * CurrentProgress);
	
	UpdateAudioExpansion();
	
	DamageOverTime();
}


void AFireZone::SetTargetProgress(float InTargetProgress)
{
	TargetProgress = FMath::Clamp(InTargetProgress, 0.001f, 1.f);
}


void AFireZone::DamageOverTime()
{
	FVector TargetLocationRelative = GetTransform().InverseTransformPosition(Target->GetActorLocation());
	FVector2D TargetLocationRelative2D{TargetLocationRelative.X, TargetLocationRelative.Y};
	
	auto DistanceSquared = TargetLocationRelative2D.SizeSquared();
	if (InnerRadius * InnerRadius < DistanceSquared && DistanceSquared < OuterRadius * OuterRadius)
	{
		float AngleRadians = -FMath::Atan2(TargetLocationRelative.Y, TargetLocationRelative.X); // Counter-clockwise
		float AngleDegrees = FMath::RadiansToDegrees(AngleRadians);
		if (AngleDegrees < 0.f)
		{
			AngleDegrees += 360.f;
		}
		
		float FireSpreadAngle = CurrentProgress * 360.f;
		
		if (AngleDegrees <= FireSpreadAngle)
		{
			IHitInterface::Execute_GetHit(Target, Target->GetActorLocation(), {.Damage = DamagePerTick}, this);
		}
	}
}

void AFireZone::InitializeAudioRing()
{
	if (!FireSound || AudioDegreesSpacing <= 0.f) return;

	// Calculate how many total fixed segments fit into a 360-degree circle
	int32 TotalNodes = FMath::FloorToInt(360.f / AudioDegreesSpacing);
	float MidRadius = (InnerRadius + OuterRadius) / 2.f;

	for (int32 i = 0; i < TotalNodes; ++i)
	{
		FString CompName = FString::Printf(TEXT("StaticFireAudio_%d"), i);
		UAudioComponent* AudioComp = NewObject<UAudioComponent>(this, UAudioComponent::StaticClass(), *CompName);
		if (AudioComp)
		{
			// Node 0 is at 0 degrees (Forward / X-axis), Node 1 is at -15 degrees (Clockwise layout)
			float AngleDegrees = -(i * AudioDegreesSpacing); 
			float Radian = FMath::DegreesToRadians(AngleDegrees);
			FVector LocalLocation(FMath::Cos(Radian) * MidRadius, -FMath::Sin(Radian) * MidRadius, 0.f);
            
			AudioComp->SetRelativeLocation(LocalLocation);
			AudioComp->SetSound(FireSound);
			AudioComp->bAllowSpatialization = true;
			AudioComp->bAutoActivate = false; // Start dormant
            
			AudioPool.Add(AudioComp);
			AudioComp->RegisterComponent();
			// AudioComp->AttachToComponent(AudioRotationRoot, FAttachmentTransformRules::KeepRelativeTransform);
			AudioComp->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
		}
	}
}


void AFireZone::UpdateAudioExpansion()
{
	if (AudioPool.Num() == 0) return;

	float CurrentDegrees = CurrentProgress * 360.f;

	// Rotate the parent pivot component clockwise.
	FRotator NewRotation(0.f, CurrentDegrees, 0.f);
	AudioRotationRoot->SetRelativeRotation(NewRotation);

	int32 ActiveNodesNeeded = CurrentDegrees / AudioDegreesSpacing + 1;
	ActiveNodesNeeded = FMath::Min(ActiveNodesNeeded, AudioPool.Num());

	if (CurrentProgress <= 0.001f) ActiveNodesNeeded = 0;
	
	float MidRadius = (InnerRadius + OuterRadius) / 2.f;
	float CurrentLeadingAngle = CurrentProgress * 360.f;
	
	for (int32 i = 0; i < ActiveNodesNeeded; ++i)
	{
		float LocalAngle = CurrentLeadingAngle - (i * AudioDegreesSpacing);
		float Radian = FMath::DegreesToRadians(LocalAngle);
        
		// Fire turns counter-clockwise
		FVector LocalLocation(FMath::Cos(Radian) * MidRadius, -FMath::Sin(Radian) * MidRadius, 50.f);
		AudioPool[i]->SetRelativeLocation(LocalLocation);
		
		if (AudioPool[i]->IsPlaying() == false)
		{
			AudioPool[i]->FadeIn(0.3f, 1.f); 
		}
		
		FVector WorldPos = AudioPool[i]->GetComponentLocation();
		FColor SphereColor = (i == 0) ? FColor::Green : FColor::Red;
		DrawDebugSphere(
			GetWorld(), WorldPos, 150.f, 12, SphereColor, false, -1.f, 1, 3.f);
	}
	
	ActiveAudioCount = ActiveNodesNeeded;
}