// Fill out your copyright notice in the Description page of Project Settings.


#include "Smileaway/Components/CharacterStats.h"


UCharacterStats::UCharacterStats()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.TickInterval = 0.2f;
}

void UCharacterStats::BeginPlay()
{
	Super::BeginPlay();
	
	BaseStats = StatsConfig.Pack();
	FinalStats = BaseStats;
	Health = FinalStats[EStats::MaxHP];
}


void UCharacterStats::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	TickBuffs(DeltaTime);
}


void UCharacterStats::TakeDamage(float DamageAmount)
{
	if (Health > 0.f && DamageAmount > 0.f)
	{
		Health = FMath::Max(0.f, Health - DamageAmount);
		OnHealthChanged.Broadcast(Health, FinalStats[EStats::MaxHP]);
	}
}


void UCharacterStats::AddStatusEffect(const UStatusEffect* Effect, float InDuration)
{
	if (Effect == nullptr) return;
	
	const float FinalDuration = InDuration == 0.f ? Effect->Duration : InDuration;
	ActiveEffects.Add({Effect, FinalDuration});
	
	CalculateFinalStats();
	
	OnStatusEffectApplied.Broadcast(Effect, FinalDuration);
}


void UCharacterStats::CalculateFinalStats()
{
	std::array<float, StatCount> AdditiveModifiers;
	std::array<float, StatCount> MultiplicativeModifiers;
	AdditiveModifiers.fill(0.f);
	MultiplicativeModifiers.fill(1.f);
	const float OldMaxHP = FinalStats[EStats::MaxHP];
	
	for (const auto& Effect : ActiveEffects)
	{
		for (const auto& Modifier : Effect.Data->Modifiers)
		{
			if (Modifier.Type == EModifierType::Additive)
			{
				AdditiveModifiers[static_cast<uint8>(Modifier.Stat)] += Modifier.Value;
			}
			else
			{
				MultiplicativeModifiers[static_cast<uint8>(Modifier.Stat)] += Modifier.Value;
			}
		}
	}
	for (uint8 i = 0; i < StatCount; ++i)
	{
		FinalStats[i] = (BaseStats[i] + AdditiveModifiers[i]) * MultiplicativeModifiers[i];
	}
	
	if (FinalStats[EStats::MaxHP] != OldMaxHP)
	{
		Health += FinalStats[EStats::MaxHP] - OldMaxHP;
		OnHealthChanged.Broadcast(Health, FinalStats[EStats::MaxHP]);
	}
}


void UCharacterStats::TickBuffs(float DeltaTime)
{
	bool bShouldUpdateStats = false;
	for (int32 i = ActiveEffects.Num() - 1; i >= 0; --i)
	{
		auto& Effect = ActiveEffects[i];

		if (Effect.Data->Duration != INFINITE_STATUS_DURATION)
		{
			Effect.RemainingTime -= DeltaTime;

			if (Effect.RemainingTime <= 0.f)
			{
				OnStatusEffectRemoved.Broadcast(Effect.Data);
				ActiveEffects.RemoveAtSwap(i);
				bShouldUpdateStats = true;
			}
		}
	}
	
	if (bShouldUpdateStats)
	{
		CalculateFinalStats();
	}
}


float UCharacterStats::GetAttack()
{
	return FinalStats[EStats::Attack];
}

float UCharacterStats::GetHealth()
{
	return Health;
}


float UCharacterStats::GetHealthPercentage()
{
	return Health / FinalStats[EStats::MaxHP];
}


float UCharacterStats::GetStat(EStats Stat)
{
	return FinalStats[Stat];
}

float UCharacterStats::GetBaseStat(EStats Stat)
{
	return BaseStats[Stat];
}


bool UCharacterStats::IsAlive()
{
	return Health > 0.f;
}

