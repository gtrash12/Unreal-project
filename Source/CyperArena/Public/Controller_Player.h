// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "FollowCam_Base.h"
#include "../CustomData.h"
#include "BaseEnemy.h"
#include "Interface_PlayerController.h"

#include "Controller_Player.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class CYPERARENA_API AController_Player : public APlayerController, public IInterface_PlayerController
{
	GENERATED_BODY()
public :
	AController_Player();
protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
public :
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(BlueprintReadWrite)
		AFollowCam_Base* follow_cam;
	UPROPERTY(BlueprintReadWrite)
		float lock_on_cooltime;
	UPROPERTY(BlueprintReadWrite)
		AActor* lock_on_target;
	UPROPERTY(BlueprintReadWrite)
		AActor* interaction_target;
	UPROPERTY(BlueprintReadWrite)
		bool is_lock_on;
	UPROPERTY(BlueprintReadWrite)
		TMap<int32, FInventoryData> inventory_list;
	UPROPERTY(BlueprintReadWrite)
		TMap<FKey, FInventoryData> quickslot_list;
	UPROPERTY(BlueprintReadWrite)
		int32 max_slot_size;

	/* 인터페이스 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interface-Controller")
		void releaseLock_ON();
		virtual void releaseLock_ON_Implementation();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interface-Controller")
		void CtoS_applyDamage(AActor* __damaged_actor, FName __damage_id, AActor* __damage_causer, FName __hit_bone_name);
		virtual void CtoS_applyDamage_Implementation(AActor* __damaged_actor, FName __damage_id, AActor* __damage_causer, FName __hit_bone_name);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interface-Controller")
		void isLock_On_Target(AActor* actor, bool& result);
		virtual void isLock_On_Target_Implementation(AActor* actor, bool& result);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interface-Controller")
		AActor* getLockOnTargetActor();
		AActor* getLockOnTargetActor_Implementation();
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Interface-Controller")
		void getItem(FName __item_id, int32 __num);
		virtual void getItem_Implementation(FName __item_id, int32 __num);
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Interface-Controller")
		int32 findInventoryEmptyIndex();
		virtual int32 findInventoryEmptyIndex_Implementation();
		UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Interface-Controller")
			int32 findSameItem(FName __item_id);
			virtual int32 findSameItem_Implementation(FName __item_id);
	/* 클래스 메소드 */
	UFUNCTION(BlueprintCallable, Category = "Targetting")
		AActor* findLockOnTarget();
	UFUNCTION(BlueprintCallable, Category = "Targetting")
		AActor* changeLockOnTarget(float __direction);
	UFUNCTION(BlueprintCallable, Category = "Targetting")
		void lockOnEvent();
	UFUNCTION(BlueprintCallable, Category = "Targetting")
		void changeLockOnAxisEvent(float __axis_value);
	UFUNCTION(BlueprintCallable, Category = "Targetting")
		bool isStackable(EItemType __item_type);

	UFUNCTION(BlueprintCallable, Server, Reliable)
		void Server_ApplyDamage(AActor* __damaged_actor, FName __damage_id, AActor* __damage_causer, FName __hit_bone_name);
		virtual void Server_ApplyDamage_Implementation(AActor* __damaged_actor, FName __damage_id, AActor* __damage_causer, FName __hit_bone_name);
};
