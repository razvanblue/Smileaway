// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Smileaway/Animation/AnimationData.h"
#include "Smileaway/Interfaces/HitInterface.h"
#include "SmileawayCharacter.generated.h"

UCLASS()
class SMILEAWAY_API ASmileawayCharacter : public ACharacter, public IHitInterface
{
	GENERATED_BODY()

public:
	ASmileawayCharacter();
	
	void TriggerHitbox(FAttackData AttackData);
	
protected:
	virtual void BeginPlay() override;
	
	virtual void GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter) override;
	
	virtual void Attack();
	
	UFUNCTION(BlueprintCallable)
	virtual void AttackEnd();
	
	virtual void OnDeath();
	
	void PlayMontageSection(UAnimMontage* Montage, const FName& SectionName) const;

	void PlayMontageSection(UAnimMontage* Montage, int32 SectionIndex) const;
	
	void PlayRandomMontageSection(UAnimMontage* Montage) const;
	
	UPROPERTY(EditAnywhere, Category = Montages)
	TObjectPtr<UAnimMontage> AttackMontage;
	
	UPROPERTY(EditAnywhere, Category = Montages)
	TObjectPtr<UAnimMontage> HitReactMontage;
	
	UPROPERTY(EditAnywhere, Category = Sounds)
	TObjectPtr<USoundBase> HitSound;

	UPROPERTY(EditAnywhere, Category = VisualEffects)
	TObjectPtr<UParticleSystem> HitParticleSystem;
};
