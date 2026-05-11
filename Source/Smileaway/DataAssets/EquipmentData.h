#pragma once

#include "CoreMinimal.h"
#include "SkillData.h"
#include "StatusEffect.h"
#include "Engine/DataAsset.h"
#include "EquipmentData.generated.h"

/**
 * 
 */
UCLASS()
class SMILEAWAY_API UEquipmentData : public UNamedEntity
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UStatusEffect> PassiveEffect;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USkillData> UniqueSkill;
};
