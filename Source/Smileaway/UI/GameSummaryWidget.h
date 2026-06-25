// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Smileaway/GameData/GameSummary.h"
#include "GameSummaryWidget.generated.h"

class UTextBlock;

/**
 * 
 */
UCLASS()
class SMILEAWAY_API UGameSummaryWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	
	UFUNCTION(BlueprintCallable)
	void SetSummary(const FGameSummary Summary);

protected:
	
	UPROPERTY(meta=(BindWidgetOptional))
	TObjectPtr<UTextBlock> SurvivalTimeText;

	UPROPERTY(meta=(BindWidgetOptional))
	TObjectPtr<UTextBlock> WaveReachedText;

	UPROPERTY(meta=(BindWidgetOptional))
	TObjectPtr<UTextBlock> EnemiesDefeatedText;

	UPROPERTY(meta=(BindWidgetOptional))
	TObjectPtr<UTextBlock> ExperienceEarnedText;

	UPROPERTY(meta=(BindWidgetOptional))
	TObjectPtr<UTextBlock> BuffCountText;
};
