// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "../CustomData.h"

#include "Interface_PlayerController.generated.h"

// This class does not need to be modified.
UINTERFACE(BlueprintType)
class UInterface_PlayerController : public UInterface
{
	GENERATED_BODY()
};

/**
 *
 */
class CYPERARENA_API IInterface_PlayerController
{
	GENERATED_BODY()

		// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Interface-Controller")
		void releaseLock_ON();
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Interface-Controller")
		void isLock_On_Target(AActor* actor, bool& result);
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Interface-Controller")
		void CtoS_applyDamage(const AActor* damaged_actor, FName __damage_id, AActor* damage_causer, FName __hit_bone_name);
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Interface-Controller")
		AActor* getLockOnTargetActor();
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Interface-Controller")
		void getItem(FName __item_id, int32 __num);
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Interface-Controller")
		int32 findInventoryEmptyIndex();
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Interface-Controller")
		int32 findSameItem(FName __item_id);
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Interface-Controller")
		bool getIsLockOn();
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Interface-Controller")
		bool getHUDReference();
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Interface-Controller")
		AFollowCam_Base* getFollowCam();
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Interface-Controller")
		void showInteractionText(const FText& __text);
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Interface-Controller")
		void removeInteractionText();
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Interface-Controller")
		void swapInvenSlot(int32 __from, int32 __to);
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Interface-Controller")
		void swapQuickSlot(FKey __from, FKey __to);
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Interface-Controller")
		void registerInventoQuick(int32 __from, FKey __to);
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Interface-Controller")
		FInventoryData getInventoryData(int32 __index);
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Interface-Controller")
		bool isInteractionTarget(AActor* __actor);
};
