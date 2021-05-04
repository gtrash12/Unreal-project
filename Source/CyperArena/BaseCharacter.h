// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Public/Interface_BaseCharacter.h"
#include "CustomData.h"

#include "BaseCharacter.generated.h"

UCLASS()
class CYPERARENA_API ABaseCharacter : public ACharacter, public IInterface_BaseCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseCharacter();

	UPROPERTY(BlueprintReadOnly, NonTransactional, meta = (Category = "Base-Ragdoll", OverrideNativeName = "PhysicsHandle"))
		UPhysicsHandleComponent* ragdoll_physics_handle;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (DisplayName = "Look Yaw", Category = "Base-Look"))
		float look_yaw;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (DisplayName = "Look Pitch", Category = "Base-Look"))
		float look_pitch;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Replicated, meta = (DisplayName = "R Look Rotation", Category = "Base-Look"))
		FRotator R_look_rotation;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (DisplayName = "Character State", Category = "Base-CharacterState"))
		ECharacterState character_state;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (DisplayName = "Is On Sprint", Category = "Base-CharacterState"))
		bool is_on_sprint;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (DisplayName = "Is Simulation Owner", Category = "Base-Ragdoll"))
		bool is_simulation_owner;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (DisplayName = "Is Ragdoll on the Ground", Category = "Base-Ragdoll"))
		bool is_ragdoll_on_the_ground;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (DisplayName = "Is Ragdoll Face Up", Category = "Base-Ragdoll"))
		bool is_ragdoll_face_up;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Replicated, meta = (DisplayName = "Ragdoll Server Location", Category = "Base-Ragdoll"))
		FVector ragdoll_server_location;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (DisplayName = "Last Ragdoll Server Location", Category = "Base-Ragdoll"))
		FVector last_ragdoll_server_location;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (DisplayName = "Prev Ragdoll Server Location", Category = "Base-Ragdoll"))
		FVector prev_ragdoll_server_location;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (DisplayName = "Replication Delay Count", Category = "Base-Ragdoll"))
		float replication_delay_count;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (DisplayName = "Last Replication Delay", Category = "Base-Ragdoll"))
		float last_replication_delay;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (DisplayName = "Main Anim Instance", Category = "Base-Ragdoll"))
		UAnimInstance* main_anim_instance;
	UPROPERTY(BlueprintReadOnly)
		UAnimMontage* airbone_b_anim;
	UPROPERTY(BlueprintReadOnly)
		UAnimMontage* get_up_f_anim;
	UPROPERTY(BlueprintReadOnly)
		UAnimMontage* get_up_b_anim;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (DisplayName = "IS On Action", Category = "Base-Combat"))
		bool is_on_action;


private :
	UPROPERTY()
		float d_time;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//UFUNCTION(BlueprintCallable, BlueprintNativeEvent, meta = (Category = "Base", CppFromBpEvent, OverrideNativeName = "Look"))
	void look();
	virtual void look_Implementation() override;
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

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, meta = (Category = "Base-Character State"))
		void findClosestPlayer(/*out*/ AActor*& closest_player);
		virtual void findClosestPlayer_Implementation(AActor*& closest_player);
};
