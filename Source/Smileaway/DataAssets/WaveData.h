#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "WaveData.generated.h"

UENUM(BlueprintType)
enum class ESpawnSelectionMode : uint8
{
	RandomSingle,		// Entire group spawns from one random point
	RandomPerEnemy,		// Each enemy chooses a random point
	Sequential,			// Cycle through points
	ClosestToPlayer		// Spawn at closest point relative to player
};

USTRUCT(BlueprintType)
struct FWaveEntry
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> SpawnClass;
	
	UPROPERTY(EditAnywhere)
	int32 SpawnCount = 1;
	
	UPROPERTY(EditAnywhere)
	float SpawnDelay = 0.f;
	
	UPROPERTY(EditAnywhere)
	ESpawnSelectionMode SpawnMode = ESpawnSelectionMode::RandomPerEnemy;
	
	UPROPERTY(EditAnywhere)
	FName SpawnGroup;
};

/**
 * 
 */
UCLASS(BlueprintType)
class SMILEAWAY_API UWaveData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(EditAnywhere)
	TArray<FWaveEntry> WaveEntries;
	
	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<UObject>> RewardOptions;
};
