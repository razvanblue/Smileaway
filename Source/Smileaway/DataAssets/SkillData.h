#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "SkillData.generated.h"

class USkillBase;

/**
 * 
 */
UCLASS(BlueprintType)
class SMILEAWAY_API USkillData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(EditDefaultsOnly)
	FText Name;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UTexture2D> Icon;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<USkillBase> SkillClass;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UAnimMontage> SkillMontage;
	
	UPROPERTY(EditDefaultsOnly)
	float Cooldown;
};
