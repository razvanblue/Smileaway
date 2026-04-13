// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include <array>
#include "CharacterStats.generated.h"

UENUM(BlueprintType)
enum class EStats : uint8
{
	MaxHP UMETA(DisplayName = "Max HP"),
	Attack UMETA(DisplayName = "Attack"),
	Speed UMETA(DisplayName = "Speed"),
	
	MAX UMETA(Hidden)
};

template<typename T>
struct TStatArray
{
	std::array<T, static_cast<uint8>(EStats::MAX)> Data;
	
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
	
	FORCEINLINE TStatArray<float> Pack() const
	{
		return {MaxHP, Attack, Speed};
	}
};
static_assert(sizeof(FStats) == sizeof(float) * static_cast<uint8>(EStats::MAX), "FStats should be same size as EStats enum");

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

	EStats Stat;
	EModifierType Type;
	float Value;

	float Duration = -1.f; // -1 = permanent
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealthChangedSignature, float, HealthPercent);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SMILEAWAY_API UCharacterStats : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCharacterStats();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	void TakeDamage(float DamageAmount);
	
	void AddModifier(const FStatModifier& Modifier);
	
	float GetHealthPercentage();
	
	double GetAttack();

	bool IsAlive();

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnHealthChangedSignature OnHealthChanged;
	
protected:
	virtual void BeginPlay() override;
		
private:
	
	UPROPERTY(EditAnywhere, Category = "Stats")
	FStats StatsConfig;

	UPROPERTY(EditAnywhere, Category = "Stats")
	double Health = 100.f;
	
	UPROPERTY(EditAnywhere, Category = "Stats")
	TArray<FStatModifier> StatModifiers;
	
	// Internal static data structure used for fast calculations of base stats
	TStatArray<float> BaseStats;
	
	// Internal static data structure used for fast calculations of final stats, including modifiers
	TStatArray<float> FinalStats;
};
