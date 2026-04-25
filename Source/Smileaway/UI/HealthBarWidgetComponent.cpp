// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthBarWidgetComponent.h"
#include "HealthBarWidget.h"

void UHealthBarWidgetComponent::InitWidget()
{
	Super::InitWidget();

	HealthBarWidget = Cast<UHealthBarWidget>(GetUserWidgetObject());
}

void UHealthBarWidgetComponent::SetHealth(float NewHealth, float MaxHealth)
{
	if (HealthBarWidget)
	{
		HealthBarWidget->HealthBar->SetPercent(NewHealth / MaxHealth);
	}
}