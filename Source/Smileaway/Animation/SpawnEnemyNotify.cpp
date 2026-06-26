// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnEnemyNotify.h"

#include "Smileaway/Character/EnemyBase.h"

void USpawnEnemyNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                               const FAnimNotifyEventReference& EventReference)
{
	auto* Owner = MeshComp->GetOwner();
	if (!Owner) return;
	
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	const FVector SpawnLocation = Owner->GetActorLocation() + Owner->GetActorTransform().TransformVectorNoScale(SpawnLocationRelative);
	const FRotator SpawnRotation = Owner->GetActorRotation();
	
	auto* SpawnedActor = Owner->GetWorld()->SpawnActor(EnemyClass, &SpawnLocation, &SpawnRotation, SpawnParams);
	
	if (auto* SpawnedCharacter = Cast<AEnemyBase>(SpawnedActor))
	{
		SpawnedCharacter->SpawnDefaultController();
		SpawnedCharacter->Tags.Add(FName("Enemy.Add"));
	}
}
