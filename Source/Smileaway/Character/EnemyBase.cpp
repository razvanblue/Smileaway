// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyBase.h"

#include "Components/CapsuleComponent.h"
#include "Smileaway/UI/HealthBarWidgetComponent.h"

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
