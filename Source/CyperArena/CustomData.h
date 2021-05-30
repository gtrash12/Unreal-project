// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"

#include "CustomData.generated.h"

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

USTRUCT(Atomic, BlueprintType)
struct FdamageData : public FTableRowBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		FString name;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		float base_damage;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		float base_damage_percent;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		FVector knock_back;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		EKnockBackType knock_back_type;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		EDamageElementalType elemental;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		ETargetControlType target_control;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		EBuffAndDebuffType buff_and_debuff;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		EAttackType attack_type;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		uint8 durability_level;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		FVector knock_back_offset;

};

UENUM(BlueprintType)
enum class EActionRotateType : uint8
{
	Target UMETA(DisplayName = "Target"),
	Input UMETA(DisplayName = "Input"),
	Static UMETA(DisplayName = "Static")
};

USTRUCT(Atomic, BlueprintType)
struct FActionData : public FTableRowBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		FString name;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		UAnimMontage* anim_montage;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		float stamina;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		float stamina_penalty;
	/// <summary>
	/// 라스트 인풋으로 회전하는데 걸리는 시간
	/// </summary>
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		float rotate_time;

	/// <summary>
	/// 액션 시 회전 방향 결정
	/// </summary>
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		EActionRotateType action_rotate_type;
};

/*
UCLASS()
class CYPERARENA_API ACustomData : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACustomData();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
*/