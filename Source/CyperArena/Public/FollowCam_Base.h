// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "FollowCam_Base.generated.h"

UCLASS()
class CYPERARENA_API AFollowCam_Base : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFollowCam_Base();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		USpringArmComponent* spring_arm;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		UCameraComponent* camera;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		AActor* follow_target;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		AActor* look_target;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		FVector location_offset;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		FVector original_offset;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		bool is_lock_on;
};
