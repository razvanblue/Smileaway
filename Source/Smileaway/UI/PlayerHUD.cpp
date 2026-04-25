#include "PlayerHUD.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UPlayerHUD::SetHealth(float CurrentHealth, float MaxHealth)
{
	if (!HealthBar || !CurrentHealthText || !MaxHealthText)
	{
		return;
	}
	
	HealthBar->SetPercent(CurrentHealth / MaxHealth);
	CurrentHealthText->SetText(FText::AsNumber(FMath::RoundToInt(CurrentHealth)));
	MaxHealthText->SetText(FText::AsNumber(FMath::RoundToInt(MaxHealth)));
}

void UPlayerHUD::SetLevel(int32 NewLevel)
{
	if (!LevelText) return;
	
	LevelText->SetText(FText::AsNumber(NewLevel));
}

void UPlayerHUD::SetExperience(int32 CurrentXP, int32 XPToNextLevel)
{
	if (!LevelProgressBar || !CurrentExperienceText || !RequiredExperienceText)
	{
		return;
	}
	
	LevelProgressBar->SetPercent(CurrentXP / (float)XPToNextLevel);
	CurrentExperienceText->SetText(FText::AsNumber(CurrentXP));
	RequiredExperienceText->SetText(FText::AsNumber(XPToNextLevel));
}
