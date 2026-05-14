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

	// Updates the remaining time. Called by NativeTick.
	UFUNCTION(BlueprintCallable, Category = "Status Effect")
	void UpdateRemainingTime(float NewRemainingTime);
	
	UFUNCTION(BlueprintCallable, Category = "Status Effect")
	void AddStack(float Duration);
	
	UFUNCTION(BlueprintCallable, Category = "Status Effect")
	void RemoveStack(int32 Count = 1);
	
	int32 GetStackCount() const { return StackCount; }

protected:
	
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> EffectIcon;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> DurationText;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> StackCountText;

private:
	
	UPROPERTY()
	TObjectPtr<const UStatusEffect> StatusEffect;
	
	UPROPERTY()
	TObjectPtr<UMaterialInstanceDynamic> OverlayMaterialInstance;
	
	TArray<float> ExpirationTimes;
	float TotalDuration;
	
	int32 StackCount = 1;
};
