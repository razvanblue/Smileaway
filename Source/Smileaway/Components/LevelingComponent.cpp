// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelingComponent.h"

ULevelingComponent::ULevelingComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


void ULevelingComponent::BeginPlay()
{
	Super::BeginPlay();
	
	RecalculateXPThreshold();
	GainExperience(0);
}


void ULevelingComponent::LevelUp()
{
	while (CurrentXP >= XPToNextLevel)
	{
		CurrentXP -= XPToNextLevel;
		++Level;
		RecalculateXPThreshold();
	}
	
	OnLevelUp.Broadcast(Level);
}


void ULevelingComponent::RecalculateXPThreshold()
{
	XPToNextLevel = FMath::RoundToInt(XPCurve->GetFloatValue(Level));
}


void ULevelingComponent::GainExperience(int32 Amount)
{
	CurrentXP += Amount;
	
	if (CurrentXP >= XPToNextLevel)
	{
		LevelUp();
	}
}

