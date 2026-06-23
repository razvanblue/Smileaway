// Fill out your copyright notice in the Description page of Project Settings.


#include "SmileawayCharacter.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Materials/MaterialInterface.h"
#include "NiagaraFunctionLibrary.h"
#include "Smileaway/Smileaway.h"
#include "Smileaway/UI/HealthBarWidgetComponent.h"
#include "Smileaway/Components/CharacterStats.h"
#include "Smileaway/Components/LevelingComponent.h"

// Sets default values
ASmileawayCharacter::ASmileawayCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	LevelingComponent = CreateDefaultSubobject<ULevelingComponent>(TEXT("Leveling Component"));
	Stats = CreateDefaultSubobject<UCharacterStats>(TEXT("Character Stats"));
	
	HealthBarWidget = CreateDefaultSubobject<UHealthBarWidgetComponent>(TEXT("Health Bar Component"));
	HealthBarWidget->SetupAttachment(GetRootComponent());
	
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_HitInteractable, ECR_Overlap);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Projectile, ECR_Overlap);
}


// Called when the game starts or when spawned
void ASmileawayCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	HealthBarWidget->SetHealth(Stats->GetHealth(), Stats->GetStat(EStats::MaxHP));
	Stats->OnHealthChanged.AddDynamic(HealthBarWidget, &UHealthBarWidgetComponent::SetHealth);
	
	UpdateMovementSpeed(Stats->GetStat(EStats::Speed));
	Stats->OnSpeedChanged.AddDynamic(this, &ThisClass::UpdateMovementSpeed);
	
	InitializeHitFlash();
}

void ASmileawayCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (FlashTimeline.IsPlaying())
	{
		FlashTimeline.TickTimeline(DeltaTime);
	}
}

void ASmileawayCharacter::GetHit_Implementation(const FVector& ImpactPoint, FHitData HitData, AActor* Hitter)
{
	if (ActionState == EActionState::Dead) return;
	
	if (HitEffect)
	{
		const FRotator EffectRotation = (Hitter->GetActorLocation() - GetActorLocation()).Rotation();
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, HitEffect, ImpactPoint, EffectRotation);
	}
	if (HitSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, HitSound, ImpactPoint);
	}
	
	PlayHitFlashEffect();
	
	if (HitData.LaunchVelocity != FVector::ZeroVector)
	{
		GetController()->StopMovement();
		LaunchCharacter(HitData.LaunchVelocity * Stats->GetStat(EStats::KnockbackMultiplier), true, true);
	}
	
	Stats->TakeDamage(HitData.Damage);
	if (!Stats->IsAlive())
	{
		OnDeath();
	}
}

FGenericTeamId ASmileawayCharacter::GetGenericTeamId() const
{
	return FGenericTeamId(static_cast<uint8>(DamageFaction));
}

void ASmileawayCharacter::TriggerHitbox(FAttackData AttackData)
{
	const auto Location = GetActorLocation();
	const auto Forward = GetActorForwardVector();
	
	FCollisionShape Box = FCollisionShape::MakeBox(FVector{0.f, AttackData.HitboxHalfWidth, AttackData.HitboxHalfHeight});
	TArray<FHitResult> Hits;
	GetWorld()->SweepMultiByChannel(
		Hits,
		Location + Forward * AttackData.RangeMin,
		Location + Forward * AttackData.RangeMax,
		GetActorRotation().Quaternion(),
		ECC_HitInteractable,
		Box
	);
	// TArray<AActor*> ActorsToIgnore = {this};
	// UKismetSystemLibrary::BoxTraceMulti(
	// 	this,
	// 	Location + Forward * AttackData.RangeMin,
	// 	Location + Forward * AttackData.RangeMax,
	// 	Box.GetBox(),
	// 	GetActorRotation(),
	// 	UEngineTypes::ConvertToTraceType(ECC_HitInteractable),
	// 	false,
	// 	ActorsToIgnore,
	// 	EDrawDebugTrace::ForDuration,
	// 	Hits,
	// 	true
	// );
	
	for (const FHitResult& Hit : Hits)
	{
		AActor* HitActor = Hit.GetActor();
		if (IHitInterface::CanDamage(this, HitActor)
			&& HitActor->GetClass()->ImplementsInterface(UHitInterface::StaticClass()))
		{
			FHitData HitData {.Damage = AttackData.AttackMultiplier * Stats->GetAttack()};
			
			if (AttackData.PushbackForce > 0.f)
			{
				FVector Direction = (HitActor->GetActorLocation() - GetActorLocation()).GetSafeNormal();
				Direction.Z = 0.f;

				HitData.LaunchVelocity = Direction * AttackData.PushbackForce;
			}
			if (AttackData.LiftUpForce > 0.f)
			{
				HitData.LaunchVelocity.Z = AttackData.LiftUpForce;
			}
			
			IHitInterface::Execute_GetHit(HitActor, Hit.ImpactPoint, HitData, this);
		}
	}
}

