#include "RewardMenuWidget.h"
#include "RewardOptionWidget.h"
#include "Components/Button.h"
#include "Components/HorizontalBox.h"
#include "Components/TextBlock.h"
#include "Smileaway/DataAssets/RewardPoolData.h"

void URewardMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	if (ConfirmButton)
	{
		ConfirmButton->SetIsEnabled(false);
		ConfirmButton->OnClicked.AddDynamic(this, &ThisClass::ConfirmReward);
	}
}

void URewardMenuWidget::InitializeRewards(const TArray<FRewardEntry*>& RewardEntries) const
{
	for (FRewardEntry* Reward : RewardEntries)
	{
		URewardOptionWidget* RewardWidget = CreateWidget<URewardOptionWidget>(RewardBox, RewardWidgetClass);
		RewardBox->AddChildToHorizontalBox(RewardWidget);
		RewardWidget->InitializeReward(Reward);
		RewardWidget->OnRewardSelected.AddDynamic(this, &ThisClass::SelectReward);
	}
}

void URewardMenuWidget::SetTitle(FText NewTitle)
{
	if (TitleText)
	{
		TitleText->SetText(NewTitle);
	}
}

void URewardMenuWidget::ConfirmReward()
{
	if (Selection)
	{
		OnRewardConfirmed.Broadcast(Selection->Reward);
	}
	
	
}

void URewardMenuWidget::SelectReward(URewardOptionWidget* SelectedOption)
{
	if (Selection == SelectedOption)
	{
		return;
	}
	
	if (Selection)
	{
		Selection->SetHighlight(false);
	}
	
	Selection = SelectedOption;
	
	if (SelectedOption)
	{
		SelectedOption->SetHighlight(true);
	}
	
	if (ConfirmButton)
	{
		ConfirmButton->SetIsEnabled(Selection != nullptr);
	}
}
