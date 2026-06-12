// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerHUD.generated.h"

class UHealthBarWidget;
class USKillBase;
class USkillData;
class UWrapBox;
class USkillIcon;
class UStatusEffect;
class UStatusEffectIcon;
class UProgressBar;
class UTextBlock;

/**
 * @class UPlayerHUD
 * @brief Handles the player HUD, including health and experience UI elements.
 *
 * UPlayerHUD extends UUserWidget to provide functionality for displaying
 * and updating the player's health and experience in the HUD.
 */
UCLASS()
class SMILEAWAY_API UPlayerHUD : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;
	/* ---------------- Health ---------------- */

	UFUNCTION(BlueprintCallable, Category = "HUD|Health")
	void SetHealth(float CurrentHealth, float MaxHealth);

	/* ---------------- Experience ---------------- */

	UFUNCTION(BlueprintCallable, Category = "HUD|Experience")
	void SetLevel(int32 NewLevel);

	UFUNCTION(BlueprintCallable, Category = "HUD|Experience")
	void SetExperience(int32 CurrentXP, int32 XPToNextLevel);
	
	/* ---------------- Game State ---------------- */
	
	UFUNCTION(BlueprintCallable, Category = "HUD|Game State")
	void SetRemainingEnemies(int32 NewRemainingEnemies);
	
	/* ---------------- Skills ---------------- */
	
	UFUNCTION(BlueprintCallable, Category = "HUD|Skills")
	void SetSkillIcon(int32 SkillIndex, USkillBase* const Skill, const USkillData* SkillData);
	
	UFUNCTION(BlueprintCallable, Category = "HUD|Skills")
	void EnterSkillCooldown(int32 SkillIndex);
	
	UFUNCTION(BlueprintCallable, Category = "HUD|Skills")
	void ResetSkillCooldown(int32 SkillIndex);
	
	/* ---------------- Status Effects ---------------- */
	
	UFUNCTION(BlueprintCallable, Category = "HUD|Status Effects")
	void AddStatusEffect(const UStatusEffect* Effect, float Duration);
	
	UFUNCTION(BlueprintCallable, Category = "HUD|Status Effects")
	void RemoveStatusEffect(const UStatusEffect* Effect);
	
protected:

	/* ---------------- Health Widgets ---------------- */

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UHealthBarWidget> HealthBar;

	/* ---------------- Experience Widgets ---------------- */

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressBar> LevelProgressBar;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> LevelText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> CurrentExperienceText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> RequiredExperienceText;
	
	/* ---------------- Experience Widgets ---------------- */
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> RemainingEnemiesText;
	
	/* ---------------- Skill Widgets ---------------- */
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USkillIcon> SkillIcon_0;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USkillIcon> SkillIcon_1;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USkillIcon> SkillIcon_2;
	
	/* ---------------- Container Widgets ---------------- */
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UWrapBox> StatusEffectsContainer;

	UPROPERTY(EditDefaultsOnly, Category = "HUD|Status Effects")
	TSubclassOf<UStatusEffectIcon> StatusEffectIconClass;
	
	UPROPERTY()
	TMap<const UStatusEffect*, TObjectPtr<UStatusEffectIcon>> ActiveStatusEffects;
	
	UPROPERTY()
	TArray<TObjectPtr<USkillIcon>> SkillIcons;
};
