// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataAssets/RewardPoolData.h"
#include "GameFramework/PlayerController.h"
#include "UI/PlayerHUD.h"
#include "SmileawayController.generated.h"

class USkillData;
class UStatusEffect;

/**
 * 
 */
UCLASS()
class SMILEAWAY_API ASmileawayController : public APlayerController
{
	GENERATED_BODY()

public:

    virtual void OnPossess(APawn* InPawn) override;

	/**
	 * @param Effect Effect to add
	 * @param Duration Custom duration, leave 0(default) to use the effect's default duration
	 */
	UFUNCTION(BlueprintCallable)
	void AddStatusEffect(const UStatusEffect* Effect, float Duration = 0.f);
	
	UFUNCTION(BlueprintCallable)
	void EquipSkill(int32 SlotIndex, const USkillData* SkillData);

protected:

    UPROPERTY(BlueprintReadWrite)
    TObjectPtr<UPlayerHUD> PlayerHUD;

	UFUNCTION(BlueprintCallable)
    void InitializeUI(APawn* InPawn);
};
