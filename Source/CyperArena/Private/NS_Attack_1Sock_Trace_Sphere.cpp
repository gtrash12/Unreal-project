// Fill out your copyright notice in the Description page of Project Settings.


#include "NS_Attack_1Sock_Trace_Sphere.h"
#include "../Public/Interface_BaseCharacter.h"
#include "../Public/Interface_PlayerController.h"
#include "../CustomData.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "../Public/PWOGameInstance.h"

void UNS_Attack_1Sock_Trace_Sphere::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) {

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
		trace_channel = IInterface_BaseCharacter::Execute_getAttackTraceChannel(actor);
	}
}

void UNS_Attack_1Sock_Trace_Sphere::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime) {
	AActor* actor = MeshComp->GetOwner();
	if (MeshComp->GetWorld()->GetFirstPlayerController() == NULL)
		return;
	if (actor->GetClass()->ImplementsInterface(UInterface_BaseCharacter::StaticClass())) {
		FVector cur_sock_loc = MeshComp->GetSocketLocation(socket_name);
		TArray<FHitResult> hit_results;
		TArray<AActor*> ignore_actors;
		TSet<AActor*> ign;
		UKismetSystemLibrary::SphereTraceMulti(MeshComp, cur_sock_loc, cur_sock_loc, radius * actor->GetActorScale().X, trace_channel, false, ignore_actors, EDrawDebugTrace::Type::None, hit_results, true);
		for (auto i : hit_results) {
			if (i.GetActor()->GetClass()->ImplementsInterface(UInterface_BaseCharacter::StaticClass())) {
				if(ign.Contains(i.GetActor()) == false)
					IInterface_BaseCharacter::Execute_attackEvent(actor, i.GetActor(), i);
				ign.Add(i.GetActor());
			}
		}
	}
}

void UNS_Attack_1Sock_Trace_Sphere::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) {
	if (MeshComp->GetWorld()->GetFirstPlayerController() == NULL)
		return;
	MeshComp->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::OnlyTickMontagesWhenNotRendered;
}