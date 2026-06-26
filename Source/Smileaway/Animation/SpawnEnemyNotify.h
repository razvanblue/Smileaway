// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "SpawnEnemyNotify.generated.h"

class AEnemyBase;
/**
 * 
 */
UCLASS()
class SMILEAWAY_API USpawnEnemyNotify : public UAnimNotify
{
	GENERATED_BODY()
	
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
	
public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Spawn")
	TSubclassOf<AEnemyBase> EnemyClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Spawn")
	FVector SpawnLocationRelative = FVector(200.f, 0.f, 0.f);
};
