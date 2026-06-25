#include "GameSummary.h"

#include "Kismet/GameplayStatics.h"
#include "Smileaway/GameFramework/SmileawayGameMode.h"
#include "Smileaway/Character/PlayerCharacter.h"
#include "Smileaway/Components/CharacterStats.h"
#include "Smileaway/Components/LevelingComponent.h"

FGameSummary UGameSummaryLibrary::MakeSummary(UObject* WorldContext)
{
	FGameSummary Summary;
	
	auto* PC = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerPawn(WorldContext->GetWorld(), 0));
	auto* GM = Cast<ASmileawayGameMode>(WorldContext->GetWorld()->GetAuthGameMode());
	
	if (!PC || !GM)
	{
		return Summary;
	}
	
	Summary.SurvivalTime = GM->GetGameTime();
	Summary.WaveReached = GM->GetCurrentWave();
	Summary.EnemiesDefeated = GM->GetDefeatedEnemies();
	Summary.ExperienceEarned = PC->GetLevelingComponent()->GetTotalXP();
	Summary.BuffCount = PC->GetCharacterStats()->GetActiveStatusEffectCount();
	
	return Summary;
}
