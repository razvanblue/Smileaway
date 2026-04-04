// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "UI/HealthBarWidget.h"
#include "SmileawayController.generated.h"

/**
 * 
 */
UCLASS()
class SMILEAWAY_API ASmileawayController : public APlayerController
{
	GENERATED_BODY()

public:

    virtual void OnPossess(APawn* InPawn) override;

protected:

    UPROPERTY(BlueprintReadWrite)
    TObjectPtr<UHealthBarWidget> HealthWidget;

	UFUNCTION(BlueprintCallable)
    void InitializeUI(APawn* InPawn);
};
