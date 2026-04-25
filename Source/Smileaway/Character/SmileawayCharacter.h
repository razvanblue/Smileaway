// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterTypes.h"
#include "GameFramework/Character.h"
#include "GenericTeamAgentInterface.h"
#include "Smileaway/Animation/AnimationData.h"
#include "Smileaway/Interfaces/HitInterface.h"
#include "SmileawayCharacter.generated.h"

class UCharacterStats;
class UHealthBarWidgetComponent;
class ULevelingComponent;

UCLASS()
class SMILEAWAY_API ASmileawayCharacter : public ACharacter, public IHitInterface, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public:
	ASmileawayCharacter();
	
	void TriggerHitbox(FAttackData AttackData);
	
	void GainExperience(int32 Experience);

	UCharacterStats* GetCharacterStats() const { return Stats; }
	
	ULevelingComponent* GetLevelingComponent() const { return LevelingComponent; }
	
protected:
	virtual void BeginPlay() override;
	
	virtual void GetHit_Implementation(const FVector& ImpactPoint, FHitData HitData, AActor* Hitter) override;
	
	virtual FGenericTeamId GetGenericTeamId() const override;

	virtual void Attack();
	
	UFUNCTION(BlueprintCallable)
	virtual void AttackEnd();
	
	UFUNCTION(BlueprintCallable)
	virtual void Dodge();
	
	virtual void OnDeath();
	
	void PlayMontageSection(UAnimMontage* Montage, const FName& SectionName) const;

	void PlayMontageSection(UAnimMontage* Montage, int32 SectionIndex) const;
	
	void PlayRandomMontageSection(UAnimMontage* Montage) const;
	
	UPROPERTY(VisibleAnywhere, Category = "Character Stats")
	EActionState ActionState = EActionState::Unoccupied;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Stats")
	TObjectPtr<UCharacterStats> Stats;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Stats")
	TObjectPtr<ULevelingComponent> LevelingComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Stats")
	EDamageFaction DamageFaction = EDamageFaction::Neutral;
	
	UPROPERTY(VisibleAnywhere, Category = "Character Stats")
	TObjectPtr<UHealthBarWidgetComponent> HealthBarWidget;
	
	UPROPERTY(EditAnywhere, Category = Montages)
	TObjectPtr<UAnimMontage> AttackMontage;
	
	UPROPERTY(EditAnywhere, Category = Montages)
	TObjectPtr<UAnimMontage> DodgeMontage;
	
	UPROPERTY(EditAnywhere, Category = Montages)
	TObjectPtr<UAnimMontage> HitReactMontage;
	
	UPROPERTY(EditAnywhere, Category = Sounds)
	TObjectPtr<USoundBase> HitSound;

	UPROPERTY(EditAnywhere, Category = VisualEffects)
	TObjectPtr<UParticleSystem> HitParticleSystem;
};
