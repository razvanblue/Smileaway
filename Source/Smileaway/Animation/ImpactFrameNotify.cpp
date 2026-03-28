// Fill out your copyright notice in the Description page of Project Settings.


#include "ImpactFrameNotify.h"
#include "Smileaway/Character/SmileawayCharacter.h"

void UImpactFrameNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	// Super::Notify(MeshComp, Animation, EventReference);
	
	if (const auto Character = Cast<ASmileawayCharacter>(MeshComp->GetOwner()))
	{
		Character->TriggerHitbox(AttackData);
	}
}