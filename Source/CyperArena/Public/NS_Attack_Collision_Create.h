// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "NS_Attack_Collision_Create.generated.h"

/**
 * 
 */
UCLASS()
class CYPERARENA_API UNS_Attack_Collision_Create : public UAnimNotifyState
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FName damage_id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FName socket;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector offset;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector scale;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UPrimitiveComponent* collision;

	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
};
