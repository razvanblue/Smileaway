// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Smileaway/Animation/AnimationData.h"
#include "ProjectileBase.generated.h"

class UNiagaraSystem;
class UProjectileMovementComponent;
class USoundBase;
class USphereComponent;

UCLASS()
class SMILEAWAY_API AProjectileBase : public AActor
{
	GENERATED_BODY()
	
public:	
	AProjectileBase();

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
	
	UPROPERTY(EditAnywhere, Category = "Components")
	TObjectPtr<UProjectileMovementComponent> ProjectileMovement;
	
	UPROPERTY(EditAnywhere, Category = "Components")
	TObjectPtr<USphereComponent> Collision;
	
	UPROPERTY(EditAnywhere, Category = "Components")
	TObjectPtr<UNiagaraSystem> ExplosionEffect;
	
	UPROPERTY(EditAnywhere, Category = "Components")
	TObjectPtr<USoundBase> ExplosionSound;
	
	UPROPERTY(EditAnywhere, Category = "Attack Data")
	FAttackData AttackData;
};
