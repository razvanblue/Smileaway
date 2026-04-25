// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "HealthBarWidgetComponent.generated.h"

/**
 * 
 */
UCLASS()
class SMILEAWAY_API UHealthBarWidgetComponent : public UWidgetComponent
{
	GENERATED_BODY()
	
public:

	virtual void InitWidget() override;
	
	UFUNCTION(BlueprintCallable)
	void SetHealth(float NewHealth, float MaxHealth);

private:
	UPROPERTY(Transient)
	class UHealthBarWidget* HealthBarWidget;
	
};
