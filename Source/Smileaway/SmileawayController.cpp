// Fill out your copyright notice in the Description page of Project Settings.


#include "SmileawayController.h"
#include "Character/SmileawayCharacter.h"
#include "Combat/WaveSpawner.h"
#include "Components/CharacterStats.h"
#include "Components/LevelingComponent.h"
#include "GameFramework/SmileawayGameMode.h"

void ASmileawayController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	InitializeUI(InPawn);
}

void ASmileawayController::InitializeUI(APawn* InPawn)
{
	if (!PlayerHUD)
	{
		return;
	}
	
	if (auto* PC = Cast<ASmileawayCharacter>(InPawn))
	{
		auto CharacterStats = PC->GetCharacterStats();
		CharacterStats->OnHealthChanged.AddDynamic(PlayerHUD, &UPlayerHUD::SetHealth);
		PlayerHUD->SetHealth(CharacterStats->GetHealth(), CharacterStats->GetStat(EStats::MaxHP));
		CharacterStats->OnStatusEffectApplied.AddDynamic(PlayerHUD, &UPlayerHUD::AddStatusEffect);
		CharacterStats->OnStatusEffectRemoved.AddDynamic(PlayerHUD, &UPlayerHUD::RemoveStatusEffect);
		
		auto LevelingComponent = PC->GetLevelingComponent();
		LevelingComponent->OnExperienceChanged.AddDynamic(PlayerHUD, &UPlayerHUD::SetExperience);
		LevelingComponent->OnLevelUp.AddDynamic(PlayerHUD, &UPlayerHUD::SetLevel);
		PlayerHUD->SetExperience(LevelingComponent->GetCurrentXP(), LevelingComponent->GetXPToNextLevel());
		PlayerHUD->SetLevel(LevelingComponent->GetLevel());
		
		if (const auto GameMode = Cast<ASmileawayGameMode>(GetWorld()->GetAuthGameMode()))
		{
			GameMode->OnRemainingEnemiesChanged.AddDynamic(PlayerHUD, &UPlayerHUD::SetRemainingEnemies);
		}
	}
}
