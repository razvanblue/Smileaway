#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StatusEffectIcon.generated.h"

class UImage;
class UTextBlock;
class UMaterialInstanceDynamic;
class UStatusEffect;

/**
 * @brief Displays a single active status effect
 */
UCLASS()
class SMILEAWAY_API UStatusEffectIcon : public UUserWidget
{
	GENERATED_BODY()
	
public:
	
	UStatusEffectIcon(const FObjectInitializer& ObjectInitializer);
	
	virtual void NativeConstruct() override;

	// Initializes the widget from a status effect definition.
	UFUNCTION(BlueprintCallable, Category = "Status Effect")
	void InitializeStatusEffect(const UStatusEffect* InEffect, float InTotalDuration);

	// Updates the remaining time. Call whenever the effect timer changes.
	UFUNCTION(BlueprintCallable, Category = "Status Effect")
	void UpdateRemainingTime(float NewRemainingTime);

protected:
	
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> EffectIcon;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> DurationText;

private:
	
	UPROPERTY()
	TObjectPtr<const UStatusEffect> StatusEffect;
	
	UPROPERTY()
	TObjectPtr<UMaterialInstanceDynamic> OverlayMaterialInstance;
	
	float ExpirationTime;
	float TotalDuration;
};
