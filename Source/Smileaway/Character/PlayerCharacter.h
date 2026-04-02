// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SmileawayCharacter.h"
#include "PlayerCharacter.generated.h"

class UAnimMontage;
class UCameraComponent;
class UInputAction;
class UInputMappingContext;
class USpringArmComponent;
struct FInputActionValue;

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
	
protected:
	virtual void BeginPlay() override;

	/************ Enhanced Input *************/
	virtual void Attack() override;
	
	void Move(const FInputActionValue& Value);

	void Look(const FInputActionValue& Value);
	
	void HeavyAttack();
	
	void Interact();
	
	virtual void AttackEnd() override;
	
	UFUNCTION(BlueprintCallable)
	void AttackRecovery();
	
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
