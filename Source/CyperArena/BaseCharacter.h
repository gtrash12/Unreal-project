// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Public/Interface_BaseCharacter.h"
#include "CustomData.h"

#include "BaseCharacter.generated.h"

UCLASS(BlueprintType)
class CYPERARENA_API ABaseCharacter : public ACharacter, public IInterface_BaseCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseCharacter();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Base-Ragdoll")
		UPhysicsHandleComponent* ragdoll_physics_handle;
	UPROPERTY(BlueprintReadWrite, meta = (DisplayName = "Look Yaw", Category = "Base-Look"))
		float look_yaw;
	UPROPERTY(BlueprintReadWrite, meta = (DisplayName = "Look Pitch", Category = "Base-Look"))
		float look_pitch;
	UPROPERTY(BlueprintReadWrite, Replicated, meta = (DisplayName = "R Look Rotation", Category = "Base-Look"))
		FRotator R_look_rotation;
	UPROPERTY(BlueprintReadWrite, meta = (Category = "Base-Look"))
		FRotator rotate_target_rotation;
	UPROPERTY(BlueprintReadWrite, meta = (Category = "Base-Look"))
		float rotate_interp_time_end;
	UPROPERTY(BlueprintReadWrite, meta = (Category = "Base-Look"))
		float rotate_interp_time;
	UPROPERTY(BlueprintReadWrite, meta = (Category = "Base-Look"))
		FRotator rotate_original_rotation;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (DisplayName = "Character State", Category = "Base-CharacterState"))
		ECharacterState character_state;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Replicated, Category = "Base-CharacterState")
		float hp;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Base-CharacterState")
		float max_hp;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Replicated, Category = "Base-CharacterState")
		float stamina;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Replicated, Category = "Base-CharacterState")
		float cur_max_stamina;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Base-CharacterState")
		float max_stamina;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, meta = (DisplayName = "Base Power", Category = "Base-CharacterState"))
		float base_power;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (DisplayName = "Is On Sprint", Category = "Base-CharacterState"))
		bool is_on_sprint;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Base-CharacterState")
		float walk_speed;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Base-CharacterState")
		float sprint_speed;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (DisplayName = "Is Simulation Owner", Category = "Base-Ragdoll"))
		bool is_simulation_responsible;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Replicated, meta = (DisplayName = "Simulation Owner Actor", Category = "Base-Ragdoll"))
		AActor* simulation_responsible_actor;
	UPROPERTY(BlueprintReadWrite, meta = (DisplayName = "Is Ragdoll on the Ground", Category = "Base-Ragdoll"))
		bool is_ragdoll_on_the_ground;
	UPROPERTY(BlueprintReadWrite, meta = (DisplayName = "Is Ragdoll Face Up", Category = "Base-Ragdoll"))
		bool is_ragdoll_face_up;
	UPROPERTY(BlueprintReadWrite, Replicated, meta = (DisplayName = "Ragdoll Server Location", Category = "Base-Ragdoll"))
		FVector ragdoll_server_location;
	UPROPERTY(BlueprintReadWrite, meta = (DisplayName = "Last Ragdoll Server Location", Category = "Base-Ragdoll"))
		FVector last_ragdoll_server_location;
	UPROPERTY(BlueprintReadWrite, meta = (DisplayName = "Prev Ragdoll Server Location", Category = "Base-Ragdoll"))
		FVector prev_ragdoll_server_location;
	UPROPERTY(BlueprintReadWrite, meta = (DisplayName = "Replication Delay Count", Category = "Base-Ragdoll"))
		float replication_delay_count;
	UPROPERTY(BlueprintReadWrite, meta = (DisplayName = "Last Replication Delay", Category = "Base-Ragdoll"))
		float last_replication_delay;
	UPROPERTY(BlueprintReadWrite, meta = (DisplayName = "Main Anim Instance", Category = "Base-Ragdoll"))
		UAnimInstance* main_anim_instance;
	UPROPERTY()
		UAnimMontage* airbone_b_anim;
	UPROPERTY()
		UAnimMontage* get_up_f_anim;
	UPROPERTY()
		UAnimMontage* get_up_b_anim;
	UPROPERTY()
		UAnimMontage* hit_f_anim;
	UPROPERTY()
		UAnimMontage* hit_b_anim;
	UPROPERTY()
		UAnimMontage* hit_r_anim;
	UPROPERTY()
		UAnimMontage* hit_l_anim;
	UPROPERTY()
		USoundBase* sq_hit;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(Category = "Base-Network"))
		ENetworkOwnerType network_owner_type;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (DisplayName = "IS On Action", Category = "Base-Combat"))
		bool is_on_action;
	UPROPERTY(BlueprintReadWrite, meta = (Category = "Base-Combat"))
		float knock_back_speed;
	UPROPERTY(BlueprintReadWrite, meta = (Category = "Base-Combat"))
		FVector knock_back_unit_vector;
	UPROPERTY(BlueprintReadWrite, meta = (Category = "Base-Combat"))
		FVector current_velocty;
	UPROPERTY(BlueprintReadWrite, meta = (Category = "Base-Combat"))
		TSet<AActor*> hit_actors_list;
	UPROPERTY(BlueprintReadWrite, meta = (Category = "Base-Combat"))
		FdamageData damage_data;
	UPROPERTY(BlueprintReadWrite, meta = (Category = "Base-Combat"))
		FName damage_id;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (Category = "Base-Combat"))
		UAnimMontage* normal_attack_montage;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (Category = "Base-Combat"))
		UAnimMontage* next_attack_montage;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Base-Combat")
		FName normal_attack_id;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Base-Combat")
		FName next_attack_id;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Base-Combat")
		FName normal_smash_id;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Base-Combat")
		FName next_smash_id;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Base-Combat")
		FName dash_attack_id;
	UPROPERTY(BlueprintReadWrite, Category = "Base-Combat")
		FVector prev_attack_sock_start_loc;
	UPROPERTY(BlueprintReadWrite, Category = "Base-Combat")
		FVector prev_attack_sock_end_loc;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Base-Combat")
		TMap<FName, UPrimitiveComponent*> attack_collisions;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Base-Combat")
		float durability_level = 1;
	UPROPERTY(BlueprintReadWrite, Category = "Base-Combat")
		bool is_dodge = 0;

