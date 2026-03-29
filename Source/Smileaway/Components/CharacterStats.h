// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CharacterStats.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealthChangedSignature, float, HealthPercent);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SMILEAWAY_API UCharacterStats : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCharacterStats();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	void TakeDamage(float DamageAmount);
	
	float GetHealthPercentage();
	
	double GetAttack();

	bool IsAlive();

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnHealthChangedSignature OnHealthChanged;
	
protected:
	virtual void BeginPlay() override;
		
private:

	UPROPERTY(EditAnywhere, Category = "Stats")
	double CurrentHealth = 100.f;

	UPROPERTY(EditAnywhere, Category = "Stats")
	double MaxHealth = 100.f;
	
	UPROPERTY(EditAnywhere, Category = "Stats")
	double Attack = 10.f;
	
	UPROPERTY(EditAnywhere, Category = "Stats")
	float Speed  = 1.f;
};
