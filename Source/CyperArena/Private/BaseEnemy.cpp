// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseEnemy.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Interface_EnemyController.h"
#include "Interface_PlayerController.h"
#include "Kismet/KismetMathLibrary.h"

ABaseEnemy::ABaseEnemy()
{
	state_bar = CreateDefaultSubobject<UWidgetComponent>(TEXT("StateBar"));
	state_bar->CreationMethod = EComponentCreationMethod::Native;
	state_bar->AttachTo(RootComponent);
}

void ABaseEnemy::BeginPlay()
{
	Super::BeginPlay();
	GetCharacterMovement()->MaxWalkSpeed = walk_speed;
}


void ABaseEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	/* Set monster rotation to move direction when moving. when knock-back moving don't rotate monster */
	if (knock_back_speed > 0) {
		GetCharacterMovement()->bOrientRotationToMovement = false;
	}
	else {
		GetCharacterMovement()->bOrientRotationToMovement = true;
	}

	/* Get up Process wher monster down */
	if (character_state == ECharacterState::Ragdoll) {
		if (is_ragdoll_on_the_ground) {
			down_time -= DeltaTime;
			if (down_time <= 0) {
				ragdollGetUp();
				setCharacterState(ECharacterState::Walk_and_Jump);
				is_on_sprint = true;
			}
		}
		else {
			down_time = 2;
		}
	} else if (character_state == ECharacterState::Death) {
		down_time -= DeltaTime;
		if (down_time <= 0) {
			Destroy();
		}
	}
}

void ABaseEnemy::setLookLocation_Implementation(FVector __target_look_location)
{
	look_location = __target_look_location;
}

void ABaseEnemy::setTarget_Implementation(AActor* __target)
{
	target_actor = __target;
}

AActor* ABaseEnemy::getTarget_Implementation()
{
	return target_actor;
}

void ABaseEnemy::setLockOnMarker_Implementation(bool __is_lock_on)
{

}

void ABaseEnemy::setStateBarVisibility_Implementation(bool __visibility)
{
	if (state_bar->IsValidLowLevel()) {
		state_bar->SetVisibility(__visibility);
	}
}

void ABaseEnemy::setSprint_Implementation(bool __is_sprint)
{
	Multicast_setSprint(__is_sprint);
}

void ABaseEnemy::attack_Implementation()
{
	if (main_anim_instance->IsAnyMontagePlaying())
		return;
	if (character_state == ECharacterState::Walk_and_Jump) {
		FVector last_vector;
		last_vector = UKismetMathLibrary::GetForwardVector(UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), target_actor->GetActorLocation()));
		last_vector.Z = 0;
		Multicast_attack(last_vector, selectAttackMontage());
	}
}

void ABaseEnemy::applyDamage_Multicast_Exec_Implementation(FName __target_damage_id, AActor* damage_causer, FName __hit_bone_name) 
{
	Super::applyDamage_Multicast_Exec_Implementation(__target_damage_id, damage_causer, __hit_bone_name);
	target_actor = damage_causer;
	if (GetController()->GetClass()->ImplementsInterface(UInterface_EnemyController::StaticClass())) {
		IInterface_EnemyController::Execute_changeTarget(GetController(), damage_causer);
	}
	if (state_bar->IsValidLowLevel()) {
		if (hp > 0) {
			if (state_bar->IsVisible() == false)
				setStateBarVisibility(true);
		}
		else {
			death();
		}
	}
}

void ABaseEnemy::getTargetRotation_Implementation(FRotator& __target_rotation)
{
	FRotator res = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), look_location);
	res.Roll = 0;
	res.Pitch = 0;
	__target_rotation = res;
}

void ABaseEnemy::getAttackTraceChannel_Implementation(TEnumAsByte<ETraceTypeQuery>& __attack_trace_channel)
{
	__attack_trace_channel = ETraceTypeQuery::TraceTypeQuery5;
}


void ABaseEnemy::Multicast_setSprint_Implementation(bool __is_sprint)
{
	is_on_sprint = __is_sprint;
	if (__is_sprint)
		GetCharacterMovement()->MaxWalkSpeed = sprint_speed;
	else
		GetCharacterMovement()->MaxWalkSpeed = walk_speed;;
}

void ABaseEnemy::Multicast_attack_Implementation(FVector __last_movement_input, UAnimMontage* __animation)
{
	PlayAnimMontage(__animation);
	rotateActorWithInTime(UKismetMathLibrary::MakeRotFromX(__last_movement_input), 0.4f);
}

UAnimMontage* ABaseEnemy::selectAttackMontage_Implementation()
{
	if (attack_montages.Num() == 0)
		return nullptr;
	return attack_montages[UKismetMathLibrary::RandomIntegerInRange(0, attack_montages.Num()-1)];
}

void ABaseEnemy::death_Implementation() {
	if (knock_back_speed == 0)
		PlayAnimMontage(hit_b_anim);
	setStateBarVisibility(false);
	APlayerController* playercontroller = GetWorld()->GetFirstPlayerController();
	if (playercontroller->GetClass()->ImplementsInterface(UInterface_PlayerController::StaticClass())) {
		bool __is_target;
		IInterface_PlayerController::Execute_isLock_On_Target(playercontroller, this, __is_target);
		if(__is_target)
			IInterface_PlayerController::Execute_releaseLock_ON(playercontroller);
	}
	if (state_bar->IsValidLowLevel()) {
		state_bar->ConditionalBeginDestroy();
	}

	FTimerHandle timer_handle;
	GetWorld()->GetTimerManager().SetTimer(timer_handle, FTimerDelegate::CreateLambda([&]() {
		setCharacterState(ECharacterState::Death);
		GetMesh()->AddImpulseToAllBodiesBelow(GetVelocity() * FVector(10, 10, 3) + FVector(0, 0, 100), "pelvis");
		}), 0.25f, false);
}

void ABaseEnemy::setCharacterState_Implementation(ECharacterState target_character_state)
{
	Super::setCharacterState_Implementation(target_character_state);
	down_time = 3;
}

UBehaviorTree* ABaseEnemy::getBehaviorTree_Implementation()
{
	return behavior_tree;
}

void ABaseEnemy::setLookRotation_Implementation()
{
	if (target_actor != nullptr && target_actor->IsValidLowLevel()) {
		look_location = target_actor->GetActorLocation();
	}
	FRotator target_look_rotation = UKismetMathLibrary::NormalizedDeltaRotator(UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), look_location), GetActorRotation());
	FRotator interped_look_rotation = UKismetMathLibrary::RInterpTo(FRotator(look_pitch, look_yaw, 0), target_look_rotation, d_time, 3);
	look_pitch = UKismetMathLibrary::ClampAngle(interped_look_rotation.Pitch, -90, 90);
	look_yaw = UKismetMathLibrary::ClampAngle(interped_look_rotation.Yaw, -90, 90);
}