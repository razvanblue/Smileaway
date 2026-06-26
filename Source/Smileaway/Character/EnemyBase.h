// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NativeGameplayTags.h"
#include "SmileawayCharacter.h"
#include "EnemyBase.generated.h"

UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Enemy_Add);

/**
 * 
 */
UCLASS()
class SMILEAWAY_API AEnemyBase : public ASmileawayCharacter
{
	GENERATED_BODY()

public:
	AEnemyBase();

	void Attack(AActor* Target);
	
protected:
	virtual void Attack() override;

	virtual void OnDeath() override;
};

