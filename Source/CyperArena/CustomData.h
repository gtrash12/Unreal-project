// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"

#include "CustomData.generated.h"

UENUM(BlueprintType)
enum class ENetworkOwnerType : uint8
{
	ServerOwned UMETA(DisplayName = "ServerOwned"),
	ListenServerNotOwned UMETA(DisplayName = "ListenServerNotOwned"),
	DedicatedServer UMETA(DisplayName = "DedicatedServer"),
	ClientOwned UMETA(DisplayName = "ClientOwned"),
	ClientNotOwned UMETA(DisplayName = "ClientNotOwned")
};

UENUM(BlueprintType)
enum class ECharacterState : uint8
{
	Walk_and_Jump UMETA(DisplayName = "Walk_and_Jump"),
	Airbone UMETA(DisplayName = "Airbone"),
	Ragdoll UMETA(DisplayName = "Ragdoll")
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
enum class EKncokBackType : uint8
{
	Directional UMETA(DisplayName = "Directional"),
	Radial UMETA(DisplayName = "Radial"),
	Grab UMETA(DisplayName = "Grab")
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
		EKncokBackType knock_back_type;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		EDamageElementalType elemental;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		ETargetControlType target_control;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		EBuffAndDebuffType buff_and_debuff;

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