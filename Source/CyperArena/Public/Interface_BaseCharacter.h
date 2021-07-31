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
		void applyDamage(FName __target_damage_id, AActor* __damage_causer, FName __hit_bone_name, FVector __hit_location);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Base-Interface")
		void setDamageData(FdamageData __target_damage_data);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Base-Interface")
		void setDamageID(FName __target_damage_id);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Base-Interface")
		void resetHitActorList();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Base-Interface")
		void attackEvent(AActor* __hit_actor, FHitResult __hit_result);
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
		ETraceTypeQuery getAttackTraceChannel();
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
		void applyKnock_Back(FVector __velocity);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Base-Interface")
		void rotateActorWithInTime(FRotator __target_rotation, float __time);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Base-Interface")
		void getBasePower(float& __output_base_power);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Base-Interface")
		void setNextAttackID(FName __next_action_id);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Base-Interface")
		void getDurabilityLevel(uint8& __output_durability_level);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Base-Interface")
		void setIsDodge(bool __target_is_dodge);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Base-Interface")
		void getIsDodge(bool& __output_is_dodge);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Base-Interface")
		void useItem(FName __item_id);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Base-Interface")
		void recoverHP(float __amount);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Base-Interface")
		void recoverStaminaMax(float __amount);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Base-Interface")
		void increaseStat_Hp(float __amount);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Base-Interface")
		void increaseStat_Stamina(float __amount);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Base-Interface")
		void increaseStat_Speed(float __amount);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Base-Interface")
		void increaseStat_Power(float __amount);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Base-Interface")
		float getStat_Speed();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Base-Interface")
		float getStat_Hp();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Base-Interface")
		float getStat_Stamina();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Base-Interface")
		void changeWeaponMesh(FName __item_id);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Base-Interface")
		void ItemEffect_onRegistration(FName __item_id, int32 __inven_index);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Base-Interface")
		void ItemEffect_onRemoveRegistration(FName __item_id, int32 __inven_index);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Base-Interface")
		void ItemEffect_onActivate(FName __item_id, int32 __inven_index);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Base-Interface")
		void spawnAttackDecal(FHitResult __target, FTransform __decal_transform);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Base-Interface")
		void getWeapon(FName __key, /*out*/ UPrimitiveComponent*& __weapon);
};