void ASmileawayCharacter::GainExperience(int32 Experience)
{
	LevelingComponent->GainExperience(Experience);
}

void ASmileawayCharacter::Attack()
{
}

void ASmileawayCharacter::AttackEnd()
{
}

void ASmileawayCharacter::Dodge()
{
}

void ASmileawayCharacter::OnDeath()
{
	ActionState = EActionState::Dead;
	
	if (Controller)
	{
		Controller->UnPossess();
	}

	if (GetMesh())
	{
		if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
		{
			AnimInstance->StopAllMontages(0.f);
		}
	}
}

void ASmileawayCharacter::PlayMontageSection(UAnimMontage* Montage, const FName& SectionName) const
{
	if (Montage)
	{
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		AnimInstance->Montage_Play(Montage);
		AnimInstance->Montage_JumpToSection(SectionName, Montage);
	}
}

void ASmileawayCharacter::PlayMontageSection(UAnimMontage* Montage, int32 SectionIndex) const
{
	if (Montage)
	{
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		AnimInstance->Montage_Play(Montage);
		
		const FName& SectionName = Montage->CompositeSections[SectionIndex].SectionName;
		AnimInstance->Montage_JumpToSection(SectionName);
	}
}

void ASmileawayCharacter::PlayRandomMontageSection(UAnimMontage* Montage) const
{
	if (Montage)
	{
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		AnimInstance->Montage_Play(Montage);
		
		const int32 SectionIndex = FMath::RandRange(0, Montage->CompositeSections.Num() - 1);
		const FName& SectionName = Montage->CompositeSections[SectionIndex].SectionName;
		AnimInstance->Montage_JumpToSection(SectionName);
	}
}

void ASmileawayCharacter::PlayHitFlashEffect()
{
	if (HitFlashMaterialInstance && GetMesh())
	{
		GetMesh()->SetOverlayMaterial(HitFlashMaterialInstance);
		FlashTimeline.PlayFromStart();
	}
}

void ASmileawayCharacter::UpdateMovementSpeed(float NewSpeed)
{
	GetCharacterMovement()->MaxWalkSpeed = NewSpeed;
}

void ASmileawayCharacter::HandleFlashTimelineProgress(float Value)
{
	if (HitFlashMaterialInstance)
	{
		HitFlashMaterialInstance->SetScalarParameterValue("FlashIntensity", Value);
	}
}

void ASmileawayCharacter::HandleFlashTimelineFinished()
{
	if (GetMesh())
	{
		// Optimization: Strip the material off entirely so the GPU ignores the pass completely
		GetMesh()->SetOverlayMaterial(nullptr);
	}
}

void ASmileawayCharacter::InitializeHitFlash()
{
	if (HitFlashOverlay)
	{
		HitFlashMaterialInstance = UMaterialInstanceDynamic::Create(HitFlashOverlay, this);
		if (HitFlashCurve)
		{
			FOnTimelineFloat ProgressDelegate;
			ProgressDelegate.BindUFunction(this, FName("HandleFlashTimelineProgress"));

			FOnTimelineEvent FinishedDelegate;
			FinishedDelegate.BindUFunction(this, FName("HandleFlashTimelineFinished"));

			FlashTimeline.AddInterpFloat(HitFlashCurve, ProgressDelegate);

			FlashTimeline.SetTimelineFinishedFunc(FinishedDelegate);
		}
	}
}

