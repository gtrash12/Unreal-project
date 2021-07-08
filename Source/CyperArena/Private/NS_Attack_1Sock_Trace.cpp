// Fill out your copyright notice in the Description page of Project Settings.


#include "NS_Attack_1Sock_Trace.h"
#include "../Public/Interface_BaseCharacter.h"
#include "../Public/Interface_PlayerController.h"
#include "../CustomData.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "../Public/PWOGameInstance.h"

/// <summary>
/// 노티파이 시작시 damage_id 로 부터 damage_data를 얻어서 저장하고
/// 시전자의 hit_actor_list를 초기화
/// 시전자의 attack_trace_channel을 얻어서 트레이스할 대상을 설정
/// 메쉬의 VisibilityBasedAnimTickOption 을 AlwaysTickPoseAndRefreshBones 으로 전환해서 실행 캐릭터가 플레이어의 화면에 안보여도 본 트랜스폼을 매 틱 갱신해서 충돌 판정이 정확하게 일어나도록 설정
/// </summary>
/// <param name="MeshComp"></param>
/// <param name="Animation"></param>
/// <param name="TotalDuration"></param>
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
}

/// <summary>
/// notify 실행 기간 동안 매 틱 BoxTraceMulti 를 실행해서 탐지된 모든 액터에 대해 attackEvent를 실행시킴 
/// </summary>
/// <param name="MeshComp"></param>
/// <param name="Animation"></param>
/// <param name="FrameDeltaTime"></param>
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
/// <summary>
/// 노티파이 종료시 실행 메쉬의 VisibilityBasedAnimTickOption 을 OnlyTickMontagesWhenNotRendered 으로 전환해서 비렌더링시 본 트랜스폼을 갱신하지 않고 Tick 이벤트만 수행하도록 변경
/// </summary>
/// <param name="MeshComp"></param>
/// <param name="Animation"></param>
void UNS_Attack_1Sock_Trace::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) {
	if (MeshComp->GetWorld()->GetFirstPlayerController() == NULL)
		return;

	MeshComp->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::OnlyTickMontagesWhenNotRendered;
}