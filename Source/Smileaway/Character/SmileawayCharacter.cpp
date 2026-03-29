// Fill out your copyright notice in the Description page of Project Settings.


#include "SmileawayCharacter.h"

#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Smileaway/Components/CharacterStats.h"

// Sets default values
ASmileawayCharacter::ASmileawayCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	Stats = CreateDefaultSubobject<UCharacterStats>(TEXT("Character Stats"));
	
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Overlap);
}


void ASmileawayCharacter::TriggerHitbox(FAttackData AttackData)
{
	const auto Location = GetActorLocation();
	const auto Forward = GetActorForwardVector();
	
	FCollisionShape Box = FCollisionShape::MakeBox(FVector{0.f, AttackData.HitboxHalfWidth, AttackData.HitboxHalfHeight});
	TArray<FHitResult> Hits;
	// GetWorld()->SweepMultiByChannel(
	// 	Hits,
	// 	Location + Forward * AttackData.RangeMin,
	// 	Location + Forward * AttackData.RangeMax,
	// 	GetActorRotation().Quaternion(),
	// 	ECC_GameTraceChannel1,
	// 	Box
	// );
	TArray<AActor*> ActorsToIgnore = {this};
	UKismetSystemLibrary::BoxTraceMulti(
		this,
		Location + Forward * AttackData.RangeMin,
		Location + Forward * AttackData.RangeMax,
		Box.GetBox(),
		GetActorRotation(),
		UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel1),
		false,
		ActorsToIgnore,
		EDrawDebugTrace::ForDuration,
		Hits,
		true
	);
	
	for (const FHitResult& Hit : Hits)
	{
		AActor* HitActor = Hit.GetActor();
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Orange, TEXT("Hit something!"));
		if (HitActor && HitActor->GetClass()->ImplementsInterface(UHitInterface::StaticClass()))
		{
			IHitInterface::Execute_GetHit(HitActor, Hit.ImpactPoint, AttackData.AttackMultiplier * Stats->GetAttack(), this);
		}
	}
}

// Called when the game starts or when spawned
void ASmileawayCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASmileawayCharacter::GetHit_Implementation(const FVector& ImpactPoint, double DamageAmount, AActor* Hitter)
{
	if (ActionState == EActionState::Dead) return;
	
	if (HitParticleSystem)
	{
		UGameplayStatics::SpawnEmitterAtLocation(this, HitParticleSystem, ImpactPoint);
	}
	if (HitSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, HitSound, ImpactPoint);
	}
	
	Stats->TakeDamage(DamageAmount);
	if (!Stats->IsAlive())
	{
		OnDeath();
	}
}

void ASmileawayCharacter::Attack()
{
}

void ASmileawayCharacter::AttackEnd()
{
}

void ASmileawayCharacter::OnDeath()
{
	ActionState = EActionState::Dead;
	
	Controller->UnPossess();
	
	if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
	{
		AnimInstance->StopAllMontages(0.f);
	}
	
	Destroy();
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


