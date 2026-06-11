// Fill out your copyright notice in the Description page of Project Settings.


#include "SmileawayController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "AI/NavigationSystemBase.h"
#include "Character/PlayerCharacter.h"
#include "Components/CharacterStats.h"
#include "Components/LevelingComponent.h"
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

void ASmileawayController::PauseGame()
{
	if (PauseMenu == nullptr)
	{
		PauseMenu = CreateWidget<UUserWidget>(this, PauseMenuClass);
		PauseMenu->AddToViewport();
	}
	PauseMenu->SetVisibility(ESlateVisibility::Visible);
	
	SetPause(true);
	FInputModeGameAndUI InputModeGameAndUI;
	SetInputMode(InputModeGameAndUI);
	bShowMouseCursor = true;
	
	if (auto* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->RemoveMappingContext(GameplayInputMappingContext);
		Subsystem->AddMappingContext(MenuInputMappingContext, 1);
	}
}

void ASmileawayController::UnpauseGame()
{
	if (PauseMenu)
	{
		PauseMenu->SetVisibility(ESlateVisibility::Collapsed);
	}
	
	FInputModeGameOnly InputModeGameOnly;
	SetInputMode(InputModeGameOnly);
	bShowMouseCursor = false;
	SetPause(false);
	
	if (auto* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->RemoveMappingContext(MenuInputMappingContext);
		Subsystem->AddMappingContext(GameplayInputMappingContext, 1);
	}
}

void ASmileawayController::GrantPlayerReward(FRewardEntry* Reward)
{
	if (auto* PC = Cast<APlayerCharacter>(GetCharacter()))
	{
		PC->GrantReward(Reward);
	}
}

void ASmileawayController::BeginPlay()
{
	Super::BeginPlay();
	
	if (auto* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(GameplayInputMappingContext, 0);
	}
}

void ASmileawayController::SetupInputComponent()
{
	Super::SetupInputComponent();
	
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent))
	{
		EnhancedInputComponent->BindAction(PauseInputAction, ETriggerEvent::Triggered, this, &ThisClass::PauseGame);
		EnhancedInputComponent->BindAction(UnpauseInputAction, ETriggerEvent::Triggered, this, &ThisClass::UnpauseGame);
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
			LevelingComponent->OnLevelUp.AddDynamic(GameMode, &ASmileawayGameMode::OnPlayerLevelUp);
		}
		
		PC->OnSkillEquipped.AddDynamic(PlayerHUD, &UPlayerHUD::SetSkillIcon);
	}
}
