#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Smileaway/DataAssets/RewardPoolData.h"
#include "SmileawayGameMode.generated.h"

struct FGameplayTag;
class URewardMenuWidget;
class UWaveSpawner;
class UWaveData;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FRemainingEnemiesChangedDelegate, int32, NewRemainingEnemies);

/**
 * 
 */
UCLASS()
class SMILEAWAY_API ASmileawayGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	ASmileawayGameMode();
	
	virtual void StartPlay() override;
	
	UFUNCTION(BlueprintCallable)
	void AdvanceWave();
	
	UFUNCTION(BlueprintCallable)
	void StartWave(int32 NextWave);
	
	UFUNCTION(BlueprintCallable)
	void CompleteWave();
	
	UFUNCTION(BlueprintCallable)
	void ShowRewardMenu();
	
	void OnEnemyDeath();

	void OnRewardConfirmed(FRewardEntry* SelectedReward);
	
	UWaveSpawner* GetWaveSpawner() const { return WaveSpawner; } 
	
	UPROPERTY(BlueprintAssignable)
	FRemainingEnemiesChangedDelegate OnRemainingEnemiesChanged;
	
protected:
	
	UPROPERTY(EditAnywhere)
	int32 CurrentWave = 0;
	
	UPROPERTY(EditAnywhere)
	TArray<TObjectPtr<UWaveData>> Waves;
	
	UPROPERTY()
	TObjectPtr<UWaveSpawner> WaveSpawner;
	
	UPROPERTY(BlueprintReadOnly, Category = "Gameplay")
	int32 RemainingEnemies = 0;		// Includes alive and enemies not yet spawned
	
	UPROPERTY(EditDefaultsOnly, Category="Rewards")
	TObjectPtr<URewardPoolData> RewardPool;
	
	UPROPERTY(EditDefaultsOnly, Category="Rewards")
	TSubclassOf<URewardMenuWidget> RewardMenuClass;
	
private:
	
	TArray<FRewardEntry*> GenerateRewardChoices(int32 Count, const FGameplayTag& Tag);
	
	void ResumeGameplay();
	
	UPROPERTY()
	TObjectPtr<URewardMenuWidget> ActiveRewardMenu;
};
