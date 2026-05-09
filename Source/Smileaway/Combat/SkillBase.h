// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "SkillBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FEnterCooldownDelegate, float, InCooldownDuration);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FResetCooldownDelegate);

/**
 * 
 */
UCLASS(Blueprintable)
class SMILEAWAY_API USkillBase : public UObject
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float Cooldown = 3.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UAnimMontage> SkillMontage;

	UPROPERTY(BlueprintAssignable)
	FEnterCooldownDelegate OnEnterCooldown;
	
	UPROPERTY(BlueprintAssignable)
	FResetCooldownDelegate OnResetCooldown;
	
	virtual void Activate(class ACharacter* Character);

	bool CanActivate() const;

	void Cleanup();
	
	void EnterCooldown();
	
	void ResetCooldown();

protected:
	
	bool bOnCooldown = false;
	FTimerHandle CooldownTimer;
};
