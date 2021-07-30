// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class ENetworkOwnerType : uint8
{
	OwnedAI UMETA(DisplayName = "OwnedAI"),
	RemoteAI UMETA(DisplayName = "RemoteAI"),
	OwnedPlayer UMETA(DisplayName = "OwnedPlayer"),
	RemotePlayer UMETA(DisplayName = "RemotePlayer")
};

UENUM(BlueprintType)
enum class ECharacterState : uint8
{
	Walk_and_Jump UMETA(DisplayName = "Walk_and_Jump"),
	Airbone UMETA(DisplayName = "Airbone"),
	Ragdoll UMETA(DisplayName = "Ragdoll"),
	Death UMETA(DisplayName = "Death")
};

UENUM(BlueprintType)
enum class EAnimWeaponLayer : uint8
{
	None UMETA(DisplayName = "None"),
	Sword UMETA(DisplayName = "Sword"),
	Dagger UMETA(DisplayName = "Dagger"),
	Monster UMETA(DisplayName = "Monster")
};

UENUM(BlueprintType)
enum class EKnockBackType : uint8
{
	Directional UMETA(DisplayName = "Directional"),
	Radial UMETA(DisplayName = "Radial"),
	Grab UMETA(DisplayName = "Grab"),
	RadialDistance UMETA(DisplayName = "RadialDistance"),
	RadialDistanceReverse UMETA(DisplayName = "RadialDistanceReverse"),
	RadialXY UMETA(DisplayName = "RadialXY"),
	RadialXYDistance UMETA(DisplayName = "RadialXYDistance"),
	RadialXYDistanceReverse UMETA(DisplayName = "RadialXYDistanceReverse")
};

UENUM(BlueprintType)
enum class ETargetControlType : uint8
{
	None UMETA(DisplayName = "None"),
	Stun UMETA(DisplayName = "Stun"),
	Ragdoll UMETA(DisplayName = "Ragdoll")
};

UENUM(BlueprintType)
enum class EBuffAndDebuffType : uint8
{
	None UMETA(DisplayName = "None"),
	Heal UMETA(DisplayName = "Heal"),
	Poison UMETA(DisplayName = "Poison")
};

UENUM(BlueprintType)
enum class EDamageElementalType : uint8
{
	None UMETA(DisplayName = "None"),
	Fire UMETA(DisplayName = "Fire"),
	Ice UMETA(DisplayName = "Ice")
};

UENUM(BlueprintType)
enum class EAttackType : uint8
{
	Slash UMETA(DisplayName = "Slash"),
	Sting UMETA(DisplayName = "Sting"),
	Bash UMETA(DisplayName = "Bash"),
	Magic UMETA(DisplayName = "Magic"),
	Earthquake UMETA(DisplayName = "Earthquake")
};

UENUM(BlueprintType)
enum class EItemType : uint8
{
	Consumables UMETA(DisplayName = "Consumables"),
	Raw UMETA(DisplayName = "Raw"),
	Key UMETA(DisplayName = "Key"),
	Equipment_Weapon UMETA(DisplayName = "Equipment_Weapon"),
	Equipment_Head UMETA(DisplayName = "Equipment_Head"),
	Equipment_Upperbody UMETA(DisplayName = "Equipment_Upperbody"),
	Equipment_Pants UMETA(DisplayName = "Equipment_Pants"),
	Equipment_Shoes UMETA(DisplayName = "Equipment_Shoes"),
	Equipment_Gloves UMETA(DisplayName = "Equipment_Gloves")
};

UENUM(BlueprintType)
enum class EItemRank : uint8
{
	Common,
	Rare,
	Precious,
	Unique,
	Legendary
};

UENUM(BlueprintType)
enum class EEquipmentType : uint8
{
	Weapon,
	Head,
	UpperBody,
	LowerBody,
	Hand,
	Shoes,
	Artifact1,
	Artifact2,
	Artifact3,
	Artifact4
};

UENUM(BlueprintType)
enum class EActionRotateType : uint8
{
	Target UMETA(DisplayName = "Target"),
	Input UMETA(DisplayName = "Input"),
	Static UMETA(DisplayName = "Static")
};

UENUM(BlueprintType)
enum class EActionMovableType : uint8
{
	None,
	Walkable,
	Sprintable,
	Jumpable
};
