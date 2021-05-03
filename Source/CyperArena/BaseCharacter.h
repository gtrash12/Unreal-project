// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Public/Interface_BaseCharacter.h"

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
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (DisplayName = "Look Yaw", Category = "Base-Look", MultiLine = "true", OverrideNativeName = "look_yaw"))
	float look_yaw;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (DisplayName = "Look Pitch", Category = "Base-Look", MultiLine = "true", OverrideNativeName = "look_pitch"))
	float look_pitch;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Replicated, meta = (DisplayName = "R Look Rotation", Category = "Base-Look", MultiLine = "true", OverrideNativeName = "R_look_rotation"))
	FRotator R_look_rotation;
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
};
