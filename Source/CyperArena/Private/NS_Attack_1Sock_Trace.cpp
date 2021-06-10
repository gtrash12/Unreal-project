// Fill out your copyright notice in the Description page of Project Settings.


#include "NS_Attack_1Sock_Trace.h"
#include "../Public/Interface_BaseCharacter.h"
#include "../Public/Interface_PlayerController.h"
#include "../CustomData.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "../Public/PWOGameInstance.h"

void UNS_Attack_1Sock_Trace::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) {

	if (MeshComp->GetWorld()->GetFirstPlayerController() == NULL)
		return;
	AActor* actor = MeshComp->GetOwner();
	FdamageData damage_data;
	Cast<UPWOGameInstance>(actor->GetGameInstance())->findDamageData(damage_id, damage_data);
	if (actor->GetClass()->ImplementsInterface(UInterface_BaseCharacter::StaticClass())) {
		MeshComp->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::AlwaysTickPoseAndRefreshBones;
		IInterface_BaseCharacter::Execute_resetHitActorList(actor);
		IInterface_BaseCharacter::Execute_setDamageData(actor, damage_data);
		IInterface_BaseCharacter::Execute_setDamageID(actor, damage_id);
		IInterface_BaseCharacter::Execute_getAttackTraceChannel(actor, trace_channel);
	}
	//prev_sock_loc = MeshComp->GetSocketLocation(socket_name);
}

void UNS_Attack_1Sock_Trace::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime) {
	AActor* actor = MeshComp->GetOwner();
	if (MeshComp->GetWorld()->GetFirstPlayerController() == NULL)
		return;
	if (actor->GetClass()->ImplementsInterface(UInterface_BaseCharacter::StaticClass())) {
		FVector cur_sock_loc;
		FRotator trace_rotation;
		TArray<FHitResult> hit_results;
		const TArray<AActor*> ignore_actors;
		cur_sock_loc = MeshComp->GetSocketLocation(socket_name);

		if (cur_sock_loc == prev_sock_loc)
			return;
		else
			trace_rotation = UKismetMathLibrary::FindLookAtRotation(prev_sock_loc, cur_sock_loc);
		//UKismetSystemLibrary::BoxTraceMulti(MeshComp, prev_sock_loc, cur_sock_loc, volume, trace_rotation, trace_channel, false, ignore_actors, EDrawDebugTrace::Type::ForOneFrame, hit_results, true);
		UKismetSystemLibrary::BoxTraceMulti(MeshComp, cur_sock_loc, cur_sock_loc, volume * actor->GetActorScale().X, FRotator::ZeroRotator, trace_channel, false, ignore_actors, EDrawDebugTrace::Type::None, hit_results, true);
		for (auto i : hit_results) {
			if (i.GetActor()->GetClass()->ImplementsInterface(UInterface_BaseCharacter::StaticClass())) {
				IInterface_BaseCharacter::Execute_attackEvent(actor, i.GetActor(), i.BoneName);
			}
		}
		prev_sock_loc = cur_sock_loc;
	}
}

void UNS_Attack_1Sock_Trace::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) {
	if (MeshComp->GetWorld()->GetFirstPlayerController() == NULL)
		return;
	MeshComp->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::OnlyTickPoseWhenRendered;
}