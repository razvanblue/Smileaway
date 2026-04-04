// Fill out your copyright notice in the Description page of Project Settings.


#include "SmileawayController.h"
#include "Character/SmileawayCharacter.h"
#include "Components/CharacterStats.h"

void ASmileawayController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	InitializeUI(InPawn);
}

void ASmileawayController::InitializeUI(APawn* InPawn)
{
	if (!HealthWidget)
	{
		return;
	}
	
	if (auto* PC = Cast<ASmileawayCharacter>(InPawn))
	{
		PC->GetCharacterStats()->OnHealthChanged.AddDynamic(HealthWidget, &UHealthBarWidget::SetHealthPercent);
		HealthWidget->SetHealthPercent(PC->GetCharacterStats()->GetHealthPercentage());
	}
}