private :
	UPROPERTY()
		float d_time;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void PostInitializeComponents() override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

// --> Interface_BaseCharacter 인터페이스의 함수들 오버라이드 선언

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Base-Interface")
		void setLookRotation();
		virtual void setLookRotation_Implementation() override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Base-Interface")
		void applyDamage(FName __target_damage_id, AActor* __damage_causor, FName __hit_bone_name);
		virtual void applyDamage_Implementation(FName __target_damage_id, AActor* __damage_causor, FName __hit_bone_name) override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Base-Interface")
		void setDamageData(FdamageData __target_damage_data);
		virtual void setDamageData_Implementation(FdamageData __target_damage_data) override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Base-Interface")
		void setDamageID(FName __target_damage_id);
		virtual void setDamageID_Implementation(FName __target_damage_id) override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Base-Interface")
		void resetHitActorList();
		virtual void resetHitActorList_Implementation() override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Base-Interface")
		void attackEvent(AActor* __hit_actor, FName __hit_bone_name);
		virtual void attackEvent_Implementation(AActor* __hit_actor, FName __hit_bone_name) override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Base-Interface")
		void resetNextAttack(bool __is_on_action__toggle);
		virtual void resetNextAttack_Implementation(bool __is_on_action__toggle) override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Base-Interface")
		void getDamageData(/*out*/ FdamageData& __damage_data);
		virtual void getDamageData_Implementation(FdamageData& __damage_data) override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Base-Interface")
		void getLookRotation(/*out*/ FRotator& __look_rotation);
		virtual void getLookRotation_Implementation(/*out*/ FRotator& __look_rotation) override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Base-Interface")
		void getTargetRotation(/*out*/ FRotator& __target_rotation);
		virtual void getTargetRotation_Implementation(/*out*/ FRotator& __target_rotation) override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Base-Interface")
		void setPrevSockLoc(FVector __start, FVector __end);
		virtual void setPrevSockLoc_Implementation(FVector __start, FVector __end) override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Base-Interface")
		void getPrevSockLoc(/*out*/ FVector& __start, /*out*/ FVector& __end);
		virtual void getPrevSockLoc_Implementation(/*out*/ FVector& __start, /*out*/ FVector& __end) override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Base-Interface")
		void getAttackTraceChannel(/*out*/ TEnumAsByte<ETraceTypeQuery>& __attack_trace_channel);
		virtual void getAttackTraceChannel_Implementation(/*out*/ TEnumAsByte<ETraceTypeQuery>& __attack_trace_channel) override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Base-Interface")
		void rotateActorInterp(FRotator __target_rotation, float __delta_time, float __speed);
		virtual void rotateActorInterp_Implementation(FRotator __target_rotation, float __delta_time, float __speed) override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Base-Interface")
		void getCharacterState(/*out*/ ECharacterState& __output_character_state);
		virtual void getCharacterState_Implementation(/*out*/ ECharacterState& __output_character_state) override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Base-Interface")
		void getIsOnSprint(/*out*/ bool& __output_is_on_sprint);
		virtual void getIsOnSprint_Implementation(/*out*/ bool& __output_is_on_sprint) override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, meta = (Category = "Base-Interface"))
		void setCharacterState(ECharacterState target_character_state);
		virtual void setCharacterState_Implementation(ECharacterState target_character_state)override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Base-Interface")
		void getCurrentVelocity(FVector& __output_current_velocity);
		virtual void getCurrentVelocity_Implementation(FVector& __output_current_velocity) override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Base-Interface")
		void getLookDirection(float& __output_look_pitch, float& __output_look_yaw);
		virtual void getLookDirection_Implementation(float& __output_look_pitch, float& __output_look_yaw) override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Base-Interface")
		void setNextAttackMontage(UAnimMontage* __next_attack_anim);
		virtual void setNextAttackMontage_Implementation(UAnimMontage* __next_attack_anim) override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Base-Interface")
		void setIsOnAction(bool __target_is_on_action);
		virtual void setIsOnAction_Implementation(bool __target_is_on_action) override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, meta = (Category = "Base-Interface"))
		void applyKnock_Back(FVector velocity);
		virtual void applyKnock_Back_Implementation(FVector velocity)override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Base-Interface")
		void rotateActorWithInTime(FRotator __target_rotation, float __time);
		virtual void rotateActorWithInTime_Implementation(FRotator __target_rotation, float __time)override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Base-Interface")
		void getBasePower(float& __output_base_power);
		virtual void getBasePower_Implementation(float& __output_base_power)override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Base-Interface")
		void setNextAttackID(FName __next_action_id);
		virtual void setNextAttackID_Implementation(FName __next_action_id) override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Base-Interface")
		void getWeapon(FName __key, /*out*/ UPrimitiveComponent*& __weapon);
		virtual void getWeapon_Implementation(FName __key, /*out*/ UPrimitiveComponent*& __weapon) override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Base-Interface")
		void getDurabilityLevel(uint8& __output_durability_level);
		virtual void getDurabilityLevel_Implementation(uint8& __output_durability_level) override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Base-Interface")
		void setIsDodge(bool __target_is_dodge);
		virtual void setIsDodge_Implementation(bool __target_is_dodge) override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Base-Interface")
		void getIsDodge(bool& __output_is_dodge);
		virtual void getIsDodge_Implementation(bool& __output_is_dodge) override;
