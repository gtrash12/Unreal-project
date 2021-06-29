// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseEnemy.h"
#include "GameFramework/CharacterMovementComponent.h"

ABaseEnemy::ABaseEnemy()
{
	/*static ConstructorHelpers::FObjectFinder<UBehaviorTree> BT(TEXT("BehaviorTree'/Game/Blueprint/AI/BT_Enemy.BT_Enemy'"));
	if (BT.Succeeded())
	{
		behavior_tree = BT.Object;
	}*/
}

void ABaseEnemy::BeginPlay()
{
	Super::BeginPlay();
	GetCharacterMovement()->MaxWalkSpeed = walk_speed;
}


void ABaseEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABaseEnemy::applyDamage_Multicast_Exec_Implementation(FName __target_damage_id, AActor* damage_causer, FName __hit_bone_name) {
	Super::applyDamage_Multicast_Exec_Implementation(__target_damage_id, damage_causer, __hit_bone_name);
	target_actor = damage_causer;
	//Cast<Controller>(GetController()
}