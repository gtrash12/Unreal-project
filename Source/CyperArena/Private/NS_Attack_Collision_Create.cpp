// Fill out your copyright notice in the Description page of Project Settings.


#include "NS_Attack_Collision_Create.h"
#include "../Public/Interface_BaseCharacter.h"
#include "../Public/Interface_PlayerController.h"
#include "../CustomData.h"
#include "../BaseCharacter.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/SphereComponent.h"

void UNS_Attack_Collision_Create::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) {
	if (MeshComp->GetWorld()->GetFirstPlayerController() == NULL)
		return;
	AActor* actor = MeshComp->GetOwner();
	FdamageData damage_data;
	ABaseCharacter* base_character = Cast<ABaseCharacter>(actor);
	if (base_character == nullptr)
		return;
	if (MeshComp->GetWorld()->GetFirstPlayerController()->GetClass()->ImplementsInterface(UInterface_PlayerController::StaticClass()))
	{
		IInterface_PlayerController::Execute_findDamageData(MeshComp->GetWorld()->GetFirstPlayerController(), damage_id, damage_data);
	}
	if (actor->GetClass()->ImplementsInterface(UInterface_BaseCharacter::StaticClass())) {
		IInterface_BaseCharacter::Execute_resetHitActorList(actor);
		IInterface_BaseCharacter::Execute_setDamageData(actor, damage_data);
		USphereComponent* collision1 = NewObject<USphereComponent>(actor, USphereComponent::StaticClass(), FName("attack_collision"));
		UKismetSystemLibrary::PrintString(actor, "ASDF");
		if (collision1 != nullptr) {
			collision1->RegisterComponent();
			collision1->AttachTo(MeshComp,socket,EAttachLocation::Type::KeepRelativeOffset);
			collision1->SetWorldScale3D(scale);
			collision1->SetRelativeLocation(FVector::ZeroVector);
			collision1->SetCollisionProfileName(FName(TEXT("EnemyHitCollision")));
			collision1->SetGenerateOverlapEvents(true);
			UKismetSystemLibrary::PrintString(actor, "징");
		}
		collision1->OnComponentBeginOverlap.AddDynamic(base_character, &ABaseCharacter::onWeaponBeginOverlap);
		collision1->SetCollisionEnabled(ECollisionEnabled::Type::QueryAndPhysics);
	}
}

void UNS_Attack_Collision_Create::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) {
	if (MeshComp->GetWorld()->GetFirstPlayerController() == NULL)
		return;
	AActor* actor = MeshComp->GetOwner();
	ABaseCharacter* base_character = Cast<ABaseCharacter>(actor);
	if (base_character == nullptr)
		return;
	//UPrimitiveComponent* weapon;
	if (actor->GetClass()->ImplementsInterface(UInterface_BaseCharacter::StaticClass())) {
		//IInterface_BaseCharacter::Execute_getWeapon(actor, weapon);
	}
}