// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthBarWidget.h"

void UHealthBarWidget::SetHealthPercent(float Percent)
{
	HealthBar->SetPercent(Percent);
}