#include "RewardOptionWidget.h"

#include "Components/Border.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

void URewardOptionWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	NormalButtonStyle = HighlightedButtonStyle = RewardButton->GetStyle();
	HighlightedButtonStyle.Normal.TintColor = FLinearColor::Yellow;
	HighlightedButtonStyle.Hovered.TintColor = FLinearColor::Yellow;
	
	if (RewardButton)
	{
		RewardButton->OnClicked.AddDynamic(this, &ThisClass::SelectReward);
	}
}

void URewardOptionWidget::SetHighlight(bool bEnabled)
{
	if (!RewardButton || !RewardBorder) return;
	
	RewardButton->SetStyle(bEnabled ? HighlightedButtonStyle : NormalButtonStyle);
	RewardBorder->SetBrushColor(bEnabled ? FLinearColor::Yellow : FLinearColor::White);
}

void URewardOptionWidget::InitializeReward(FRewardEntry* InReward)
{
	if (!InReward || !IsValid(InReward->GrantedEntity))
	{
		return;
	}

	Reward = InReward;
	RewardNameText->SetText(Reward->GrantedEntity->Name);
	RewardIcon->SetBrushFromTexture(Reward->GrantedEntity->Icon);
	RewardDescriptionText->SetText(Reward->GrantedEntity->Description);
}

void URewardOptionWidget::SelectReward()
{
	OnRewardSelected.Broadcast(this);
}
