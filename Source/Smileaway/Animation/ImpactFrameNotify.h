// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AnimationData.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "ImpactFrameNotify.generated.h"

/**
 * 
 */
UCLASS()
class SMILEAWAY_API UImpactFrameNotify : public UAnimNotify
{
	GENERATED_BODY()
	
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FAttackData AttackData;
};
