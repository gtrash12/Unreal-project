// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../BaseCharacter.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Components/Widget.h"
#include "Interface_AI.h"

#include "BaseEnemy.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class CYPERARENA_API ABaseEnemy : public ABaseCharacter, public IInterface_AI
{
	GENERATED_BODY()

public :
	ABaseEnemy();

	//캐릭터 결정부
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "BaseEnemy")
		FName name;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "BaseEnemy")
		class UBehaviorTree* behavior_tree;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "BaseEnemy")
		TArray<UAnimInstance*> attack_montages;

	// 내부동작 필요변수
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "BaseEnemy")
		UWidget* state_bar;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "BaseEnemy")
		FVector look_location;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "BaseEnemy")
		float down_time;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "BaseEnemy")
		AActor* target_actor;
protected:
	virtual void BeginPlay() override;
public:
	virtual void Tick(float DeltaTime) override;

	virtual void applyDamage_Multicast_Exec_Implementation(FName __target_damage_id, AActor* damage_causer, FName __hit_bone_name) override;
};
