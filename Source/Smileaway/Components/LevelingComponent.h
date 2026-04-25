// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LevelingComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FExperienceChangedDelegate, int32, Amount, int32, ToNextLevel);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FLevelUpDelegate, int32, NewLevel);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SMILEAWAY_API ULevelingComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	ULevelingComponent();
	
	virtual void BeginPlay() override;

	void GainExperience(int32 Amount);

	UFUNCTION(BlueprintCallable)
	int32 GetLevel() const { return Level; }
	
	UFUNCTION(BlueprintCallable)
	int32 GetCurrentXP() const { return CurrentXP; }
	
	UFUNCTION(BlueprintCallable)
	int32 GetXPToNextLevel() const { return XPToNextLevel; }
	
	UPROPERTY(BlueprintAssignable, Category="Level")
	FExperienceChangedDelegate OnExperienceChanged;
	
	UPROPERTY(BlueprintAssignable, Category="Level")
	FLevelUpDelegate OnLevelUp;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Level")
	int32 Level = 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Level")
	int32 CurrentXP = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Level")
	int32 XPToNextLevel = 100;

	UPROPERTY(EditDefaultsOnly, Category="Level")
	TObjectPtr<UCurveFloat> XPCurve;
	
	void LevelUp();
	
	void RecalculateXPThreshold();
};