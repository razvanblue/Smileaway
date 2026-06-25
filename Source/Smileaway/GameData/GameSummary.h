#pragma once

#include "CoreMinimal.h"
#include "GameSummary.generated.h"

USTRUCT(BlueprintType)
struct FGameSummary
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadOnly)
	float SurvivalTime = 0.0f;

	UPROPERTY(BlueprintReadOnly)
	int32 WaveReached = 0;
	
	UPROPERTY(BlueprintReadOnly)
	int32 EnemiesDefeated = 0;

	UPROPERTY(BlueprintReadOnly)
	int32 ExperienceEarned = 0;

	UPROPERTY(BlueprintReadOnly)
	int32 BuffCount = 0;
};

UCLASS()
class SMILEAWAY_API UGameSummaryLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "Game Summary", meta = (WorldContext = "WorldContext"))
	static FGameSummary MakeSummary(UObject* WorldContext);
};
