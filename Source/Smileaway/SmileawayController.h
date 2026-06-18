// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataAssets/RewardPoolData.h"
#include "GameFramework/PlayerController.h"
#include "UI/PlayerHUD.h"
#include "SmileawayController.generated.h"

class UInputAction;
class UInputMappingContext;
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
	
	UFUNCTION(BlueprintCallable)
	void PauseGame();
	
	UFUNCTION(BlueprintCallable)
	void UnpauseGame();
	
	void GrantPlayerReward(FRewardEntry* Reward);
	
	void OnGameOver();

protected:
	
	virtual void BeginPlay() override;
	
	virtual void SetupInputComponent() override;

	UFUNCTION(BlueprintCallable)
	void InitializeUI(APawn* InPawn);

    UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UPlayerHUD> PlayerHUD;
	
	UPROPERTY(EditDefaultsOnly, Category = "Menu")
	TSubclassOf<UUserWidget> PauseMenuClass;
	
	UPROPERTY(EditDefaultsOnly, Category = "Menu")
	TSubclassOf<UUserWidget> GameOverMenuClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputMappingContext> GameplayInputMappingContext;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputMappingContext> MenuInputMappingContext;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> PauseInputAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> UnpauseInputAction;
	
private:
	
	UPROPERTY(Transient)
	TObjectPtr<UUserWidget> PauseMenu;
};
