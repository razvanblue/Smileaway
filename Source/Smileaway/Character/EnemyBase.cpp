// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyBase.h"

#include "Components/CapsuleComponent.h"
#include "Smileaway/UI/HealthBarWidgetComponent.h"

AEnemyBase::AEnemyBase()
{
	DamageFaction = EDamageFaction::Enemy;
}

void AEnemyBase::Attack(AActor* Target)
{
	// Actor must be valid
	SetActorRotation((Target->GetActorLocation() - GetActorLocation()).Rotation());

	Attack();
}

void AEnemyBase::Attack()
{
	bool bCanAttack = AttackMontage && (ActionState & EActionState::CanAttack) != EActionState::None;
	if (bCanAttack)
	{
		//ActionState = EActionState::Attacking;
		PlayMontageSection(AttackMontage, 0);
	}
}

void AEnemyBase::OnDeath()
{
	Super::OnDeath();
	
	HealthBarWidget->SetVisibility(false);
	
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	// GetMesh()->SetCollisionProfileName("Ragdoll");
	GetMesh()->SetSimulatePhysics(true);
	
	SetLifeSpan(3.f);
}
