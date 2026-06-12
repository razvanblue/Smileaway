// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthBarWidget.h"

#include "Components/TextBlock.h"

void UHealthBarWidget::SetHealthPercent(float NewHealth, float MaxHealth)
{
	TargetHealth = NewHealth / MaxHealth;
	
	if (CurrentHealthText)
	{
		CurrentHealthText->SetText(FText::AsNumber(FMath::RoundToInt(NewHealth)));
	}
	
	if (MaxHealthText)
	{
		MaxHealthText->SetText(FText::AsNumber(FMath::RoundToInt(MaxHealth)));
	}
}

void UHealthBarWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	
	float DisplayedHealthPercent = FMath::FInterpTo(
		HealthBar->GetPercent(),
		TargetHealth,
		InDeltaTime,
		InterpolationSpeed);

	HealthBar->SetPercent(DisplayedHealthPercent);
}
