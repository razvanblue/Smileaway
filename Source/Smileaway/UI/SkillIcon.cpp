#include "SkillIcon.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Smileaway/DataAssets/SkillData.h"


USkillIcon::USkillIcon(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, CooldownResetTime {0.f}
	, CooldownDuration {0.f}
{
}

void USkillIcon::NativeConstruct()
{
	Super::NativeConstruct();
	
	SkillIcon->SetVisibility(ESlateVisibility::Hidden);

	if (SkillIcon == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Missing Skill Icon!"));
		return;
	}
	OverlayMaterialInstance = SkillIcon->GetDynamicMaterial();
	ensureMsgf(OverlayMaterialInstance, TEXT("SkillIcon must use a UI material in its Brush."));
}

void USkillIcon::EnterCooldown(float InCooldownDuration)
{
	CooldownDuration = InCooldownDuration;
	CooldownResetTime = GetWorld()->GetTimeSeconds() + InCooldownDuration;
}

void USkillIcon::ResetCooldown()
{
	UpdateRemainingTime(0.f);
}

void USkillIcon::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	
	if (SkillData && CooldownDuration != 0.f)
	{
		UpdateRemainingTime(CooldownResetTime - GetWorld()->GetTimeSeconds());
	}
}

void USkillIcon::SetSkill(const USkillData* Skill, float InCooldownDuration)
{
	SkillData = Skill;
	EnterCooldown(InCooldownDuration);
	
	if (OverlayMaterialInstance && SkillData && SkillData->Icon)
	{
		OverlayMaterialInstance->SetTextureParameterValue(TEXT("Icon"), SkillData->Icon);
	}
	
	UpdateRemainingTime(CooldownDuration); // Resets skill cooldown
	SkillIcon->SetVisibility(Skill ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}

void USkillIcon::UpdateRemainingTime(float NewRemainingTime)
{
	const float RemainingTime = FMath::Clamp(NewRemainingTime, 0.f, CooldownDuration);
	if (RemainingTime == 0.f)
	{
		CooldownDuration = 0.f;
	}
	
	const float RemainingPercent = (CooldownDuration > 0)
		? RemainingTime / CooldownDuration
		: 0.f;
	
	if (OverlayMaterialInstance)
	{
		OverlayMaterialInstance->SetScalarParameterValue("RemainingPercent", RemainingPercent);
	}
	
	if (DurationText)
	{
		DurationText->SetText(RemainingTime > 0.15f ? FText::AsNumber(FMath::CeilToInt(RemainingTime)) : FText::GetEmpty());
	}
}
