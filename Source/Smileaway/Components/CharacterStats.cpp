// Fill out your copyright notice in the Description page of Project Settings.


#include "Smileaway/Components/CharacterStats.h"

// Sets default values for this component's properties
UCharacterStats::UCharacterStats()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


void UCharacterStats::BeginPlay()
{
	Super::BeginPlay();
}


void UCharacterStats::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}


void UCharacterStats::TakeDamage(float DamageAmount)
{
	if (CurrentHealth > 0.f && DamageAmount > 0.f)
	{
		CurrentHealth = FMath::Max(0.f, CurrentHealth - DamageAmount);
		OnHealthChanged.Broadcast(GetHealthPercentage());
	}
}


float UCharacterStats::GetHealthPercentage()
{
	return CurrentHealth / MaxHealth;
}

double UCharacterStats::GetAttack()
{
	return Attack;
}


bool UCharacterStats::IsAlive()
{
	return CurrentHealth > 0.f;
}

