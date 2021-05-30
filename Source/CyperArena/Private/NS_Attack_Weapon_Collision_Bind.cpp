// Fill out your copyright notice in the Description page of Project Settings.


#include "NS_Attack_Weapon_Collision_Bind.h"
#include "../Public/Interface_BaseCharacter.h"
#include "../Public/Interface_PlayerController.h"
#include "../BaseCharacter.h"
#include "../CustomData.h"
#include "Kismet/KismetSystemLibrary.h"

void UNS_Attack_Weapon_Collision_Bind::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) {
	if (MeshComp->GetWorld()->GetFirstPlayerController() == NULL)
		return;
	AActor* actor = MeshComp->GetOwner();
	ABaseCharacter* base_character = Cast<ABaseCharacter>(actor);
	if (base_character == nullptr)
		return;
	FdamageData damage_data;
	UPrimitiveComponent* weapon;
	if (MeshComp->GetWorld()->GetFirstPlayerController()->GetClass()->ImplementsInterface(UInterface_PlayerController::StaticClass()))
	{
		IInterface_PlayerController::Execute_findDamageData(MeshComp->GetWorld()->GetFirstPlayerController(), damage_id, damage_data);
	}
	if (actor->GetClass()->ImplementsInterface(UInterface_BaseCharacter::StaticClass())) {
		IInterface_BaseCharacter::Execute_getWeapon(actor, weapon_key, weapon);
		IInterface_BaseCharacter::Execute_resetHitActorList(actor);
		IInterface_BaseCharacter::Execute_setDamageData(actor, damage_data);
		IInterface_BaseCharacter::Execute_setDamageID(actor, damage_id);
		weapon->OnComponentBeginOverlap.AddDynamic(base_character, &ABaseCharacter::onWeaponBeginOverlap);
		weapon->SetCollisionEnabled(ECollisionEnabled::Type::QueryAndPhysics);
	}
}

void UNS_Attack_Weapon_Collision_Bind::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) {
	if (MeshComp->GetWorld()->GetFirstPlayerController() == NULL)
		return;
	AActor* actor = MeshComp->GetOwner();
	ABaseCharacter* base_character = Cast<ABaseCharacter>(actor);
	if (base_character == nullptr)
		return;
	UPrimitiveComponent* weapon;
	if (actor->GetClass()->ImplementsInterface(UInterface_BaseCharacter::StaticClass())) {
		IInterface_BaseCharacter::Execute_getWeapon(actor, weapon_key, weapon);
		weapon->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
		weapon->OnComponentBeginOverlap.RemoveDynamic(base_character, &ABaseCharacter::onWeaponBeginOverlap);
	}
}