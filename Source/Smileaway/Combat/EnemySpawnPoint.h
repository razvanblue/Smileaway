#pragma once

#include "CoreMinimal.h"
#include "Engine/TargetPoint.h"
#include "EnemySpawnPoint.generated.h"

UCLASS()
class SMILEAWAY_API AEnemySpawnPoint : public ATargetPoint
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Spawn")
	FName SpawnGroup;
};
