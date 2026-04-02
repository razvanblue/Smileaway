// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "DodgeNotifyState.generated.h"

/**
 * 
 */
UCLASS()
class SMILEAWAY_API UDodgeNotifyState : public UAnimNotifyState
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Speed")
	float SpeedMultiplier = 2.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Speed")
	float TurnRateMultiplier =0.2f;
	
private:
	virtual void NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, const FAnimNotifyEventReference& EventReference) override;
	
	float OriginalSpeed = 0.f;
	FRotator OriginalRotationRate = FRotator::ZeroRotator;
};
