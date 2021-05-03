// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "../CustomData.h"

#include "Interface_BaseCharacter.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInterface_BaseCharacter : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class CYPERARENA_API IInterface_BaseCharacter
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, meta = (Category = "Base-Interface", OverrideNativeName = "rotateActorTimeline"))
		void rotateActorTimeline(FRotator target_rotation, float time);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, meta = (Category = "Base-Interface", OverrideNativeName = "look"))
		void look();
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, meta = (Category = "Base-Interface", OverrideNativeName = "getWeapon"))
		void getWeapon(/*out*/ USkeletalMeshComponent*& weapon);
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, meta = (Category = "Base-Interface", OverrideNativeName = "getDamageData"))
		void getDamageData(/*out*/ FdamageData& damage_data);
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, meta = (Category = "Base-Interface", OverrideNativeName = "setDamageData"))
		void setDamageData(FdamageData damage_data);
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, meta = (Category = "Base-Interface", OverrideNativeName = "rotateActorInterp"))
		void rotateActorInterp(FRotator target_rotation, float delta_time, float speed);
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, meta = (Category = "Base-Interface", OverrideNativeName = "getLookRotation"))
		void getLookRotation(/*out*/ FRotator& look_rotation);
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, meta = (Category = "Base-Interface", OverrideNativeName = "getTargetRotation"))
		void getTargetRotation(/*out*/ FRotator& target_rotation);
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, meta = (Category = "Base-Interface", OverrideNativeName = "knock_Back"))
		void knock_Back(FVector velocity);
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, meta = (Category = "Base-Interface", OverrideNativeName = "resetHitActorList"))
		void resetHitActorList();
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, meta = (Category = "Base-Interface", OverrideNativeName = "applyDamage"))
		void applyDamage(FdamageData damage_data, AActor* damage_causor);
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, meta = (Category = "Base-Interface", OverrideNativeName = "setPrevSockLoc"))
		void setPrevSockLoc(FVector start, FVector end);
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, meta = (Category = "Base-Interface", OverrideNativeName = "getPrevSockLoc"))
		void getPrevSockLoc(/*out*/ FVector& start, /*out*/ FVector& end);
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, meta = (Category = "Base-Interface", OverrideNativeName = "attackEvent"))
		void attackEvent(const AActor* hit_actor__const);
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, meta = (Category = "Base-Interface", OverrideNativeName = "getAttackTraceChannel"))
		void getAttackTraceChannel(/*out*/ TEnumAsByte<ETraceTypeQuery>& NewParam);
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, meta = (Category = "Base-Interface", OverrideNativeName = "ResetNextAttack"))
		void ResetNextAttack();
};
