#include "SmileawayGameMode.h"

#include "Smileaway/SmileawayController.h"
#include "Smileaway/Combat/WaveSpawner.h"
#include "Smileaway/DataAssets/RewardPoolData.h"
#include "Smileaway/UI/RewardMenuWidget.h"

ASmileawayGameMode::ASmileawayGameMode()
{
	WaveSpawner = CreateDefaultSubobject<UWaveSpawner>(TEXT("Wave Spawner"));
}

void ASmileawayGameMode::StartPlay()
{
	Super::StartPlay();
	
	RewardPool = DuplicateObject<URewardPoolData>(RewardPool, this);
}

void ASmileawayGameMode::AdvanceWave()
{
	if (CurrentWave < Waves.Num())
	{
		StartWave(CurrentWave + 1);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Level Complete"));
		// TODO: Level clear
	}
}

void ASmileawayGameMode::StartWave(int32 NextWave)
{
	if (NextWave - 1 >= Waves.Num())
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Missing wave"));
		return;
	}
	
	CurrentWave = NextWave;
	
	RemainingEnemies = WaveSpawner->GetTotalEnemiesInWave(Waves[CurrentWave - 1]);
	OnRemainingEnemiesChanged.Broadcast(RemainingEnemies);
	
	WaveSpawner->BeginWave(Waves[CurrentWave - 1]);
}

void ASmileawayGameMode::CompleteWave()
{
	if (!RewardPool->Rewards.IsEmpty())
	{
		const auto RewardChoices = GenerateRewardChoices(3, TAG_Reward_WaveClear);
		ShowRewardMenu();
	}
	else // No rewards left, start the next wave directly
	{
		AdvanceWave();
	}
}

void ASmileawayGameMode::ShowRewardMenu()
{
	auto* PC = Cast<ASmileawayController>(GetWorld()->GetFirstPlayerController());
	if (!PC || !RewardMenuClass)
	{
		return;
	}
	
	const TArray<FRewardEntry*> RewardChoices = GenerateRewardChoices(3, TAG_Reward_WaveClear);
	if (RewardChoices.IsEmpty())
	{
		return;
	}

	ActiveRewardMenu = CreateWidget<URewardMenuWidget>(PC, RewardMenuClass);
	ActiveRewardMenu->InitializeRewards(RewardChoices);
	ActiveRewardMenu->OnRewardConfirmed.AddUObject(this, &ThisClass::OnRewardConfirmed);
	ActiveRewardMenu->OnRewardConfirmed.AddUObject(PC, &ASmileawayController::GrantPlayerReward);
	ActiveRewardMenu->AddToViewport();

	PC->SetPause(true);
	FInputModeUIOnly InputModeUIOnly;
	PC->SetInputMode(InputModeUIOnly);
	PC->bShowMouseCursor = true;
}

void ASmileawayGameMode::OnEnemyDeath()
{
	OnRemainingEnemiesChanged.Broadcast(--RemainingEnemies);
	
	WaveSpawner->OnEnemyDeath();
	
	if (RemainingEnemies <= 0)
	{
		CompleteWave();
	}
}

void ASmileawayGameMode::OnRewardConfirmed(FRewardEntry* ConfirmedReward)
{
	if (ConfirmedReward == nullptr || ConfirmedReward->GrantedEntity == nullptr)
	{
		return;
	}
	
	ConfirmedReward->Count--;
	
	auto* PC = Cast<ASmileawayController>(GetWorld()->GetFirstPlayerController());
	// TODO: Grant reward
	
	ResumeGameplay();
	AdvanceWave();
}


TArray<FRewardEntry*> ASmileawayGameMode::GenerateRewardChoices(int32 Count, const FGameplayTag& Tag)
{
	TArray<FRewardEntry*> RewardCandidates;
	TArray<FRewardEntry*> FinalRewardSelection;
	FinalRewardSelection.Reserve(Count);
	
	for (FRewardEntry& Entry : RewardPool->Rewards)
	{
		if (Entry.Count > 0 && (!Tag.IsValid() || Entry.RewardTags.HasTag(Tag)))
		{
			RewardCandidates.Add(&Entry);
		}
	}
	
	while (FinalRewardSelection.Num() < Count && !RewardCandidates.IsEmpty())
	{
		int32 Index = FMath::RandRange(0, RewardCandidates.Num() - 1);
		
		FinalRewardSelection.Add(RewardCandidates[Index]);
		RewardCandidates.RemoveAt(Index);
	}
	
	return FinalRewardSelection;
}

void ASmileawayGameMode::ResumeGameplay()
{
	if (ActiveRewardMenu)
	{
		ActiveRewardMenu->RemoveFromParent();
		ActiveRewardMenu = nullptr;
	}
	
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	FInputModeGameOnly InputModeGameOnly;
	PC->SetInputMode(InputModeGameOnly);
	PC->bShowMouseCursor = false;
	PC->SetPause(false);
}