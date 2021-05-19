// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "NS_Attack_Weapon_2Sock_Trace.generated.h"

/**
 * 
 */
UCLASS()
class CYPERARENA_API UNS_Attack_Weapon_2Sock_Trace : public UAnimNotifyState
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FName damage_id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FName weapon_key;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FName socket_start;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FName socket_end;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float half_width;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float half_height;
		UPrimitiveComponent* weapon;
		TEnumAsByte<ETraceTypeQuery> trace_channel;

	//UFUNCTION(BlueprintNativeEvent)
		virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) override;
	//UFUNCTION(BlueprintNativeEvent)
		virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime) override;
	//UFUNCTION(BlueprintImplementableEvent)
		//bool Received_NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) const;
};
