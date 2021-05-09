// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "../CustomData.h"

#include "Interface_BaseCharacter.generated.h"

// This class does not need to be modified.
UINTERFACE(BlueprintType)
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
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Base-Interface")
		void setLookRotation();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Base-Interface")
		void applyDamage(FdamageData __damage_data, AActor* __damage_causor);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Base-Interface")
		void setDamageData(FdamageData __target_damage_data);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Base-Interface")
		void resetHitActorList();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Base-Interface")
		void attackEvent(AActor* __hit_actor);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Base-Interface")
		void resetNextAttack(bool __is_on_action__toggle);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Base-Interface")
		void getDamageData(/*out*/ FdamageData& __damage_data);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Base-Interface")
		void getLookRotation(/*out*/ FRotator& __look_rotation);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Base-Interface")
		void getTargetRotation(/*out*/ FRotator& __target_rotation);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Base-Interface")
		void setPrevSockLoc(FVector __start, FVector __end);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Base-Interface")
		void getPrevSockLoc(/*out*/ FVector& __start, /*out*/ FVector& __end);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Base-Interface")
		void getAttackTraceChannel(/*out*/ TEnumAsByte<ETraceTypeQuery>& __attack_trace_channel);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Base-Interface")
		void rotateActorInterp(FRotator __target_rotation, float __delta_time, float __speed);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Base-Interface")
		void getCharacterState(/*out*/ ECharacterState& __output_character_state);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Base-Interface")
		void getIsOnSprint(/*out*/ bool& __output_is_on_sprint);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Base-Interface")
		void setCharacterState(ECharacterState target_character_state);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Base-Interface")
		void getCurrentVelocity(FVector& __output_current_velocity);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Base-Interface")
		void getLookDirection(float& __output_look_pitch, float& __output_look_yaw);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Base-Interface")
		void setNextAttackMontage(UAnimMontage* __next_attack_anim);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Base-Interface")
		void setIsOnAction(bool __target_is_on_action);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Base-Interface")
		void rotateActorTimeline(FRotator __target_rotation, float __time);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Base-Interface")
		void applyKnock_Back(FVector __velocity);
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Base-Interface")
		void getWeapon(/*out*/ UMeshComponent*& __weapon);
};
