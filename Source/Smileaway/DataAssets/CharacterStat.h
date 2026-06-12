#pragma once

#include "CoreMinimal.h"
#include <array>

#include "CharacterStat.generated.h"

UENUM(BlueprintType)
enum class EStats : uint8
{
	MaxHP UMETA(DisplayName = "Max HP"),
	Attack UMETA(DisplayName = "Attack"),
	Speed UMETA(DisplayName = "Speed"),
	KnockbackMultiplier UMETA(DisplayName = "Knockback Multiplier"),
	
	MAX UMETA(Hidden)
};
constexpr uint8 StatCount = static_cast<uint8>(EStats::MAX);

template<typename T>
struct TStatArray
{
	std::array<T, StatCount> Data;
	
	FORCEINLINE T& operator[](EStats Stat)
	{
		return Data[static_cast<uint8>(Stat)];
	}
	
	FORCEINLINE const T& operator[](EStats Stat) const
	{
		return Data[static_cast<uint8>(Stat)];
	}
	
	FORCEINLINE T& operator[](uint8 Index)
	{
		return Data[Index];
	}
	
	FORCEINLINE const T& operator[](uint8 Index) const
	{
		return Data[Index];
	}
};

USTRUCT(BlueprintType)
struct FStats
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxHP = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Attack = 10.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Speed  = 1.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float KnockbackMultiplier = 1.f;
	
	FORCEINLINE TStatArray<float> Pack() const
	{
		return {MaxHP, Attack, Speed, KnockbackMultiplier};
	}
};
static_assert(sizeof(FStats) == sizeof(float) * StatCount, "FStats should be same size as EStats enum");

UENUM(BlueprintType)
enum class EModifierType : uint8
{
	Additive UMETA(DisplayName = "Additive"),
	Multiplicative UMETA(DisplayName = "Multiplicative")
};

USTRUCT(BlueprintType)
struct FStatModifier
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	EStats Stat;
	
	UPROPERTY(EditAnywhere)
	EModifierType Type;
	
	UPROPERTY(EditAnywhere)
	float Value;
};