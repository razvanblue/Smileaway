// Fill out your copyright notice in the Description page of Project Settings.

#include "GameSummaryWidget.h"
#include "Components/TextBlock.h"

void UGameSummaryWidget::SetSummary(FGameSummary Summary)
{
	if (SurvivalTimeText)
	{
		const int32 TotalSeconds =
			FMath::RoundToInt(Summary.SurvivalTime);

		const int32 Minutes = TotalSeconds / 60;
		const int32 Seconds = TotalSeconds % 60;

		SurvivalTimeText->SetText(
			FText::FromString(
				FString::Printf(TEXT("%02d:%02d"), Minutes, Seconds)));
	}

	if (WaveReachedText)
	{
		WaveReachedText->SetText(
			FText::AsNumber(Summary.WaveReached));
	}

	if (EnemiesDefeatedText)
	{
		EnemiesDefeatedText->SetText(
			FText::AsNumber(Summary.EnemiesDefeated));
	}

	if (ExperienceEarnedText)
	{
		ExperienceEarnedText->SetText(
			FText::AsNumber(Summary.ExperienceEarned));
	}

	if (BuffCountText)
	{
		BuffCountText->SetText(
			FText::AsNumber(Summary.BuffCount));
	}
}