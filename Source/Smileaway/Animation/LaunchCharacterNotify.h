// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "LaunchCharacterNotify.generated.h"

/**
 * 
 */
UCLASS()
class SMILEAWAY_API ULaunchCharacterNotify : public UAnimNotify
{
	GENERATED_BODY()
	
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Launch")
	FVector LaunchVelocity = FVector(1000.f, 0.f, 300.f);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Launch")
	bool bIsControllerRelative = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Launch")
	bool bXYOverride = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Launch")
	bool bZOverride = true;

};
