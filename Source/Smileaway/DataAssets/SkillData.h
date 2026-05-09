#pragma once

#include "CoreMinimal.h"
#include "NamedEntity.h"
#include "Engine/DataAsset.h"
#include "SkillData.generated.h"

class USkillBase;

/**
 * 
 */
UCLASS(BlueprintType)
class SMILEAWAY_API USkillData : public UNamedEntity
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<USkillBase> SkillClass;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UAnimMontage> SkillMontage;
	
	UPROPERTY(EditDefaultsOnly)
	float Cooldown;
};
