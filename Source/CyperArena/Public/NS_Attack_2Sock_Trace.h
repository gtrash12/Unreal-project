// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "NS_Attack_2Sock_Trace.generated.h"

/**
 * 
 */
UCLASS()
class CYPERARENA_API UNS_Attack_2Sock_Trace : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FName damage_id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FName socket_start_name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FName socket_end_name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector volume;
	FVector prev_sock_start_loc;
	FVector prev_sock_end_loc;
	TEnumAsByte<ETraceTypeQuery> trace_channel;

	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) override;
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime) override;
};
