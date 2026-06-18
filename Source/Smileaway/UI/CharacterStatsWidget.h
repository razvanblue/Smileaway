// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Smileaway/DataAssets/CharacterStat.h"
#include "CharacterStatsWidget.generated.h"

class UCharacterStats;
class UGridPanel;
class UTextBlock;
/**
 * 
 */
UCLASS()
class SMILEAWAY_API UCharacterStatsWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	
	UFUNCTION(BlueprintCallable)
	void PopulateStatsGrid(UCharacterStats* InCharacterStats);

	UFUNCTION(BlueprintCallable)
	void UpdateStats();
	
protected:
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UGridPanel> StatsGrid;
	
private:
	
	UTextBlock* CreateStyledText(const UTextBlock* Template, const FText& Text);
	
	static const FText GetStatText(uint8 Stat);
	
	UPROPERTY(Transient)
	TObjectPtr<UCharacterStats> CharacterStats;
	
	// Stored for updating text
	TStatArray<UTextBlock*> BaseStats;
	TStatArray<UTextBlock*> FinalStats;
};
