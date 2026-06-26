// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyBase.h"

#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Smileaway/Components/LevelingComponent.h"
#include "Smileaway/GameFramework/SmileawayGameMode.h"
#include "Smileaway/UI/HealthBarWidgetComponent.h"

UE_DEFINE_GAMEPLAY_TAG(TAG_Enemy_Add, "Enemy.Add");

AEnemyBase::AEnemyBase()
{
	DamageFaction = EDamageFaction::Enemy;
}

void AEnemyBase::Attack(AActor* Target)
{
	// Actor must be valid
	FVector LookTarget = Target->GetActorLocation();
	LookTarget.Z = GetActorLocation().Z;
	
	SetActorRotation((LookTarget - GetActorLocation()).Rotation());

	Attack();
}

void AEnemyBase::Attack()
{
	bool bCanAttack = AttackMontage && (ActionState & EActionState::CanAttack) != EActionState::None;
	if (bCanAttack)
	{
		//ActionState = EActionState::Attacking;
		PlayRandomMontageSection(AttackMontage);
	}
}

void AEnemyBase::OnDeath()
{
	Super::OnDeath();
	
	if (auto* Player = Cast<ASmileawayCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0)))
	{
		Player->GainExperience(LevelingComponent->GetXPToNextLevel());
	}
	
	if (auto* GameMode = Cast<ASmileawayGameMode>(GetWorld()->GetAuthGameMode()); GameMode && !ActorHasTag(FName("Enemy.Add")))
	{
		GameMode->OnEnemyDeath();
	}
	
	HealthBarWidget->SetVisibility(false);
	
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	// GetMesh()->SetCollisionProfileName("Ragdoll");
	GetMesh()->SetSimulatePhysics(true);
	
	SetLifeSpan(3.f);
}
