// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterTypes.h"
#include "GameFramework/Character.h"
#include "GenericTeamAgentInterface.h"
#include "Components/TimelineComponent.h"
#include "Smileaway/Animation/AnimationData.h"
#include "Smileaway/Interfaces/HitInterface.h"
#include "SmileawayCharacter.generated.h"

class UNiagaraComponent;
class UCharacterStats;
class UHealthBarWidgetComponent;
class ULevelingComponent;
class UMaterialInterface;
class UNiagaraSystem;

UCLASS()
class SMILEAWAY_API ASmileawayCharacter : public ACharacter, public IHitInterface, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public:
	ASmileawayCharacter();

	virtual void BeginPlay() override;
	
	virtual void Tick(float DeltaTime) override;

	virtual void GetHit_Implementation(const FVector& ImpactPoint, FHitData HitData, AActor* Hitter) override;
	
	virtual FGenericTeamId GetGenericTeamId() const override;
	
	void TriggerHitbox(FAttackData AttackData);
	
	void GainExperience(int32 Experience);
	
	UNiagaraComponent* GetBurningEffect();

	UCharacterStats* GetCharacterStats() const { return Stats; }

	EDamageFaction GetDamageFaction() const { return DamageFaction; }
	
	ULevelingComponent* GetLevelingComponent() const { return LevelingComponent; }
	
protected:

	virtual void Attack();
	
	UFUNCTION(BlueprintCallable)
	virtual void AttackEnd();
	
	UFUNCTION(BlueprintCallable)
	virtual void Dodge();
	
	virtual void OnDeath();
	
	void PlayMontageSection(UAnimMontage* Montage, const FName& SectionName) const;

	void PlayMontageSection(UAnimMontage* Montage, int32 SectionIndex) const;
	
	void PlayRandomMontageSection(UAnimMontage* Montage) const;
	
	void PlayHitFlashEffect();
	
	UFUNCTION()
	void UpdateMovementSpeed(float NewSpeed);
	
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
	
	UPROPERTY(EditDefaultsOnly, Category=VisualEffects)
	TObjectPtr<UNiagaraSystem> HitEffect;
	
	UPROPERTY(EditDefaultsOnly, Category=VisualEffects)
	TObjectPtr<UMaterialInterface> HitFlashOverlay;
	
	UPROPERTY(EditDefaultsOnly, Category = VisualEffects)
	UCurveFloat* HitFlashCurve;
	
	UPROPERTY(EditAnywhere, Category = VisualEffects)
	TObjectPtr<UNiagaraComponent> BurningEffect;
	
private:
	
	UFUNCTION()
	void HandleFlashTimelineProgress(float Value);

	UFUNCTION()
	void HandleFlashTimelineFinished();
	
	void InitializeHitFlash();
	
	UPROPERTY(Transient)
	TObjectPtr<UMaterialInstanceDynamic> HitFlashMaterialInstance;
	
	FTimeline FlashTimeline;
};
