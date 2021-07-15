// Fill out your copyright notice in the Description page of Project Settings.


#include "NS_Attack_2Sock_Trace.h"
#include "../Public/Interface_BaseCharacter.h"
#include "../Public/Interface_PlayerController.h"
#include "../CustomData.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "../Public/PWOGameInstance.h"

void UNS_Attack_2Sock_Trace::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) {

	if (MeshComp->GetWorld()->GetFirstPlayerController() == NULL)
		return;
	AActor* actor = MeshComp->GetOwner();
	FdamageData damage_data;
	Cast<UPWOGameInstance>(actor->GetGameInstance())->findDamageData(damage_id, damage_data);
	if (actor->GetClass()->ImplementsInterface(UInterface_BaseCharacter::StaticClass())) {
		IInterface_BaseCharacter::Execute_resetHitActorList(actor);
		IInterface_BaseCharacter::Execute_setDamageData(actor, damage_data);
		IInterface_BaseCharacter::Execute_setDamageID(actor, damage_id);
		trace_channel = IInterface_BaseCharacter::Execute_getAttackTraceChannel(actor);
	}
	prev_sock_start_loc = MeshComp->GetSocketLocation(socket_start_name);
	prev_sock_end_loc = MeshComp->GetSocketLocation(socket_end_name);
}

void UNS_Attack_2Sock_Trace::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime) {
	AActor* actor = MeshComp->GetOwner();
	if (MeshComp->GetWorld()->GetFirstPlayerController() == NULL)
		return;
	if (actor->GetClass()->ImplementsInterface(UInterface_BaseCharacter::StaticClass())) {
		FVector cur_sock_start_loc;
		FVector cur_sock_end_loc;
		FVector prev_mid;
		FVector cur_mid;
		FRotator trace_rotation;
		TArray<FHitResult> hit_results;
		FVector halfsize;
		const TArray<AActor*> ignore_actors;
		cur_sock_start_loc = MeshComp->GetSocketLocation(socket_start_name);
		cur_sock_end_loc = MeshComp->GetSocketLocation(socket_end_name);
		prev_mid = (prev_sock_start_loc + prev_sock_end_loc) * 0.5f;
		cur_mid = (cur_sock_start_loc + cur_sock_end_loc) * 0.5f;
		if (cur_mid == prev_mid)
			return;
		else
			trace_rotation = UKismetMathLibrary::FindLookAtRotation(prev_mid, cur_mid);
		halfsize = UKismetMathLibrary::Quat_UnrotateVector(trace_rotation.Quaternion(), cur_sock_end_loc - cur_sock_start_loc) / 2;
		halfsize.X = abs(halfsize.X);
		halfsize.Y = abs(halfsize.Y);
		halfsize.Z = abs(halfsize.Z);
		halfsize += volume;
		UKismetSystemLibrary::BoxTraceMulti(MeshComp, prev_mid, cur_mid, halfsize * actor->GetActorScale().X, trace_rotation, trace_channel, false, ignore_actors, EDrawDebugTrace::Type::None, hit_results, true);
		for (auto i : hit_results) {
			if (i.GetActor()->GetClass()->ImplementsInterface(UInterface_BaseCharacter::StaticClass())) {
				IInterface_BaseCharacter::Execute_attackEvent(actor, i.GetActor(), i);
			}
		}
		prev_sock_start_loc = cur_sock_start_loc;
		prev_sock_end_loc = cur_sock_end_loc;
	}
}