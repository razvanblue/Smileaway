// Fill out your copyright notice in the Description page of Project Settings.


#include "IFrameNotifyState.h"

#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"
#include "Smileaway/Smileaway.h"

void UIFrameNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                     float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	if (!MeshComp) return;

	ACharacter* Character = Cast<ACharacter>(MeshComp->GetOwner());
	if (!Character) return;
	
	Character->GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_HitInteractable, ECR_Ignore);
	Character->GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Projectile, ECR_Ignore);
}

void UIFrameNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	if (!MeshComp) return;

	ACharacter* Character = Cast<ACharacter>(MeshComp->GetOwner());
	if (!Character) return;
	
	Character->GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_HitInteractable, ECR_Overlap);
	Character->GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Projectile, ECR_Overlap);
}
