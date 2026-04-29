#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Smileaway/DataAssets/CharacterStat.h"
#include "Smileaway/DataAssets/StatusEffect.h"
#include "CharacterStats.generated.h"

USTRUCT()
struct FActiveStatusEffect
{
	GENERATED_BODY()

	UPROPERTY()
	const UStatusEffect* Data;

	UPROPERTY(VisibleAnywhere)
	float RemainingTime = 0.f;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHealthChangedSignature, float, NewHealth, float, MaxHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnStatusEffectAppliedSignature, const UStatusEffect*, Effect, float, Duration);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStatusEffectRemovedSignature, const UStatusEffect*, Effect);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SMILEAWAY_API UCharacterStats : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCharacterStats();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	UFUNCTION(BlueprintCallable)
	void AddStatusEffect(const UStatusEffect* Effect);
	
	void TakeDamage(float DamageAmount);
	
	float GetAttack();
	
	float GetHealth();
	
	float GetHealthPercentage();
	
	float GetStat(EStats Stat);
	
	float GetBaseStat(EStats Stat);

	bool IsAlive();

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnHealthChangedSignature OnHealthChanged;
	
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnStatusEffectAppliedSignature OnStatusEffectApplied;
	
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnStatusEffectRemovedSignature OnStatusEffectRemoved;
	
protected:
	virtual void BeginPlay() override;
		
private:
	
	void CalculateFinalStats();
	
	void TickBuffs(float DeltaTime);
	
	UPROPERTY(EditAnywhere, Category = "Stats")
	FStats StatsConfig;

	UPROPERTY(EditAnywhere, Category = "Stats")
	double Health = 100.f;
	
	UPROPERTY(EditAnywhere, Category = "Stats")
	TArray<FActiveStatusEffect> ActiveEffects;
	
	// Internal static data structure used for fast calculations of base stats
	TStatArray<float> BaseStats;
	
	// Internal static data structure used for fast calculations of final stats, including modifiers
	TStatArray<float> FinalStats;
};