// --> 클래스 멤버 함수선언


	/*UFUNCTION(BlueprintCallable, meta = (Category = "Base", CppFromBpEvent, OverrideNativeName = "rotateActorTimeline"))
	virtual void rotateActorTimeline(FRotator bpp__target_rotation__pf, float bpp__time__pf);*/

	UFUNCTION(BlueprintCallable, meta = (Category = "Base-Ragdoll", OverrideNativeName = "Stick to the Ground"))
		virtual void stickToTheGround(FVector location);

	UFUNCTION(BlueprintCallable, Server, UnReliable)
		void CtoS_targetLocation(ABaseCharacter* target_actor, FVector target_location);
		virtual void CtoS_targetLocation_Implementation(ABaseCharacter* target_actor, FVector target_location);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, meta = (Category = "Base-Ragdoll"))
		void ragdoll_ClientOnwer();
		virtual void ragdoll_ClientOnwer_Implementation();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, meta = (Category = "Base-Ragdoll"))
		void ragdoll_ServerOnwer();
		virtual void ragdoll_ServerOnwer_Implementation();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, meta = (Category = "Base-Ragdoll"))
		void ragdoll_SyncLocation();
		virtual void ragdoll_SyncLocation_Implementation();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, meta = (Category = "Base-Ragdoll"))
		void airboneStart(FVector __velocity);
		virtual void airboneStart_Implementation(FVector __velocity);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, meta = (Category = "Base-Ragdoll"))
		void ragdollGetUp();
		virtual void ragdollGetUp_Implementation();

	UFUNCTION(BlueprintCallable, Server, UnReliable)
		void ragdoll_SetOnServer();
		virtual void ragdoll_SetOnServer_Implementation();

	UFUNCTION(BlueprintCallable, NetMulticast, UnReliable)
		void ragdoll_SetMultiCast(AActor* responsible_actor);
		virtual void ragdoll_SetMultiCast_Implementation(AActor* responsible_actor);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, meta = (Category = "Base-Character State"))
		void findClosestPlayer(/*out*/ AActor*& closest_player);
		virtual void findClosestPlayer_Implementation(AActor*& closest_player);

	UFUNCTION(BlueprintCallable, meta = (Category = "Base-Server"))
		virtual void getNetworkOwnerType(/*out*/ ENetworkOwnerType& output);

	UFUNCTION()
		virtual void onCapsuleComponentHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION(BlueprintCallable, NetMulticast, UnReliable)
		void applyDamage_Multicast(FName __target_damage_id, AActor* damage_causor, FName __hit_bone_name);
		virtual void applyDamage_Multicast_Implementation(FName __target_damage_id, AActor* damage_causor, FName __hit_bone_name);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, meta = (Category = "Base-Combat"))
		void applyDamage_Multicast_Exec(FName __target_damage_id, AActor* damage_causor, FName __hit_bone_name);
		virtual void applyDamage_Multicast_Exec_Implementation(FName __target_damage_id, AActor* damage_causor, FName __hit_bone_name);

	UFUNCTION(BlueprintCallable, NetMulticast, UnReliable)
		void animation_Sound_Multicast(UAnimMontage* anim, USoundBase* sound);
		virtual void animation_Sound_Multicast_Implementation(UAnimMontage* anim, USoundBase* sound);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, meta = (Category = "Base-Combat"))
		void selectHitAnimation(FVector velocity, /*out*/UAnimMontage*& hit_anim);
		virtual void selectHitAnimation_Implementation(FVector velocity, UAnimMontage*& hit_anim);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, meta = (Category = "Base-Tick"))
		void knock_BackProcess();
		virtual void knock_BackProcess_Implementation();

	UFUNCTION(BlueprintCallable, Server, UnReliable)
		void CtoS_setRotation(FRotator __target_rotation);
		virtual void CtoS_setRotation_Implementation(FRotator __target_rotation);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, meta = (Category = "Base-Combat"))
		void onWeaponBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
		virtual void onWeaponBeginOverlap_Implementation(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, meta = (Category = "Base-Tick"))
		void rotateProcess();
		virtual void rotateProcess_Implementation();

		virtual bool airbone_HitChk(FVector __velocity, FVector& __hitnormal);

	UFUNCTION(BlueprintCallable, Server, UnReliable)
		void CtoS_setCharacterState(ECharacterState __target_character_state);
		virtual void CtoS_setCharacterState_Implementation(ECharacterState __target_character_state);

	UFUNCTION(BlueprintCallable, NetMulticast, UnReliable)
		void Multicast_setCharacterState(ECharacterState __target_character_state);
		virtual void Multicast_setCharacterState_Implementation(ECharacterState __target_character_state);
};
