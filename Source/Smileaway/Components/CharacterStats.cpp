// Fill out your copyright notice in the Description page of Project Settings.


#include "Smileaway/Components/CharacterStats.h"


UCharacterStats::UCharacterStats()
{
	PrimaryComponentTick.bCanEverTick = true;
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
}


void UCharacterStats::TakeDamage(float DamageAmount)
{
	if (Health > 0.f && DamageAmount > 0.f)
	{
		Health = FMath::Max(0.f, Health - DamageAmount);
		OnHealthChanged.Broadcast(GetHealthPercentage());
	}
}

void UCharacterStats::AddModifier(const FStatModifier& Modifier)
{
	StatModifiers.Add(Modifier);
}


float UCharacterStats::GetHealthPercentage()
{
	return Health / FinalStats[EStats::MaxHP];
}


double UCharacterStats::GetAttack()
{
	return FinalStats[EStats::Attack];
}


bool UCharacterStats::IsAlive()
{
	return Health > 0.f;
}

