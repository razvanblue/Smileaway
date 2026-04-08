#pragma once

#include "CoreMinimal.h"
#include "AnimationData.generated.h"

USTRUCT(BlueprintType)
struct FAttackData
{
	GENERATED_USTRUCT_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AttackMultiplier = 1.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float PushbackForce = 0.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float LiftUpForce = 0.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RangeMin = 0.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RangeMax = 100.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float HitboxHalfWidth = 50.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float HitboxHalfHeight = 100.f;
};