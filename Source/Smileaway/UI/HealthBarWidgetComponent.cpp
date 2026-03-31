// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthBarWidgetComponent.h"
#include "HealthBarWidget.h"

void UHealthBarWidgetComponent::InitWidget()
{
	Super::InitWidget();

	HealthBarWidget = Cast<UHealthBarWidget>(GetUserWidgetObject());
}

void UHealthBarWidgetComponent::SetHealthPercent(float Percent)
{
	if (HealthBarWidget)
	{
		HealthBarWidget->HealthBar->SetPercent(Percent);
	}
}