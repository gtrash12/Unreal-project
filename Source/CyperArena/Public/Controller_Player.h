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
UCLASS()
class CYPERARENA_API AController_Player : public APlayerController, public IInterface_PlayerController
{
	GENERATED_BODY()
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

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		void releaseLock_ON();
		virtual void releaseLock_ON_Implementation();

	UFUNCTION(BlueprintCallable, Category = "Targetting")
		AActor* findLockOnTarget();
	UFUNCTION(BlueprintCallable, Category = "Targetting")
		AActor* changeLockOnTarget(float __direction);
	UFUNCTION(BlueprintCallable, Category = "Targetting")
		void lockOnEvent();
	UFUNCTION(BlueprintCallable, Category = "Targetting")
		void changeLockOnAxisEvent(float __axis_value);
};
