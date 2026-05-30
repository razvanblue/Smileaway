// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GenericTeamAgentInterface.h"
#include "GameFramework/Actor.h"
#include "Smileaway/Animation/AnimationData.h"
#include "Smileaway/Character/CharacterTypes.h"
#include "ProjectileBase.generated.h"

class UNiagaraSystem;
class UProjectileMovementComponent;
class USoundBase;
class USphereComponent;

UCLASS()
class SMILEAWAY_API AProjectileBase : public AActor, public IGenericTeamAgentInterface
{
	GENERATED_BODY()
	
public:	
	AProjectileBase();

	virtual FGenericTeamId GetGenericTeamId() const override;
	
	UPROPERTY(EditAnywhere, Category = "Attack Data")
	FAttackData AttackData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack Data")
	EDamageFaction DamageFaction = EDamageFaction::Neutral;

protected:
	virtual void BeginPlay() override;
	
	UFUNCTION(BlueprintCallable)
	virtual void OnOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);
	
	UFUNCTION(BlueprintCallable)
	void OnHit(
		UPrimitiveComponent* HitComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		FVector NormalImpulse,
		const FHitResult& Hit);
	
	void Explode();
	
	void TriggerHitbox();
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UProjectileMovementComponent> ProjectileMovement;
	
	UPROPERTY(EditAnywhere, Category = "Components")
	TObjectPtr<USphereComponent> Collision;
	
	UPROPERTY(EditAnywhere, Category = "Components")
	TObjectPtr<UParticleSystem> ExplosionEffect;
	
	UPROPERTY(EditAnywhere, Category = "Components")
	TObjectPtr<USoundBase> ExplosionSound;
};
