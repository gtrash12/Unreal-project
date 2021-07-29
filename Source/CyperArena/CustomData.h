// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Public/CustomEnums.h"
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include "Engine/Texture.h"
#include "Public/BaseItemEffect.h"

#include "CustomData.generated.h"

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
		float durability_level;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		FVector knock_back_offset;

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
	/* 라스트 인풋으로 회전하는데 걸리는 시간 */
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		float rotate_time;
	/* 액션 시 회전 방향 결정 */
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		EActionRotateType action_rotate_type;
};

USTRUCT(Atomic, BlueprintType)
struct FItemEffect
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		float value;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		TSubclassOf<UBaseItemEffect> item_effect;
};

USTRUCT(Atomic, BlueprintType)
struct FItemData : public FTableRowBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		FText name;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		UTexture2D* icon;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		EItemType item_type;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		EItemRank item_rank;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, meta = (MultiLine="true"))
		FText item_info_text;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		TArray<FItemEffect> item_effect_list;
};

USTRUCT(Atomic, BlueprintType)
struct FItemMeshData
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		FName component_id;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		USkeletalMesh* mesh;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		FTransform transform;
};

USTRUCT(Atomic, BlueprintType)
struct FItemMeshTableData : public FTableRowBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		TArray<FItemMeshData> item_mesh_data;
};

USTRUCT(Atomic, BlueprintType)
struct FInventoryData
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		FName item_id;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		int32 count;
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