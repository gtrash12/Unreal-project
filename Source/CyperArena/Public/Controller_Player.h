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
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		TMap<int32, FInventoryData> inventory_list;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		TMap<FKey, int32> quickslot_list;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		TMap<int32, FKey> reverse_quickslot_list;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		TMap<EEquipmentType, FInventoryData> equipment_list;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
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
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interface-Controller")
		void getItem(FName __item_id, int32 __num);
		virtual void getItem_Implementation(FName __item_id, int32 __num);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interface-Controller")
		int32 findInventoryEmptyIndex();
		virtual int32 findInventoryEmptyIndex_Implementation();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interface-Controller")
		int32 findSameItem(FName __item_id);
		virtual int32 findSameItem_Implementation(FName __item_id);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interface-Controller")
		bool getIsLockOn();
		virtual bool getIsLockOn_Implementation();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interface-Controller")
		AFollowCam_Base* getFollowCam();
		virtual AFollowCam_Base* getFollowCam_Implementation();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interface-Controller")
		void showInteractionText(const FText& __text);
		virtual void showInteractionText_Implementation(const FText& __text);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interface-Controller")
		void removeInteractionText();
		virtual void removeInteractionText_Implementation();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Inventory")
		void swapInvenSlot(int32 __from, int32 __to);
		virtual void swapInvenSlot_Implementation(int32 __from, int32 __to);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Inventory")
		void swapQuickSlot(FKey __from, FKey __to);
		virtual void swapQuickSlot_Implementation(FKey __from, FKey __to);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Inventory")
		void registerInventoQuick(int32 __from, FKey __to);
		virtual void registerInventoQuick_Implementation(int32 __from, FKey __to);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Inventory")
		FInventoryData getInventoryData(int32 __index);
		virtual FInventoryData getInventoryData_Implementation(int32 __index);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interface-Controller")
		bool isInteractionTarget(AActor* __actor);
		virtual bool isInteractionTarget_Implementation(AActor* __actor);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Inventory")
		void removeQuickSlot(FKey __key);
		virtual void removeQuickSlot_Implementation(FKey __key);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Inventory")
		bool isRegisteredQuickSlotKey(FKey __key);
		virtual bool isRegisteredQuickSlotKey_Implementation(FKey __key);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Inventory")
		int32 getRegisteredQuickSlotInvenIndex(FKey __key);
		virtual int32 getRegisteredQuickSlotInvenIndex_Implementation(FKey __key);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Inventory")
		void decreseItem(int32 __index, int32 __decrease_num);
		virtual void decreseItem_Implementation(int32 __index, int32 __decrease_num);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Inventory")
		void equipItem(int32 __from, EEquipmentType __to);
		virtual void equipItem_Implementation(int32 __from, EEquipmentType __to);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Inventory")
		void unequipItem(EEquipmentType __from, int32 __to);
		virtual void unequipItem_Implementation(EEquipmentType __from, int32 __to);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Inventory")
		void refreshEquipmentSlot(EEquipmentType __type);
		virtual void refreshEquipmentSlot_Implementation(EEquipmentType __type);
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
	UFUNCTION(BlueprintCallable)
		void refreshQuickSlot(FKey __key);
	UFUNCTION(BlueprintCallable)
		void updateQuickSlotData(int32 __from, int32 __to);

	UFUNCTION(BlueprintCallable, Server, Reliable)
		void Server_ApplyDamage(AActor* __damaged_actor, FName __damage_id, AActor* __damage_causer, FName __hit_bone_name);
		virtual void Server_ApplyDamage_Implementation(AActor* __damaged_actor, FName __damage_id, AActor* __damage_causer, FName __hit_bone_name);
};
