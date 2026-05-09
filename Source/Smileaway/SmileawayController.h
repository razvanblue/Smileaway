// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "UI/PlayerHUD.h"
#include "SmileawayController.generated.h"

/**
 * 
 */
UCLASS()
class SMILEAWAY_API ASmileawayController : public APlayerController
{
	GENERATED_BODY()

public:

    virtual void OnPossess(APawn* InPawn) override;
	
	UFUNCTION(BlueprintCallable)
	void EquipSkill(int32 SlotIndex, class USkillData* const SkillData);

protected:

    UPROPERTY(BlueprintReadWrite)
    TObjectPtr<UPlayerHUD> PlayerHUD;

	UFUNCTION(BlueprintCallable)
    void InitializeUI(APawn* InPawn);
};
