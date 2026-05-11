#pragma once


#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RewardMenuWidget.generated.h"

class UButton;
class UHorizontalBox;
class URewardOptionWidget;
struct FRewardEntry;

DECLARE_MULTICAST_DELEGATE_OneParam(FRewardConfirmedDelegate, FRewardEntry*);

/**
 * 
 */
UCLASS()
class SMILEAWAY_API URewardMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	
	virtual void NativeConstruct() override;
	
	void InitializeRewards(const TArray<FRewardEntry*>& RewardSelection) const;
	
	UFUNCTION()
	void ConfirmReward();
	
	UFUNCTION()
	void SelectReward(URewardOptionWidget* SelectedOption);
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UHorizontalBox> RewardBox;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> ConfirmButton;
	
	UPROPERTY(EditDefaultsOnly, Category = "Reward")
	TSubclassOf<URewardOptionWidget> RewardWidgetClass;
	
	FRewardConfirmedDelegate OnRewardConfirmed;
	
private:
	
	UPROPERTY(Transient)
	TObjectPtr<URewardOptionWidget> Selection;
};
