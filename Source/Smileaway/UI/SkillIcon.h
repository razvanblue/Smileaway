// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SkillIcon.generated.h"

class UImage;
class UTextBlock;
class UMaterialInstanceDynamic;
class USkillData;

/**
 * 
 */
UCLASS()
class SMILEAWAY_API USkillIcon : public UUserWidget
{
	GENERATED_BODY()
	
public:
	
	USkillIcon(const FObjectInitializer& ObjectInitializer);
	
	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintCallable, Category = "Skill")
	void EnterCooldown(float InCooldownDuration);
	
	UFUNCTION(BlueprintCallable, Category = "Skill")
	void ResetCooldown();
	
	/**
	 * 
	 * @param Skill Skill data
	 * @param InCooldownDuration Visual cooldown to apply to the widget, defaults to 0 / not on cooldown
	 */
	UFUNCTION(BlueprintCallable, Category = "Skill")
	void SetSkill(const USkillData* Skill, float InCooldownDuration = 0.f);

	// Updates the remaining time. Called by NativeTick.
	UFUNCTION(BlueprintCallable, Category = "Skill")
	void UpdateRemainingTime(float NewRemainingTime);

protected:
	
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> SkillIcon;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> DurationText;

private:
	
	UPROPERTY()
	TObjectPtr<const USkillData> SkillData;
	
	UPROPERTY()
	TObjectPtr<UMaterialInstanceDynamic> OverlayMaterialInstance;
	
	float CooldownResetTime;
	float CooldownDuration;
};
