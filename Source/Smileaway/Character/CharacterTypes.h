#pragma once

UENUM(BlueprintType)
enum class EActionState : uint8
{
	None = 0x00,
	Unoccupied = 0x01 UMETA(DisplayName = "Unoccupied"),
	Attacking = 0x02 UMETA(DisplayName = "Attacking"),
	ComboWindow = 0x04 UMETA(DisplayName = "Combo Window"),
	AttackRecovery = 0x08 UMETA(DisplayName = "Attack Recovery"),
	Equipping = 0x10 UMETA(DisplayName = "Equipping"),
	Staggered = 0x20 UMETA(DisplayName = "Staggered"),
	Recovering = 0x40 UMETA(DisplayName = "Recovering"),
	Dead = 0x80 UMETA(DisplayName = "Dead"),
	
	// Flag Combinations
	CanAttack = Unoccupied | ComboWindow | AttackRecovery,
	CanMove = Unoccupied | Equipping | Attacking | ComboWindow | AttackRecovery // Can move while attacking
};

ENUM_CLASS_FLAGS(EActionState)