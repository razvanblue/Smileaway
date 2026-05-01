#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SmileawayGameMode.generated.h"

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
	void StartWave(int32 NextWave);
	
	UFUNCTION(BlueprintCallable)
	void CompleteWave();
	
	void OnEnemyDeath();
	
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
};
