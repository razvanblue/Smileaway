#pragma once

#include "CoreMinimal.h"
#include "CharacterStat.h"
#include "StatusEffect.generated.h"

constexpr float INFINITE_STATUS_DURATION = -1.f;

UCLASS(BlueprintType)
class UStatusEffect : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	FName BuffId;

	UPROPERTY(EditAnywhere)
	float Duration = -1.f; // -1 means infinite duration
	
	UPROPERTY(EditAnywhere)
	uint32 MaxStackCount = 1;
	
	UPROPERTY(EditAnywhere)
	TArray<FStatModifier> Modifiers;
};
