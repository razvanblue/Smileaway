#include "PlayerHUD.h"

#include "SkillIcon.h"
#include "StatusEffectIcon.h"
#include "Components/WrapBox.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Smileaway/DataAssets/StatusEffect.h"
#include "Smileaway/DataAssets/SkillData.h"
#include "Smileaway/Combat/SkillBase.h"

void UPlayerHUD::NativeConstruct()
{
	Super::NativeConstruct();
	
	SkillIcons = {SkillIcon_0, SkillIcon_1, SkillIcon_2};
}

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

void UPlayerHUD::SetRemainingEnemies(int32 NewRemainingEnemies)
{
	if (!RemainingEnemiesText) return;
	
	RemainingEnemiesText->SetText(FText::AsNumber(NewRemainingEnemies));
}

void UPlayerHUD::SetSkillIcon(int32 SkillIndex, USkillBase* const Skill, const USkillData* SkillData)
{
	if (SkillIcons.IsValidIndex(SkillIndex) == false)
	{
		return;
	}
	
	SkillIcons[SkillIndex]->SetSkill(SkillData);
	if (Skill)
	{
		Skill->OnEnterCooldown.AddDynamic(SkillIcons[SkillIndex], &USkillIcon::EnterCooldown);
		Skill->OnResetCooldown.AddDynamic(SkillIcons[SkillIndex], &USkillIcon::ResetCooldown);
	}
}

void UPlayerHUD::EnterSkillCooldown(int32 SkillIndex)
{
}

void UPlayerHUD::ResetSkillCooldown(int32 SkillIndex)
{
}

void UPlayerHUD::AddStatusEffect(const UStatusEffect* Effect, float Duration)
{
	if (!IsValid(Effect) || !StatusEffectsContainer || !StatusEffectIconClass)
	{
		return;
	}
	
	if (auto* ExistingIcon = ActiveStatusEffects.Find(Effect))
	{
		(*ExistingIcon)->InitializeStatusEffect(Effect, Duration);
		return;
	}
	
	UStatusEffectIcon* NewIcon = CreateWidget<UStatusEffectIcon>(StatusEffectsContainer, StatusEffectIconClass);
	ActiveStatusEffects.Add(Effect, NewIcon);
	StatusEffectsContainer->AddChildToWrapBox(NewIcon);
	NewIcon->InitializeStatusEffect(Effect, Duration);
	
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, FString::Printf(TEXT("Added status effect: %s"), *Effect->GetName()));
}

void UPlayerHUD::RemoveStatusEffect(const UStatusEffect* Effect)
{
	TObjectPtr<UStatusEffectIcon> ExistingIcon;
	if (ActiveStatusEffects.RemoveAndCopyValue(Effect, ExistingIcon))
	{
		ExistingIcon->RemoveFromParent();
	}
}
