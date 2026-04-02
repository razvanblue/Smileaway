// Fill out your copyright notice in the Description page of Project Settings.


#include "DodgeNotifyState.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

void UDodgeNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration,
                                    const FAnimNotifyEventReference& EventReference)
{
	if (!MeshComp) return;

    ACharacter* Character = Cast<ACharacter>(MeshComp->GetOwner());
    if (!Character) return;

    UCharacterMovementComponent* MovementComponent = Character->GetCharacterMovement();
    if (!MovementComponent) return;
	
	OriginalSpeed = MovementComponent->MaxWalkSpeed;
	OriginalRotationRate = MovementComponent->RotationRate;
	
	MovementComponent->MaxWalkSpeed *= SpeedMultiplier;
	
	FRotator NewRotationRate{ MovementComponent->RotationRate };
	NewRotationRate.Yaw *= TurnRateMultiplier; // Only affect yaw
	MovementComponent->RotationRate = NewRotationRate;
}

void UDodgeNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	
	if (!MeshComp) return;

	ACharacter* Character = Cast<ACharacter>(MeshComp->GetOwner());
	if (!Character) return;

	UCharacterMovementComponent* MovementComponent = Character->GetCharacterMovement();
	if (!MovementComponent) return;
	
	MovementComponent->MaxWalkSpeed = OriginalSpeed;
	MovementComponent->RotationRate = OriginalRotationRate;
}
