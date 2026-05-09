// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillBase.h"

#include "GameFramework/Character.h"


void USkillBase::Activate(class ACharacter* Character)
{
	if (!Character || !CanActivate()) return;

	if (SkillMontage)
	{
		Character->PlayAnimMontage(SkillMontage);
	}

	EnterCooldown();
}


bool USkillBase::CanActivate() const
{
	return !bOnCooldown;
}

void USkillBase::Cleanup()
{
	OnEnterCooldown.Clear();
	OnResetCooldown.Clear();
	
	if (UWorld* World = GetWorld(); CooldownTimer.IsValid() && World)
	{
		World->GetTimerManager().ClearTimer(CooldownTimer);
	}
}


void USkillBase::EnterCooldown()
{
	bOnCooldown = true;
	
	if (UWorld* const World = GetWorld())
	{
		World->GetTimerManager().SetTimer(CooldownTimer, this, &ThisClass::ResetCooldown, Cooldown);
	}
	
	OnEnterCooldown.Broadcast(Cooldown);
}


void USkillBase::ResetCooldown()
{
	if (UWorld* const World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(CooldownTimer);
	}
	
	CooldownTimer.Invalidate();
	bOnCooldown = false;
	
	OnResetCooldown.Broadcast();
}
