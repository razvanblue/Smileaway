// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SmileawayCharacter.h"
#include "PlayerCharacter.generated.h"

class UAnimMontage;
class UCameraComponent;
class UInputAction;
class UInputMappingContext;
class USkillBase;
class USkillData;
class UStatusEffect;
class USpringArmComponent;
struct FInputActionValue;
struct FRewardEntry;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FSkillEquipped, int32, SkillSlotIndex, USkillBase* const, Skill, const USkillData*, SkillData);

/**
 * 
 */
UCLASS()
class SMILEAWAY_API APlayerCharacter : public ASmileawayCharacter
{
	GENERATED_BODY()

public:
	APlayerCharacter();

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	void AddStatusEffect(const UStatusEffect* Effect, float Duration = 0.f);
	
	void EquipSkill(int32 SlotIndex, const USkillData* SkillData);
	
	void GrantReward(const FRewardEntry* Reward);

	UPROPERTY(BlueprintAssignable)
	FSkillEquipped OnSkillEquipped;
	
protected:
	virtual void BeginPlay() override;

	/************ Enhanced Input *************/
	virtual void Attack() override;
	
	void Move(const FInputActionValue& Value);

	void Look(const FInputActionValue& Value);
	
	void HeavyAttack();

	/**
	 * @tparam Index Skill slot index 
	 */
	template<int32 Index>
	void UseSkill();
	
	void Interact();
	
	virtual void AttackEnd() override;

	virtual void Dodge() override;
	
	virtual void OnDeath() override;
	
	UFUNCTION(BlueprintCallable)
	void AttackRecovery();
	
	UPROPERTY(EditDefaultsOnly, Category = "Skills")
	TArray<TObjectPtr<USkillBase>> SkillSlots;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputMappingContext> InputMappingContext;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> LookInputAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> MoveInputAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> AttackInputAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> HeavyAttackInputAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> DodgeInputAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> InteractInputAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> Skill1InputAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> Skill2InputAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> Skill3InputAction;
	
	UPROPERTY(EditAnywhere, Category = Montages)
	TObjectPtr<UAnimMontage> SpecialAttackMontage;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<UCameraComponent> FollowCamera;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<USpringArmComponent> CameraBoom;
	

private:
	void InitializeEnhancedInput();
	
	void InitializeGameOverlay();
	
	bool CanMove();
	
	FTimerHandle CooldownTimer;
	float SpecialCooldown  = 3.f;
	int32 ComboCounter = 0;
	int32 MaxCombo = 0;
	
};
