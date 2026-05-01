#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WaveSpawner.generated.h"

enum class ESpawnSelectionMode : uint8;
class AEnemySpawnPoint;
class UWaveData;
struct FWaveEntry;

/**
 * 
 */
UCLASS()
class SMILEAWAY_API UWaveSpawner : public UActorComponent
{
	GENERATED_BODY()

public:
	
	void BeginWave(UWaveData* WaveData);
	
	void AdvanceWave();

	void SpawnWaveEntry(const FWaveEntry& Entry);
	
	void WaveComplete();
	
	void OnEnemyDeath();

	/**
	 * @return Total number of enemies that will be spawned in the given wave
	 */
	int32 GetTotalEnemiesInWave(const UWaveData* WaveData) const;
	
protected:
	
	virtual void BeginPlay() override;
	
	TArray<AEnemySpawnPoint*> GetMatchingSpawnPoints(FName Group) const;
	
	UPROPERTY(Transient)
	TArray<AEnemySpawnPoint*> SpawnPoints;
	
private:
	AEnemySpawnPoint* ChooseSpawnPoint(
		const TArray<AEnemySpawnPoint*>& Candidates, ESpawnSelectionMode Mode, int32 Index) const;
	
	UPROPERTY(Transient)
	const UWaveData* ActiveWave;
	
	FTimerHandle NextEntryTimer;
	
	int32 CurrentEntryIndex = -1;
	
	int32 AliveEnemies = 0;		// Enemies that have been spawned and haven't died
};
