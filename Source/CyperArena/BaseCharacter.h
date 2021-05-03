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
		CharacterState character_state;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (DisplayName = "Is Simulation Owner", Category = "Base-Ragdoll"))
		bool is_simulation_owner;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (DisplayName = "Is Ragdoll on the Ground", Category = "Base-Ragdoll"))
		bool is_ragdoll_on_the_ground;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (DisplayName = "Is Ragdoll Face Up", Category = "Base-Ragdoll"))
		bool is_ragdoll_face_up;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (DisplayName = "Ragdoll Server Location", Category = "Base-Ragdoll"))
		FVector ragdoll_server_location;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (DisplayName = "Last Ragdoll Server Location", Category = "Base-Ragdoll"))
		FVector last_ragdoll_server_location;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (DisplayName = "Prev Ragdoll Server Location", Category = "Base-Ragdoll"))
		FVector prev_ragdoll_server_location;


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
	//UFUNCTION(BlueprintCallable, BlueprintNativeEvent, meta = (Category = "Base-Interface", OverrideNativeName = "look"))
	void look();
	virtual void look_Implementation() override;
	/*UFUNCTION(BlueprintCallable, meta = (Category = "Base", CppFromBpEvent, OverrideNativeName = "rotateActorTimeline"))
	virtual void rotateActorTimeline(FRotator bpp__target_rotation__pf, float bpp__time__pf);*/

	UFUNCTION(BlueprintCallable, meta = (Category = "Base-Ragdoll", OverrideNativeName = "Stick to the Ground"))
	virtual void stickToTheGround(FVector location);
	//UFUNCTION(Server, UnReliable, BlueprintCallable, meta = (Category = "Base-Ragdoll", OverrideNativeName = "CtoS - Target Location"))
	UFUNCTION(Server, UnReliable)
	void CtoS_targetLocation(ABaseCharacter* target_actor, FVector target_location);
	virtual void CtoS_targetLocation_Implementation(ABaseCharacter* target_actor, FVector target_location);

private:
	virtual void ragdoll_ClientOnwer();
};
