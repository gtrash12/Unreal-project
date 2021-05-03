// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"

#include "CustomData.generated.h"


UENUM(BlueprintType)
enum class CharacterState : uint8
{
	TE_OptionA UMETA(DisplayName = "Walk_and_Jump"),
	TE_OptionB UMETA(DisplayName = "Airbone"),
	TE_OptionC UMETA(DisplayName = "Ragdoll"),
	TE_OptionD UMETA(DisplayName = "Down")
};

UENUM(BlueprintType)
enum class AnimWeaponLayer : uint8
{
	TE_OptionA UMETA(DisplayName = "None"),
	TE_OptionB UMETA(DisplayName = "Sword"),
	TE_OptionC UMETA(DisplayName = "Dagger"),
	TE_OptionD UMETA(DisplayName = "Monster")
};

UENUM(BlueprintType)
enum class FkncokBackType : uint8
{
	TE_OptionA UMETA(DisplayName = "Directional"),
	TE_OptionB UMETA(DisplayName = "Radial"),
	TE_OptionC UMETA(DisplayName = "Grab")
};

UENUM(BlueprintType)
enum class FtargetControlType : uint8
{
	TE_OptionA UMETA(DisplayName = "None"),
	TE_OptionB UMETA(DisplayName = "Stun"),
	TE_OptionC UMETA(DisplayName = "Ragdoll")
};

UENUM(BlueprintType)
enum class FbuffAndDebuff : uint8
{
	TE_OptionA UMETA(DisplayName = "None"),
	TE_OptionB UMETA(DisplayName = "Heal"),
	TE_OptionC UMETA(DisplayName = "Poison")
};

UENUM(BlueprintType)
enum class FdamageElemental : uint8
{
	TE_OptionA UMETA(DisplayName = "None"),
	TE_OptionB UMETA(DisplayName = "Fire"),
	TE_OptionC UMETA(DisplayName = "Ice")
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
		FkncokBackType knock_back_type;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		FdamageElemental elemental;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		FtargetControlType target_control;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		FbuffAndDebuff buff_and_debuff;

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