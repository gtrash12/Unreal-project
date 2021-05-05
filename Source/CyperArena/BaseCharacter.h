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

	UPROPERTY(BlueprintReadOnly, NonTransactional, meta = (Category = "Base-Ragdoll", OverrideNativeName = "PhysicsHandle"))
		UPhysicsHandleComponent* ragdoll_physics_handle;
	UPROPERTY(BlueprintReadWrite, meta = (DisplayName = "Look Yaw", Category = "Base-Look"))
		float look_yaw;
	UPROPERTY(BlueprintReadWrite, meta = (DisplayName = "Look Pitch", Category = "Base-Look"))
		float look_pitch;
	UPROPERTY(BlueprintReadWrite, Replicated, meta = (DisplayName = "R Look Rotation", Category = "Base-Look"))
		FRotator R_look_rotation;
	UPROPERTY(BlueprintReadWrite, meta = (Category = "Base-Look"))
		FRotator target_rotation;
	UPROPERTY(BlueprintReadWrite, meta = (Category = "Base-Look"))
		float rotation_interp_speed;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (DisplayName = "Character State", Category = "Base-CharacterState"))
		ECharacterState character_state;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Replicated, meta = (DisplayName = "Hp", Category = "Base-CharacterState"))
		float hp;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (DisplayName = "Max Hp", Category = "Base-CharacterState"))
		float max_hp;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (DisplayName = "Base Power", Category = "Base-CharacterState"))
		float base_power;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (DisplayName = "Is On Sprint", Category = "Base-CharacterState"))
		bool is_on_sprint;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Base-CharacterState")
		float walk_speed;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Base-CharacterState")
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
	UPROPERTY(BlueprintReadWrite, Replicated, meta = (Category = "Base-Combat"))
		float knock_back_count;
	UPROPERTY(BlueprintReadWrite, meta = (Category = "Base-Combat"))
		float knock_back_count_end;
	UPROPERTY(BlueprintReadWrite, meta = (Category = "Base-Combat"))
		FVector knock_back_velocity;
	UPROPERTY(BlueprintReadWrite, meta = (Category = "Base-Combat"))
		FVector current_velocty;
	UPROPERTY(BlueprintReadWrite, meta = (Category = "Base-Combat"))
		TSet<AActor*> hit_actors_list;
	UPROPERTY(BlueprintReadWrite, meta = (Category = "Base-Combat"))
		FdamageData damage_data;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (Category = "Base-Combat"))
		UAnimMontage* normal_attack_montage;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (Category = "Base-Combat"))
		UAnimMontage* next_attack_montage;
	UPROPERTY(BlueprintReadWrite, Category = "Base-Combat")
		FVector prev_attack_sock_start_loc;
	UPROPERTY(BlueprintReadWrite, Category = "Base-Combat")
		FVector prev_attack_sock_end_loc;

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

	virtual void look_Implementation() override;
	virtual void applyDamage_Implementation(FdamageData __target_damage_data, AActor* __damage_causor) override;
	virtual void setDamageData_Implementation(FdamageData __target_damage_data) override;
	virtual void resetHitActorList_Implementation() override;
	virtual void attackEvent_Implementation(AActor* __hit_actor) override;
	virtual void resetNextAttack_Implementation() override;
	//virtual void getWeapon_Implementation(USkeletalMeshComponent*& __weapon) override;
	virtual void getDamageData_Implementation(FdamageData& __damage_data) override;
	virtual void getLookRotation_Implementation(/*out*/ FRotator& __look_rotation) override;
	virtual void getTargetRotation_Implementation(/*out*/ FRotator& __target_rotation) override;
	virtual void setPrevSockLoc_Implementation(FVector __start, FVector __end) override;
	virtual void getPrevSockLoc_Implementation(/*out*/ FVector& __start, /*out*/ FVector& __end) override;
	virtual void getAttackTraceChannel_Implementation(/*out*/ TEnumAsByte<ETraceTypeQuery>& __attack_trace_channel) override;
	virtual void rotateActorInterp_Implementation(FRotator __target_rotation, float __delta_time, float __speed) override;


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
		void airboneStart();
		virtual void airboneStart_Implementation();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, meta = (Category = "Base-Ragdoll"))
		void ragdollGetUp();
		virtual void ragdollGetUp_Implementation();
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, meta = (Category = "Base-Character State"))
		void setCharacterState(ECharacterState target_character_state);
		virtual void setCharacterState_Implementation(ECharacterState target_character_state);

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

	UFUNCTION(BlueprintCallable, NetMulticast, Reliable)
		void applyDamage_Multicast(FdamageData target_damage_data, AActor* damage_causor);
		virtual void applyDamage_Multicast_Implementation(FdamageData target_damage_data, AActor* damage_causor);

	UFUNCTION(BlueprintCallable, NetMulticast, Reliable)
		void animation_Sound_Multicast(UAnimMontage* anim, USoundBase* sound);
		virtual void animation_Sound_Multicast_Implementation(UAnimMontage* anim, USoundBase* sound);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, meta = (Category = "Base-Combat"))
		void selectHitAnimation(FVector velocity, /*out*/UAnimMontage*& hit_anim);
		virtual void selectHitAnimation_Implementation(FVector velocity, UAnimMontage*& hit_anim);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, meta = (Category = "Base-Combat"))
		void knock_BackProcess();
		virtual void knock_BackProcess_Implementation();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, meta = (Category = "Base-Combat"))
		void knock_Back(FVector velocity);
		virtual void knock_Back_Implementation(FVector velocity);

	UFUNCTION(BlueprintCallable, Server, UnReliable)
		void CtoS_setRotation(FRotator __target_rotation);
		virtual void CtoS_setRotation_Implementation(FRotator __target_rotation);
};
