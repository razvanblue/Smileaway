// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterStatsWidget.h"

#include "Components/GridPanel.h"
#include "Components/GridSlot.h"
#include "Components/TextBlock.h"
#include "Smileaway/Components/CharacterStats.h"

void UCharacterStatsWidget::PopulateStatsGrid(UCharacterStats* InCharacterStats)
{
	if (!InCharacterStats || !StatsGrid)
	{
		return;
	}
	
	CharacterStats = InCharacterStats;

	int32 Row = 2;

	const UTextBlock* Template = Cast<UTextBlock>(StatsGrid->GetChildAt(1));
	
	for (int32 StatIndex = 0; StatIndex < static_cast<int32>(EStats::MAX); ++StatIndex)
	{
		const EStats Stat = static_cast<EStats>(StatIndex);

		const float FinalValue = CharacterStats->GetStat(Stat);
		const float BaseValue = CharacterStats->GetBaseStat(Stat);

		UTextBlock* NameText = CreateStyledText(Template, GetStatText(static_cast<uint8>(Stat)));
		UTextBlock* FinalText = CreateStyledText(Template, FText::AsNumber(FinalValue));
		UTextBlock* BaseText = CreateStyledText(Template, FText::Format(FText::FromString("({0})"), BaseValue));

		StatsGrid->AddChildToGrid(NameText, Row, 0);
		StatsGrid->AddChildToGrid(FinalText, Row, 1);
		StatsGrid->AddChildToGrid(BaseText, Row, 2);
		
		BaseStats[StatIndex] = BaseText;
		FinalStats[StatIndex] = FinalText;

		++Row;
	}
}

void UCharacterStatsWidget::UpdateStats()
{
	if (!CharacterStats || !IsValid(FinalStats[0]))
	{
		return;
	}
	
	int32 Row = 2;
	
	for (int32 StatIndex = 0; StatIndex < static_cast<int32>(EStats::MAX); ++StatIndex)
	{
		const EStats Stat = static_cast<EStats>(StatIndex);

		const float FinalValue = CharacterStats->GetStat(Stat);
		FinalStats[StatIndex]->SetText(FText::AsNumber(FinalValue));
		
		const float BaseValue = CharacterStats->GetBaseStat(Stat);
		BaseStats[StatIndex]->SetText(FText::Format(FText::FromString("({0})"), BaseValue));

		++Row;
	}
}

UTextBlock* UCharacterStatsWidget::CreateStyledText(
	const UTextBlock* Template,
	const FText& Text)
{
	UTextBlock* NewText = DuplicateObject<UTextBlock>(Template, this);

	NewText->SetText(Text);

	return NewText;
}

const FText UCharacterStatsWidget::GetStatText(uint8 Stat)
{
	static const FText StatNames[] = {FText::FromString("Max HP"), FText::FromString("Attack"), FText::FromString("Speed"), FText::FromString("Knockback")};
	
	return StatNames[Stat];
}
