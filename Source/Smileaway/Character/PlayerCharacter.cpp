// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Smileaway/Combat/SkillBase.h"
#include "Smileaway/DataAssets/SkillData.h"

APlayerCharacter::APlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	DamageFaction = EDamageFaction::Player;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 540.f, 0.f);

	GetMesh()->SetCollisionObjectType(ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToAllChannels(ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
	GetMesh()->SetGenerateOverlapEvents(true);
	
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 220.f;
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom);
	
	SkillSlots.SetNum(3);
}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveInputAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Move);
		EnhancedInputComponent->BindAction(LookInputAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Look);
		EnhancedInputComponent->BindAction(AttackInputAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Attack);
		EnhancedInputComponent->BindAction(HeavyAttackInputAction, ETriggerEvent::Triggered, this, &APlayerCharacter::HeavyAttack);
		EnhancedInputComponent->BindAction(DodgeInputAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Dodge);
		EnhancedInputComponent->BindAction(InteractInputAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Interact);
		EnhancedInputComponent->BindAction(Skill1InputAction, ETriggerEvent::Triggered, this, &APlayerCharacter::UseSkill<0>);
		EnhancedInputComponent->BindAction(Skill2InputAction, ETriggerEvent::Triggered, this, &APlayerCharacter::UseSkill<1>);
		EnhancedInputComponent->BindAction(Skill3InputAction, ETriggerEvent::Triggered, this, &APlayerCharacter::UseSkill<2>);
	}
}

void APlayerCharacter::EquipSkill(int32 SlotIndex, USkillData* const SkillData)
{
	if (SkillSlots.IsValidIndex(SlotIndex) == false)
	{
		return;
	}
	
	auto& Skill = SkillSlots[SlotIndex];
	if (Skill)
	{
		Skill->Cleanup();
	}
	if (SkillData)
	{
		Skill = NewObject<USkillBase>(this, SkillData->SkillClass ? SkillData->SkillClass.Get() : USkillBase::StaticClass());
		Skill->Cooldown = SkillData->Cooldown;
		Skill->SkillMontage = SkillData->SkillMontage;
	}
	else
	{
		Skill = nullptr;
	}
	
	OnSkillEquipped.Broadcast(SlotIndex, Skill, SkillData);
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	InitializeEnhancedInput();
	InitializeGameOverlay();
	
	if (AttackMontage)
	{
		MaxCombo = AttackMontage->CompositeSections.Num();
	}
}

void APlayerCharacter::Move(const FInputActionValue& Value)
{
	if (!CanMove()) return;
	
	const auto MoveInput = Value.Get<FVector2D>();
	
	const FRotator YawRotation(0.f, GetControlRotation().Yaw, 0.f);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection   = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	// Apply movement
	AddMovementInput(ForwardDirection, MoveInput.Y);
	AddMovementInput(RightDirection, MoveInput.X);
}

void APlayerCharacter::Look(const FInputActionValue& Value)
{
	const auto LookValue = Value.Get<FVector2D>();
	AddControllerYawInput(LookValue.X);
	AddControllerPitchInput(LookValue.Y);
}

void APlayerCharacter::Attack()
{
	bool bCanAttack = AttackMontage && (ActionState & EActionState::CanAttack) != EActionState::None;
	if (bCanAttack == false) return;

	if (ComboCounter >= MaxCombo)
	{
		ComboCounter = 0;
	}
	
	ActionState = EActionState::Attacking;
	PlayMontageSection(AttackMontage, ComboCounter++);
	
	Super::Attack();
}

void APlayerCharacter::HeavyAttack()
{
	bool bCanAttack = AttackMontage && (ActionState & EActionState::CanAttack) != EActionState::None;
	if (bCanAttack && !GetWorldTimerManager().IsTimerActive(CooldownTimer))
	{
		ActionState = EActionState::Attacking;
		ComboCounter = 0;
		PlayMontageSection(SpecialAttackMontage, 0);
		GetWorldTimerManager().SetTimer(CooldownTimer, this, &ThisClass::Interact, SpecialCooldown, false);
	}
}

template <int32 Index>
void APlayerCharacter::UseSkill()
{
	bool bCanAttack = (ActionState & EActionState::CanAttack) != EActionState::None;
	if (bCanAttack && SkillSlots.IsValidIndex(Index) == false) return;
	
	if (USkillBase* Skill = SkillSlots[Index])
	{
		ActionState = EActionState::Attacking;
		ComboCounter = 0;
		Skill->Activate(this);
	}
}

void APlayerCharacter::Interact()
{
}

void APlayerCharacter::AttackEnd()
{
	ActionState = EActionState::Unoccupied;
	ComboCounter = 0;
	
	Super::AttackEnd();
}

void APlayerCharacter::Dodge()
{
	if (DodgeMontage && (ActionState & EActionState::CanAttack) != EActionState::None)
	{
		StopAnimMontage(AttackMontage);
		
		ComboCounter = 0;
		ActionState = EActionState::Attacking;
		
		PlayRandomMontageSection(DodgeMontage);
	
		Super::Dodge();
	}
}

void APlayerCharacter::OnDeath()
{
	UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()), false);
}

void APlayerCharacter::AttackRecovery()
{
	ActionState = EActionState::AttackRecovery;
}

void APlayerCharacter::InitializeEnhancedInput()
{
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (auto* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(InputMappingContext, 0);
		}
	}
}

void APlayerCharacter::InitializeGameOverlay()
{
}

bool APlayerCharacter::CanMove()
{
	return (ActionState & EActionState::CanMove) != EActionState::None;
}
