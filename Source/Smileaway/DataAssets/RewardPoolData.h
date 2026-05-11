#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "NamedEntity.h"
#include "NativeGameplayTags.h"
#include "Engine/DataAsset.h"
#include "RewardPoolData.generated.h"

UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Reward_WaveClear);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Reward_LevelUp);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Reward_SkillTree);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Reward_Shop);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Reward_Drop);

USTRUCT(BlueprintType)
struct FRewardEntry
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UNamedEntity> GrantedEntity;
	
	UPROPERTY(EditDefaultsOnly)
	int32 Price = 1;
	
	UPROPERTY(EditDefaultsOnly)
	int32 Count = 1;

	UPROPERTY(EditDefaultsOnly)
	float DropChanceWeight = 1.f;
	
	UPROPERTY(EditDefaultsOnly)
	FGameplayTagContainer RewardTags;
};

/**
 * 
 */
UCLASS()
class SMILEAWAY_API URewardPoolData : public UDataAsset
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditDefaultsOnly)
	TArray<FRewardEntry> Rewards;
};
