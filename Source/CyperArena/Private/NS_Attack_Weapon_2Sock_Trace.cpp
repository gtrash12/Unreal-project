// Fill out your copyright notice in the Description page of Project Settings.


#include "NS_Attack_Weapon_2Sock_Trace.h"
#include "../Public/Interface_BaseCharacter.h"
#include "../Public/Interface_PlayerController.h"
#include "../CustomData.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

void UNS_Attack_Weapon_2Sock_Trace::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) {
	
	if (MeshComp->GetWorld()->GetFirstPlayerController() == NULL)
		return;
	AActor* actor = MeshComp->GetOwner();
	FdamageData damage_data;
	if (MeshComp->GetWorld()->GetFirstPlayerController()->GetClass()->ImplementsInterface(UInterface_PlayerController::StaticClass()))
	{
		IInterface_PlayerController::Execute_findDamageData(MeshComp->GetWorld()->GetFirstPlayerController(), damage_id, damage_data);
	}
	if (actor->GetClass()->ImplementsInterface(UInterface_BaseCharacter::StaticClass())) {
		IInterface_BaseCharacter::Execute_getWeapon(actor, weapon_key, weapon);
		IInterface_BaseCharacter::Execute_resetHitActorList(actor);
		IInterface_BaseCharacter::Execute_setPrevSockLoc(actor, weapon->GetSocketLocation(socket_start), weapon->GetSocketLocation(socket_end));
		IInterface_BaseCharacter::Execute_setDamageData(actor, damage_data);
		IInterface_BaseCharacter::Execute_setDamageID(actor, damage_id);
		IInterface_BaseCharacter::Execute_getAttackTraceChannel(actor, trace_channel);
	}
}

void UNS_Attack_Weapon_2Sock_Trace::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime) {
	AActor* actor = MeshComp->GetOwner();
	if(MeshComp->GetWorld()->GetFirstPlayerController() == NULL)
		return;
	if (actor->GetClass()->ImplementsInterface(UInterface_BaseCharacter::StaticClass())) {
		FdamageData damage_data;
		FVector prev_sock_start;
		FVector prev_sock_end;
		FVector cur_sock_start;
		FVector cur_sock_end;
		FVector prev_mid;
		FVector cur_mid;
		FRotator trace_rotation;
		FVector halfsize;
		TArray<FHitResult> hit_results;
		const TArray<AActor*> ignore_actors;
		IInterface_BaseCharacter::Execute_getPrevSockLoc(actor, prev_sock_start, prev_sock_end);
		prev_mid = (prev_sock_start + prev_sock_end) * 0.5f;
		cur_sock_start = weapon->GetSocketLocation(socket_start);
		cur_sock_end = weapon->GetSocketLocation(socket_end);
		cur_mid = (cur_sock_start + cur_sock_end) * 0.5f;
		
		if (cur_mid == prev_mid)
			return;
		else
			trace_rotation = UKismetMathLibrary::FindLookAtRotation(prev_mid, cur_mid);
		halfsize = UKismetMathLibrary::Quat_UnrotateVector(trace_rotation.Quaternion(), prev_sock_start - prev_sock_end) / 2;
		halfsize.Y += half_width;
		halfsize.Z += half_height;
		UKismetSystemLibrary::BoxTraceMulti(MeshComp, prev_mid, cur_mid, halfsize, trace_rotation, trace_channel, false, ignore_actors, EDrawDebugTrace::Type::None, hit_results, true);
		IInterface_BaseCharacter::Execute_setPrevSockLoc(actor, cur_sock_start, cur_sock_end);
		for (auto i : hit_results) {
			if (i.GetActor()->GetClass()->ImplementsInterface(UInterface_BaseCharacter::StaticClass())) {
				IInterface_BaseCharacter::Execute_attackEvent(actor, i.GetActor());
			}
		}
		IInterface_BaseCharacter::Execute_setPrevSockLoc(actor, MeshComp->GetSocketLocation(socket_start), MeshComp->GetSocketLocation(socket_end));
	}
}