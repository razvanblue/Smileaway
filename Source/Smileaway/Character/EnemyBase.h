// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SmileawayCharacter.h"
#include "EnemyBase.generated.h"

/**
 * 
 */
UCLASS()
class SMILEAWAY_API AEnemyBase : public ASmileawayCharacter
{
	GENERATED_BODY()
	
protected:
	virtual void OnDeath() override;
};

