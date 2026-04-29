#include "StatusEffectIcon.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Smileaway/DataAssets/StatusEffect.h"


UStatusEffectIcon::UStatusEffectIcon(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, ExpirationTime {0.f}
	, TotalDuration {INFINITE_STATUS_DURATION}
{
}

void UStatusEffectIcon::NativeConstruct()
{
	Super::NativeConstruct();

	if (EffectIcon == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Missing Effect Icon!"));
		return;
	}
	OverlayMaterialInstance = EffectIcon->GetDynamicMaterial();
	ensureMsgf(OverlayMaterialInstance, TEXT("EffectIcon must use a UI material in its Brush."));
}

void UStatusEffectIcon::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	
	if (TotalDuration != INFINITE_STATUS_DURATION)
	{
		UpdateRemainingTime(ExpirationTime - GetWorld()->GetTimeSeconds());
	}
}

void UStatusEffectIcon::InitializeStatusEffect(const UStatusEffect* Effect, float InTotalDuration)
{
	if (Effect == nullptr)
	{
		return;
	}
	
	StatusEffect = Effect;
	TotalDuration = InTotalDuration;
	ExpirationTime = GetWorld()->GetTimeSeconds() + InTotalDuration;
	
	if (StatusEffect->Icon && OverlayMaterialInstance)
	{
		OverlayMaterialInstance->SetTextureParameterValue(TEXT("Icon"), StatusEffect->Icon);
	}
	
	UpdateRemainingTime(TotalDuration);
}

void UStatusEffectIcon::UpdateRemainingTime(float NewRemainingTime)
{
	const float RemainingTime = FMath::Clamp(NewRemainingTime, 0.f, TotalDuration);
	const float RemainingPercent = RemainingTime / TotalDuration;
	
	if (OverlayMaterialInstance)
	{
		OverlayMaterialInstance->SetScalarParameterValue("RemainingPercent", RemainingPercent);
	}
	
	if (DurationText)
	{
		DurationText->SetText(FText::AsNumber(FMath::CeilToInt(RemainingTime)));
	}
}
