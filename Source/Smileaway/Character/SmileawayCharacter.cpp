// Fill out your copyright notice in the Description page of Project Settings.


#include "SmileawayCharacter.h"

// Sets default values
ASmileawayCharacter::ASmileawayCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASmileawayCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASmileawayCharacter::Attack()
{
}

void ASmileawayCharacter::AttackEnd()
{
}

void ASmileawayCharacter::OnDeath()
{
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


