// Fill out your copyright notice in the Description page of Project Settings.


#include "SmileawayController.h"
#include "Character/PlayerCharacter.h"
#include "Components/CharacterStats.h"
#include "Components/LevelingComponent.h"
#include "DataAssets/SkillData.h"
#include "GameFramework/SmileawayGameMode.h"

void ASmileawayController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	InitializeUI(InPawn);
}

void ASmileawayController::AddStatusEffect(const UStatusEffect* Effect, float Duration)
{
	if (auto* PC = Cast<APlayerCharacter>(GetCharacter()))
	{
		PC->AddStatusEffect(Effect, Duration);
	}
}

void ASmileawayController::EquipSkill(int32 SlotIndex, const USkillData* SkillData)
{
	if (auto* PC = Cast<APlayerCharacter>(GetCharacter()))
	{
		PC->EquipSkill(SlotIndex, SkillData);
	}
}

void ASmileawayController::InitializeUI(APawn* InPawn)
{
	if (!PlayerHUD)
	{
		return;
	}
	
	if (auto* PC = Cast<APlayerCharacter>(InPawn))
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
		
		PC->OnSkillEquipped.AddDynamic(PlayerHUD, &UPlayerHUD::SetSkillIcon);
	}
}
