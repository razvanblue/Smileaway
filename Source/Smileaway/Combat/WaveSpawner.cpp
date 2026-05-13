// Fill out your copyright notice in the Description page of Project Settings.


#include "WaveSpawner.h"

#include "EnemySpawnPoint.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Smileaway/Character/EnemyBase.h"
#include "Smileaway/DataAssets/WaveData.h"


void UWaveSpawner::BeginPlay()
{
	Super::BeginPlay();

	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemySpawnPoint::StaticClass(),FoundActors);

	for (AActor* Actor : FoundActors)
	{
		if (AEnemySpawnPoint* SpawnPoint = Cast<AEnemySpawnPoint>(Actor))
		{
			SpawnPoints.Add(SpawnPoint);
		}
	}
}


void UWaveSpawner::BeginWave(UWaveData* WaveData)
{
	if (!IsValid(WaveData) || WaveData->WaveEntries.IsEmpty())
	{
		return;
	}
	
	ActiveWave = WaveData;
	CurrentEntryIndex = -1;
	AliveEnemies = 0;
	
	if (WaveData->WaveEntries[0].SpawnDelay <= 0.05f)
	{
		AdvanceWave();
	}
	else
	{
		GetWorld()->GetTimerManager().SetTimer(
			NextEntryTimer, 
			this,
			&ThisClass::AdvanceWave,
			WaveData->WaveEntries[0].SpawnDelay);
	}
}


void UWaveSpawner::AdvanceWave()
{
	GetWorld()->GetTimerManager().ClearTimer(NextEntryTimer);
	
	if (!ActiveWave || !ActiveWave->WaveEntries.IsValidIndex(CurrentEntryIndex + 1))
	{
		WaveComplete();
		return;
	}
	
	const auto& WaveEntries = ActiveWave->WaveEntries;
	
	do // Spawn the next wave entry and all subsequent entries with delay == 0 
	{
		SpawnWaveEntry(WaveEntries[++CurrentEntryIndex]);
	}
	while (CurrentEntryIndex + 1 < WaveEntries.Num() && WaveEntries[CurrentEntryIndex + 1].SpawnDelay <= 0.05f);
	
	
	if (CurrentEntryIndex + 1 < WaveEntries.Num())
	{
		GetWorld()->GetTimerManager().SetTimer(
			NextEntryTimer, 
			this,
			&ThisClass::AdvanceWave,
			WaveEntries[CurrentEntryIndex + 1].SpawnDelay);
	}
}


void UWaveSpawner::SpawnWaveEntry(const FWaveEntry& Entry)
{
	TArray<AEnemySpawnPoint*> Candidates = GetMatchingSpawnPoints(Entry.SpawnGroup);
	if (Candidates.IsEmpty())
	{
		UE_LOG(LogTemp, Error, TEXT("No spawn candidates found for SpawnGroup '%s'!"), *Entry.SpawnGroup.ToString());
		return;
	}
	
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	
	for (int32 i = 0; i < Entry.SpawnCount; ++i)
	{
		AEnemySpawnPoint* SpawnPoint = Candidates[FMath::RandRange(0, Candidates.Num() - 1)];
		const FVector SpawnLocation = SpawnPoint->GetActorLocation()
			+ FVector(FMath::RandRange(-200.f, 200.f), FMath::RandRange(-200.f, 200.f), 0.f);
		const FRotator SpawnRotation = SpawnPoint->GetActorRotation();;
		
		auto* SpawnedActor = GetWorld()->SpawnActor(Entry.SpawnClass, &SpawnLocation, &SpawnRotation, SpawnParams);
		if (auto* SpawnedCharacter = Cast<AEnemyBase>(SpawnedActor))
		{
			SpawnedCharacter->SpawnDefaultController();
			++AliveEnemies;
		}
	}
}


void UWaveSpawner::WaveComplete()
{
	
}


void UWaveSpawner::OnEnemyDeath()
{
	if (--AliveEnemies <= 0)
	{
		AdvanceWave();
	}
}


TArray<AEnemySpawnPoint*> UWaveSpawner::GetMatchingSpawnPoints(FName Group) const
{
	TArray<AEnemySpawnPoint*> Result;

	for (AEnemySpawnPoint* Point : SpawnPoints)
	{
		if (!IsValid(Point))
		{
			continue;
		}

		if (Group.IsNone() || Point->SpawnGroup == Group)
		{
			Result.Add(Point);
		}
	}

	return Result;
}


AEnemySpawnPoint* UWaveSpawner::ChooseSpawnPoint(const TArray<AEnemySpawnPoint*>& Candidates, ESpawnSelectionMode Mode, int32 Index) const
{
	if (Candidates.IsEmpty())
	{
		return nullptr;
	}

	switch (Mode)
	{
	case ESpawnSelectionMode::RandomSingle:
	case ESpawnSelectionMode::RandomPerEnemy:
		return Candidates[FMath::RandRange(0, Candidates.Num() - 1)];

	case ESpawnSelectionMode::Sequential:
		return Candidates[Index % Candidates.Num()];

	default:
		return Candidates[Index % Candidates.Num()];
	}
}

int32 UWaveSpawner::GetTotalEnemiesInWave(const UWaveData* WaveData) const
{
	int32 TotalEnemies = 0;
	
	for (const auto WaveEntry : WaveData->WaveEntries)
	{
		if (WaveEntry.SpawnClass->IsChildOf(AEnemyBase::StaticClass()))
		{
			TotalEnemies += WaveEntry.SpawnCount;
		}
	}
	
	return TotalEnemies;
}

