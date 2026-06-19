// Fill out your copyright notice in the Description page of Project Settings.


#include "LockRotationNotifyState.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

void ULockRotationNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                           float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	ACharacter* Character = Cast<ACharacter>(MeshComp->GetOwner());
	if (!Character) return;

	
	bUseControllerRotationYaw = Character->bUseControllerRotationYaw;
	bOrientRotationToMovement = Character->GetCharacterMovement()->bOrientRotationToMovement;
	
	Character->bUseControllerRotationYaw = false;
	Character->GetCharacterMovement()->bOrientRotationToMovement = false;
	
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
}

void ULockRotationNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	ACharacter* Character = Cast<ACharacter>(MeshComp->GetOwner());
	if (!Character) return;

	Character->bUseControllerRotationYaw = bUseControllerRotationYaw;
	Character->GetCharacterMovement()->bOrientRotationToMovement = bOrientRotationToMovement;
	
	Super::NotifyEnd(MeshComp, Animation, EventReference);
}
