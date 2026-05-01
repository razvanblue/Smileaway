#include "SmileawayGameMode.h"

#include "Smileaway/Combat/WaveSpawner.h"

ASmileawayGameMode::ASmileawayGameMode()
{
	WaveSpawner = CreateDefaultSubobject<UWaveSpawner>(TEXT("Wave Spawner"));
}

void ASmileawayGameMode::StartPlay()
{
	Super::StartPlay();
	
	StartWave(1);
}

void ASmileawayGameMode::StartWave(int32 NextWave)
{
	if (NextWave - 1 < Waves.Num())
	{
		CurrentWave = NextWave;
		
		RemainingEnemies = WaveSpawner->GetTotalEnemiesInWave(Waves[CurrentWave - 1]);
		OnRemainingEnemiesChanged.Broadcast(RemainingEnemies);
		
		WaveSpawner->BeginWave(Waves[CurrentWave - 1]);
	}
}

void ASmileawayGameMode::CompleteWave()
{
	if (CurrentWave - 1 < Waves.Num())
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Level Complete"));
		// TODO: Level clear
	}
	else
	{
		StartWave(CurrentWave + 1);
	}
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
