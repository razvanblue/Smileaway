#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Smileaway/DataAssets/RewardPoolData.h"
#include "RewardOptionWidget.generated.h"

class UBorder;
class UButton;
class UImage;
class UTextBlock;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRewardSelected, URewardOptionWidget*, SelectedOption);

/**
 * 
 */
UCLASS()
class SMILEAWAY_API URewardOptionWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION()
	virtual void NativeConstruct() override;
	
	UFUNCTION()
	void SetHighlight(bool bEnabled);
	
	void InitializeReward(FRewardEntry* InReward);
	
	FRewardEntry* Reward = nullptr;
	
	UPROPERTY(BlueprintAssignable)
	FOnRewardSelected OnRewardSelected;
	
protected:
	
	UFUNCTION()
	void SelectReward();
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> RewardButton;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UBorder> RewardBorder;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> RewardIcon;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> RewardNameText;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> RewardDescriptionText;
	
private:
	
	FButtonStyle NormalButtonStyle;
	FButtonStyle HighlightedButtonStyle;
};
