// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacter.h"

#include "Kismet/KismetSystemLibrary.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Net/UnrealNetwork.h"
#include "Engine.h"
#include "EngineUtils.h"
#include "Public/Interface_PlayerController.h"
#include "Public/PWOGameInstance.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"
#include "NiagaraComponent.h"
#include "../public/CameraShake_Hit.h"
#include "Camera/CameraShake.h"

// Sets default values
ABaseCharacter::ABaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//리플리케이션 설정
	bReplicates = true;

	ragdoll_physics_handle = CreateDefaultSubobject<UPhysicsHandleComponent>(TEXT("PhysicsHandle"));
	ragdoll_physics_handle->CreationMethod = EComponentCreationMethod::Native;
	ragdoll_physics_handle->InterpolationSpeed = 10;
	//ragdoll_physics_handle->bAutoActivate = false;

	character_state = ECharacterState::Walk_and_Jump;

	// 애니메이션 설정
	static ConstructorHelpers::FObjectFinder<UAnimMontage> Airbone_B_Montage(TEXT("AnimMontage'/Game/Animation/Airbone_b_Montage.Airbone_b_Montage'"));
	if (Airbone_B_Montage.Succeeded())
		airbone_b_anim = Airbone_B_Montage.Object;
	static ConstructorHelpers::FObjectFinder<UAnimMontage> ALS_CLF_GetUp_Back_Montage_Default(TEXT("AnimMontage'/Game/Animation/Actions/ALS_CLF_GetUp_Back_Montage_Default.ALS_CLF_GetUp_Back_Montage_Default'"));
	if (ALS_CLF_GetUp_Back_Montage_Default.Succeeded())
		get_up_b_anim = ALS_CLF_GetUp_Back_Montage_Default.Object;
	static ConstructorHelpers::FObjectFinder<UAnimMontage> ALS_CLF_GetUp_Front_Montage_Default(TEXT("AnimMontage'/Game/Animation/Actions/ALS_CLF_GetUp_Front_Montage_Default.ALS_CLF_GetUp_Front_Montage_Default'"));
	if (ALS_CLF_GetUp_Front_Montage_Default.Succeeded())
		get_up_f_anim = ALS_CLF_GetUp_Front_Montage_Default.Object;
	static ConstructorHelpers::FObjectFinder<UAnimMontage> Hit_B_Montage(TEXT("AnimMontage'/Game/Animation/Combat/Hit_B_Montage.Hit_B_Montage'"));
	if (Hit_B_Montage.Succeeded())
		hit_b_anim = Hit_B_Montage.Object;
	static ConstructorHelpers::FObjectFinder<UAnimMontage> Hit_F_Montage(TEXT("AnimMontage'/Game/Animation/Combat/Hit_F_Montage.Hit_F_Montage'"));
	if (Hit_F_Montage.Succeeded())
		hit_f_anim = Hit_F_Montage.Object;
	static ConstructorHelpers::FObjectFinder<UAnimMontage> Hit_R_Montage(TEXT("AnimMontage'/Game/Animation/Combat/Hit_R_Montage.Hit_R_Montage'"));
	if (Hit_R_Montage.Succeeded())
		hit_r_anim = Hit_R_Montage.Object;
	static ConstructorHelpers::FObjectFinder<UAnimMontage> Hit_L_Montage(TEXT("AnimMontage'/Game/Animation/Combat/Hit_L_Montage.Hit_L_Montage'"));
	if (Hit_L_Montage.Succeeded())
		hit_l_anim = Hit_L_Montage.Object;
	static ConstructorHelpers::FObjectFinder<USoundBase> SQ_Hit(TEXT("SoundCue'/Game/SFX/SQ_Hit.SQ_Hit'"));
	if (SQ_Hit.Succeeded())
		sq_hit = SQ_Hit.Object;
}

// 컴포넌트 초기화 후 함수 바인딩 등
void ABaseCharacter::PostInitializeComponents() {

	Super::PostInitializeComponents();

	// 캡슐 컴포넌트의 히트 델리게이트에 함수 등록
	//GetCapsuleComponent()->OnComponentHit.AddDynamic(this, &ABaseCharacter::onCapsuleComponentHit);

	//Make sure the mesh and animbp update after the CharacterBP
	GetMesh()->AddTickPrerequisiteActor(this);
	main_anim_instance = GetMesh()->GetAnimInstance();
	GetCharacterMovement()->bOrientRotationToMovement = 0;

	
}

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	max_hp = hp;
	stamina = max_stamina;
}

// Called every frame
void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	d_time = DeltaTime;

	//바라보기에 필요한 변수 갱신
	setLookRotation();
	
	 //에어본시 피지컬 애니메이션의 관절 각 운동량 조절
	// 중력 트레이스와 예측 트레이스를 투 개를 이용해 매 틱 충돌판정
	if (character_state == ECharacterState::Airbone) {
		//UMeshComponent* mesh = GetMesh();
		//mesh->SetAllPhysicsAngularVelocityInRadians(GetVelocity());
		FVector hit_normal;
		FVector hit_location;
		if (airbone_HitChk(GetVelocity(), hit_normal, hit_location)) {
			/* 충돌이 감지되었으면 이펙트 실행 */
			if (UKismetSystemLibrary::IsDedicatedServer(this) == false) {
				/* 카메라 쉐이크 */
				if (GetController() && GetController()->IsValidLowLevel() && GetController()->IsA<APlayerController>()) {
					Cast<APlayerController>(GetController())->ClientPlayCameraShake(UCameraShake_Hit::StaticClass(), GetVelocity().Size() / 1000);
				}
				/* 먼지 이펙트 & 사운드 */
				UPWOGameInstance* gameinstance = Cast<UPWOGameInstance>(GetGameInstance());
				FRotator effect_rotator = hit_normal.ToOrientationRotator();
				effect_rotator.Pitch -= 90;
				UNiagaraComponent* blood_effect = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), gameinstance->ground_dust_effect, hit_location, effect_rotator, FVector::OneVector, true, true, ENCPoolMethod::AutoRelease, true);
				UGameplayStatics::SpawnSoundAtLocation(this, gameinstance->sq_ground_hit, GetActorLocation(), FRotator::ZeroRotator, GetVelocity().Size() / 1500);
			}
			/* ragdoll 상태로 전환 */
			setCharacterState(ECharacterState::Ragdoll);
		}
	}
	//래그돌 위치 동기화 시스템
	else if (character_state == ECharacterState::Ragdoll) {
		SetReplicateMovement(false);
		getNetworkOwnerType(network_owner_type);
		is_on_action = false;
		//UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("[%i] ownedaoi : %i, remoteai : %i, ownedplayer : %i, remoteplayer : %i"), network_owner_type,ENetworkOwnerType::OwnedAI, ENetworkOwnerType::RemoteAI, ENetworkOwnerType::OwnedPlayer, ENetworkOwnerType::RemotePlayer));
		if (UKismetSystemLibrary::IsDedicatedServer(this) == false) {
			switch (network_owner_type)
			{
			case ENetworkOwnerType::OwnedAI:
			case ENetworkOwnerType::RemoteAI:
				if (is_simulation_responsible) {
					ragdoll_ClientOnwer_Implementation();
				}
				else {
					ragdoll_SyncLocation_Implementation();
				}
				break;
			case ENetworkOwnerType::OwnedPlayer:
				if (UKismetSystemLibrary::IsStandalone(this))
					ragdoll_ServerOnwer_Implementation();
				else
					ragdoll_ClientOnwer_Implementation();
				break;
			case ENetworkOwnerType::RemotePlayer:
				ragdoll_SyncLocation_Implementation();
				break;
			default:
				break;
			}
		}
	}
	

	knock_BackProcess();
	rotateProcess();
	hitBonePhysicalReactionProcess();
}

// Called to bind functionality to input
void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

// 리플리케이션 가능하게 하기
void ABaseCharacter::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ABaseCharacter, R_look_rotation);
	DOREPLIFETIME(ABaseCharacter, ragdoll_server_location);
	DOREPLIFETIME(ABaseCharacter, hp);
	DOREPLIFETIME(ABaseCharacter, simulation_responsible_actor);
	DOREPLIFETIME(ABaseCharacter, stamina);
	DOREPLIFETIME(ABaseCharacter, cur_max_stamina);
}

/// --> 인터페이스 함수 정의

/// <summary>
/// 캐릭터가 바라보는 방향을 결정
/// </summary>
void ABaseCharacter::setLookRotation_Implementation()
{
	FRotator tmp_rotator1 = UKismetMathLibrary::MakeRotator(0.0f, look_pitch, look_yaw);
	FRotator tmp_rotator2 = UKismetMathLibrary::NormalizedDeltaRotator(R_look_rotation, AActor::GetActorRotation());
	FRotator interp_tmp = UKismetMathLibrary::RInterpTo(tmp_rotator1, tmp_rotator2, d_time, 5.000000);
	look_pitch = UKismetMathLibrary::ClampAngle(interp_tmp.Pitch, -90.000000, 90.000000);
	look_yaw = UKismetMathLibrary::ClampAngle(interp_tmp.Yaw, -90.000000, 90.000000);
}

/// <summary>
/// 피격 액터가 데미지 프로세스를 처리하도록 전달
/// 체력 연산 후 멀티캐스트 함수 실행
/// </summary>
/// <param name="target_damage_data">전달할 데미지 데이터</param>
/// <param name="damage_causer">데미지를 입힌 액터</param>
void ABaseCharacter::applyDamage_Implementation(FName __target_damage_id, AActor* __damage_causer, FName __hit_bone_name, FVector __hit_location)
{
	FdamageData __target_damage_data;
	float causer_power;
	Cast<UPWOGameInstance>(GetGameInstance())->findDamageData(__target_damage_id, __target_damage_data);
	if (checkBlock(__target_damage_data, __damage_causer)) {
		stamina = UKismetMathLibrary::Max(0, stamina-10);
	}else{
		if (__damage_causer->GetClass()->ImplementsInterface(UInterface_BaseCharacter::StaticClass())) {
			IInterface_BaseCharacter::Execute_getBasePower(__damage_causer, causer_power);
			hp -= __target_damage_data.base_damage + __target_damage_data.base_damage_percent * causer_power;
			hp = UKismetMathLibrary::Max(0, hp);
		}
	}
	applyDamage_Multicast(__target_damage_id, __damage_causer, __hit_bone_name, __hit_location);
}

/// <summary>
/// 피격 액터 셋을 초기화하여 다음 공격에 피격될 수 있게 함
/// </summary>
void ABaseCharacter::resetHitActorList_Implementation() {
	hit_actors_list.Reset();
}


/// <summary>
/// 데미지데이터 업데이트
/// </summary>
/// <param name="target_damage_data">타겟 데미지 데이터</param>
void ABaseCharacter::setDamageData_Implementation(FdamageData __target_damage_data) {
	damage_data = __target_damage_data;
}

/// <summary>
/// 데미지 아이디 업데이트
/// </summary>
/// <param name="__target_damage_id"></param>
void ABaseCharacter::setDamageID_Implementation(FName __target_damage_id) {
	damage_id = __target_damage_id;
}

/// <summary>
/// 공격이 적에게 맞았을 때 피격액터에게 데미지 이벤트를 전달
/// owned 액터에서만 실행되도록 구현하여 네트워크 상황에서 한 번만 실행되도록 보장
/// </summary>
/// <param name="hit_actor">피격 액터</param>
void ABaseCharacter::attackEvent_Implementation(AActor* __hit_actor, FHitResult __hit_result) {
	bool flag = false;
	getNetworkOwnerType(network_owner_type);
	if (damage_data.attack_type == EAttackType::Earthquake && Cast<APawn>(__hit_actor)->GetMovementComponent()->IsFalling() == true) {
		return;
	}
	FName __hit_bone_name = __hit_result.BoneName;
	//UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("[%i] ownedaoi : %i, remoteai : %i, ownedplayer : %i, remoteplayer : %i"), network_owner_type, ENetworkOwnerType::OwnedAI, ENetworkOwnerType::RemoteAI, ENetworkOwnerType::OwnedPlayer, ENetworkOwnerType::RemotePlayer));
	if (HasAuthority()) {
		if (network_owner_type == ENetworkOwnerType::OwnedPlayer) {
			if (__hit_actor->GetOwner() != GetOwner()) {
				flag = true;
			}
		}
		if (UKismetSystemLibrary::IsDedicatedServer(this) == false) {
			if (network_owner_type == ENetworkOwnerType::OwnedAI) {
				if (__hit_actor == GetWorld()->GetFirstPlayerController()->GetPawn()) {
					flag = true;
				}
			}
		}
	}
	else {
		if (network_owner_type == ENetworkOwnerType::OwnedPlayer) {
			if (__hit_actor->GetOwner() != GetOwner()) {
				flag = true;
			}
		}
		if (network_owner_type == ENetworkOwnerType::RemoteAI) {
			if (__hit_actor == GetWorld()->GetFirstPlayerController()->GetPawn()) {
				flag = true;
			}
		}
	}
	bool hit_actor_is_dodge;
	if (flag && damage_data.attack_type != EAttackType::Earthquake) {
		if (__hit_actor->GetClass()->ImplementsInterface(UInterface_BaseCharacter::StaticClass())) {
			IInterface_BaseCharacter::Execute_getIsDodge(__hit_actor, hit_actor_is_dodge);
			if (hit_actor_is_dodge) {
				flag = false;
			}
		}
	}
	/* pelvis 하위 본 히트시에만 applyDamage*/
	//UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("[%d] %s"), GetMesh()->GetBoneIndex(__hit_bone_name), *(__hit_bone_name.ToString())));
	if (flag && (GetMesh()->GetBoneIndex(__hit_bone_name) < 9 )) {
		flag = false;
	}
	if (flag) {
		if (hit_actors_list.Contains(__hit_actor) == false) {
			if (GetWorld()->GetFirstPlayerController()->GetClass()->ImplementsInterface(UInterface_PlayerController::StaticClass())) {
				IInterface_PlayerController::Execute_CtoS_applyDamage(GetWorld()->GetFirstPlayerController(), __hit_actor, damage_id, this, __hit_bone_name, __hit_result.ImpactPoint);
			}
			hit_actors_list.Add(__hit_actor);
		}
		else {
			/* 피 이펙트 스폰 */
			/*if (hit_actor_is_dodge == false && Cast<ABaseCharacter>(__hit_actor)->checkBlock(damage_data, this) == false) {
				UPWOGameInstance* gameinstance = Cast<UPWOGameInstance>(GetGameInstance());
				UNiagaraComponent* blood_effect = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), gameinstance->blood_effect, __hit_result.ImpactPoint, __hit_result.ImpactNormal.ToOrientationRotator(), FVector::OneVector, true, true, ENCPoolMethod::AutoRelease, true);
			}*/
		}
	}
}

/// <summary>
/// is_on_action을 초기화 시켜 캐릭터가 액션을 수행할 수 있게 하고, 공격 시퀀스를 초기화
/// </summary>
void ABaseCharacter::resetNextAttack_Implementation(bool __is_on_action__toggle) {
	next_attack_montage = normal_attack_montage;
	if (__is_on_action__toggle)
		is_on_action = !is_on_action;
	next_attack_id = normal_attack_id;
}


/// <summary>
/// 데미지 데이터를 전달
/// </summary>
/// <param name="__damage_data">출력 데미지 데이터</param>
void ABaseCharacter::getDamageData_Implementation(FdamageData& __damage_data) {
	__damage_data = damage_data;
}

/// <summary>
/// 바라보는 방향 출력
/// </summary>
/// <param name="__look_rotation">출력 로테이션</param>
void ABaseCharacter::getLookRotation_Implementation(/*out*/ FRotator& __look_rotation) {
	__look_rotation = R_look_rotation;
}


/// <summary>
/// 타겟 로테이션을 출력
/// 경우에 따라 인풋 로테이션과 액터 로테이션을 선택
/// </summary>
/// <param name="__target_rotation">출력 타겟 로테이션</param>
void ABaseCharacter::getTargetRotation_Implementation(/*out*/ FRotator& __target_rotation) {
	bool is_moving = GetLastMovementInputVector().Size() > 0.0f;
	if (is_moving) {
		FRotator input_rotation = UKismetMathLibrary::MakeRotFromX(GetLastMovementInputVector());
		__target_rotation = input_rotation;
	}
	else {
		__target_rotation = GetActorRotation();
	}
}

/// <summary>
/// 공격 트레이스시 사용할 소켓 이전 위치 변수 업데이트
/// </summary>
/// <param name="start">시작 지점 소켓 위치</param>
/// <param name="end">끝 지점 소켓 위치</param>
void ABaseCharacter::setPrevSockLoc_Implementation(FVector __start, FVector __end) {
	prev_attack_sock_start_loc = __start;
	prev_attack_sock_end_loc = __end;
}

/// <summary>
/// 공격 트레이스시 사용할 소켓 이전 위치 변수 전달
/// </summary>
/// <param name="__start"></param>
/// <param name="__end"></param>
void ABaseCharacter::getPrevSockLoc_Implementation(/*out*/ FVector& __start, /*out*/ FVector& __end) {
	__start = prev_attack_sock_start_loc;
	__end = prev_attack_sock_end_loc;
}


/// <summary>
/// 어택 트레이스에 사용할 트레이스 채널을 반환
/// </summary>
/// <param name="__attack_trace_channel">출력 어택 트레이스 채널</param>
ETraceTypeQuery ABaseCharacter::getAttackTraceChannel_Implementation() {
	return ETraceTypeQuery::TraceTypeQuery4;
}

/// <summary>
/// 캐릭터 보간 회전
/// </summary>
/// <param name="__target_rotation">회전 목표 로테이션</param>
/// <param name="__delta_time">델타 타임</param>
/// <param name="__speed">보간 속도</param>
void ABaseCharacter::rotateActorInterp_Implementation(FRotator __target_rotation, float __delta_time, float __speed) {
	rotate_interp_time_end = 0;
	FRotator interp_rotator = UKismetMathLibrary::RInterpTo(GetActorRotation(),__target_rotation, d_time,__speed);
	//GetMovementComponent().repli 
	getNetworkOwnerType(network_owner_type);
	if (HasAuthority()) {
		SetActorRotation(interp_rotator.Quaternion(), ETeleportType::TeleportPhysics);
	}
	else {
		if (network_owner_type == ENetworkOwnerType::RemoteAI) {
			SetActorRotation(interp_rotator.Quaternion(), ETeleportType::TeleportPhysics);
		}
		else {
			CtoS_setRotation(interp_rotator);
			SetActorRotation(interp_rotator.Quaternion(), ETeleportType::TeleportPhysics);
		}
	}
}

/// <summary>
/// 캐릭터 상태 반환
/// </summary>
/// <param name="__output_character_state">출력 캐릭터 상태</param>
void ABaseCharacter::getCharacterState_Implementation(/*out*/ ECharacterState& __output_character_state) {
	__output_character_state = character_state;
}

/// <summary>
/// is_on_sprint 변수 반환
/// 캐릭터가 스프린트 중인지 반환
/// </summary>
/// <param name="__output_is_on_sprint">출력 스프린트 여부 변수</param>
void ABaseCharacter::getIsOnSprint_Implementation(/*out*/ bool& __output_is_on_sprint) {
	if (knock_back_speed > 0) {
		__output_is_on_sprint = false;
	}
	else {
		__output_is_on_sprint = is_on_sprint;
	}
}

/// <summary>
/// current_velocity 변수 반환
/// : 무브먼트 벨로시티에 넉백 벨로시티등을 다 합친 벨로시티의 합
/// </summary>
/// <param name="__output_current_velocity">출력 current_velocity</param>
void ABaseCharacter::getCurrentVelocity_Implementation(FVector& __output_current_velocity) {
	//__output_current_velocity = UKismetMathLibrary::Add_VectorVector(GetVelocity(), current_velocty);
	//__output_current_velocity = GetVelocity();
	//__output_current_velocity = GetCharacterMovement()->Velocity;
	if (network_owner_type == ENetworkOwnerType::RemoteAI) {
		if (current_velocty.Size() > 0) {
			__output_current_velocity = current_velocty;
		}
		else {
			__output_current_velocity = GetVelocity();
		}
	}
	else {
		__output_current_velocity = GetVelocity();
	}
}

/// <summary>
/// 바라보기 관련 변수 반환
/// </summary>
/// <param name="__output_look_pitch">출력 피치</param>
/// <param name="__output_look_yaw">출력 요</param>
void ABaseCharacter::getLookDirection_Implementation(float& __output_look_pitch, float& __output_look_yaw) {
	__output_look_pitch = look_pitch;
	__output_look_yaw = look_yaw;
}

/// <summary>
/// 다음 공격시 실행할 애니메이션 몽타주를 변경
/// </summary>
/// <param name="__next_attack_anim">다음 실행할 애니메이션 몽타주</param>
void ABaseCharacter::setNextAttackMontage_Implementation(UAnimMontage* __next_attack_anim) {
	next_attack_montage = __next_attack_anim;
}

/// <summary>
/// 액션을 수행할 수 있을지 말지 결정하는 변수를 업데이트
/// </summary>
/// <param name="__target_is_on_action">액션 수행 가능 여부</param>
void ABaseCharacter::setIsOnAction_Implementation(bool __target_is_on_action) {
	is_on_action = __target_is_on_action;
}

/// <summary>
/// 액터를 일정 시간동안 목표 로테이션으로 회전시킴
/// </summary>
/// <param name="__target_rotation">목표 로테이션</param>
/// <param name="__time">회전에 걸리는 시간</param>
void ABaseCharacter::rotateActorWithInTime_Implementation(FRotator __target_rotation, float __time) {
	rotate_target_rotation = __target_rotation;
	rotate_original_rotation = GetActorRotation();
	rotate_interp_time_end = __time;
	rotate_interp_time = 0;
}

/// <summary>
/// 베이스 파워 반환
/// </summary>
/// <param name="__output_base_power"></param>
void ABaseCharacter::getBasePower_Implementation(float& __output_base_power) {
	__output_base_power = base_power + additive_power;
}

/// <summary>
/// 다음 기본공격 id 설정
/// </summary>
/// <param name="__next_action_id"></param>
void ABaseCharacter::setNextAttackID_Implementation(FName __next_action_id) {
	next_attack_id = __next_action_id;
}

/// <summary>
/// attack_collisions TMap 에서 키와 매칭되는 프리미티브 컴포넌트를 반환
/// </summary>
/// <param name="__key">활성화 할 프리미티브 컴포넌트 키</param>
/// <param name="__weapon">반환 피리미티브 컴포넌트</param>
void ABaseCharacter::getWeapon_Implementation(FName __key, /*out*/ UPrimitiveComponent*& __weapon) {
	if(attack_collisions.Contains(__key))
		__weapon = attack_collisions[__key];
}

void ABaseCharacter::getDurabilityLevel_Implementation(uint8& __output_durability_level) {
	__output_durability_level = durability_level;
}

void ABaseCharacter::setIsDodge_Implementation(bool __target_is_dodge) {
	is_dodge = __target_is_dodge;
}

void ABaseCharacter::getIsDodge_Implementation(bool& __output_is_dodge) {
	__output_is_dodge = is_dodge;
}

void ABaseCharacter::recoverHP_Implementation(float __amount)
{
	hp = UKismetMathLibrary::FMin(getStat_Hp(), hp + __amount);
}
void ABaseCharacter::recoverStaminaMax_Implementation(float __amount)
{
	cur_max_stamina = UKismetMathLibrary::FMin(getStat_Stamina(), cur_max_stamina + __amount);
	stamina = UKismetMathLibrary::FMin(cur_max_stamina, stamina + __amount);
}
void ABaseCharacter::increaseStat_Hp_Implementation(float __amount)
{
	additive_hp += __amount;
	hp = UKismetMathLibrary::FMin(getStat_Hp(), hp);
}
void ABaseCharacter::increaseStat_Stamina_Implementation(float __amount)
{
	additive_stamina += __amount;
	cur_max_stamina = UKismetMathLibrary::FMin(getStat_Stamina(), cur_max_stamina);
	stamina = UKismetMathLibrary::FMin(cur_max_stamina, stamina);
}

void ABaseCharacter::increaseStat_Power_Implementation(float __amount)
{
	additive_power += __amount;
}
void ABaseCharacter::increaseStat_Speed_Implementation(float __amount)
{
	additive_speed += __amount;
	GetCharacterMovement()->MaxWalkSpeed = additive_speed + walk_speed;
	is_on_sprint = false;
}

float ABaseCharacter::getStat_Speed_Implementation()
{
	return walk_speed + additive_speed;
}

float ABaseCharacter::getStat_Hp_Implementation()
{
	return max_hp + additive_hp;
}

float ABaseCharacter::getStat_Stamina_Implementation()
{
	return max_stamina + additive_stamina;
}

//void ABaseCharacter::changeWeaponMesh_Implementation(FName __item_id)
//{
//
//}

//void ABaseCharacter::spawnAttackDecal(FTransform __decal_transform)
//{
//
//}

// <-- 인터페이스 함수 정의 끝



// --> 클래스 멤버 함수 정의


/// <summary>
/// 서버에서 체력연산 후 멀티캐스트로 넉백 등을 처리
/// </summary>
/// <param name="target_damage_data">전달할 데미지 데이터</param>
/// <param name="damage_causer">데미지를 입힌 액터</param>
void ABaseCharacter::applyDamage_Multicast_Implementation(FName __target_damage_id, AActor* damage_causer, FName __hit_bone_name, FVector __hit_location)
{
	getNetworkOwnerType(network_owner_type);
	applyDamage_Multicast_Exec(__target_damage_id, damage_causer, __hit_bone_name, __hit_location);
}

/// <summary>
/// applyDamage_Multicast의 실제 구현 (블루프린트에서 오버라이딩 할 수 있게 하기 위함)
/// 피격 액터의 durability_level 이 공격의 durability_level 보다 크면 넉백과 에어본, 경직을 무시하고 히트 부위에 피지컬 애니메이션 실행
/// 이외의 상황에서 damage_id를 통해 DamageData를 구하고 해당 DamageData의 넉백 벡터와 offset, 공격 액터의 방향과 현재 액터의 위치 관계에 따라 넉백 벡터를 회전해서 적용
/// </summary>
/// <param name="__target_damage_id">데미지 id</param>
/// <param name="damage_causer">공격한 액터</param>
/// <param name="__hit_bone_name">피격된 본</param>
void ABaseCharacter::applyDamage_Multicast_Exec_Implementation(FName __target_damage_id, AActor* damage_causer, FName __hit_bone_name, FVector __hit_location) {
	FdamageData target_damage_data;
	UPWOGameInstance* gameinstance = Cast<UPWOGameInstance>(GetGameInstance());
	gameinstance->findDamageData(__target_damage_id, target_damage_data);
	// 넉백 벡터를 넉백타입과 방향에 맞게 회전
	FVector rotated_vector = rotateKnockBackVector(target_damage_data.knock_back_type, target_damage_data.knock_back, target_damage_data.knock_back_offset, damage_causer);
	if (checkBlock(target_damage_data, damage_causer)) {
		/* target_control 을 None 으로 변환해 에어본 되지 않도록 함 */
		target_damage_data.target_control = ETargetControlType::None;
		setupTargetControl(target_damage_data, rotated_vector/2);
		/* target_control 에 따라 넉백 진행 */
		blockProcess(target_damage_data, rotated_vector, damage_causer);
	}else if (durability_level >= target_damage_data.durability_level) {
		// 강인도가 데미지의 강인도 수치보다 높을시 히트 부위 덜렁거리는 피지컬 애니메이션 실행
		/* 피 나이아가라 이펙트 스폰 */
		spawnBloodEffect(__hit_location, rotated_vector, __hit_bone_name);
		playPhysicalHitAnimation(__hit_bone_name, damage_causer);
		return;
	}
	else {
		// 그 외의 노말 히트인 경우 에어본, 넉백 등을 적용
		/* 피 나이아가라 이펙트 스폰 */
		spawnBloodEffect(__hit_location, rotated_vector, __hit_bone_name);
		/* 히트 애니메이션 사운드 재생 */
		UAnimMontage* hit_anim = nullptr;
		selectHitAnimation(rotated_vector, hit_anim);
		animation_Sound_Multicast(hit_anim, sq_hit);
		/* target_control 에 따라 넉백 적용 */
		setupTargetControl(target_damage_data, rotated_vector);
	}
}

/// <summary>
/// 넉백 벡터를 knock_back_type 과 damage_causer 의 위치와 방향에 맞게 회전한 결과를 반환
/// </summary>
/// <param name="__knock_back_type"></param>
/// <param name="__knock_back"></param>
/// <param name="__knock_back_offset"></param>
/// <param name="damage_causer"></param>
/// <returns></returns>
FVector ABaseCharacter::rotateKnockBackVector(EKnockBackType __knock_back_type, FVector __knock_back, FVector __knock_back_offset, AActor* damage_causer)
{
	FVector rotated_vector;
	FVector rotated_offset = UKismetMathLibrary::Quat_RotateVector(damage_causer->GetActorRotation().Quaternion(), __knock_back_offset);
	FVector knock_back_point_vector = damage_causer->GetActorLocation() + rotated_offset;
	if (__knock_back_type == EKnockBackType::Directional)
		/* 넉백 타입이 Directinal 일때 공격 시전자 방향 기준으로 넉백 벡터 회전 */
		rotated_vector = UKismetMathLibrary::Quat_RotateVector(damage_causer->GetActorRotation().Quaternion(), __knock_back);
	else if (__knock_back_type == EKnockBackType::RadialXY) {
		/* RadialXY 일 때 공격 시전자와의 look at 방향으로 넉백 벡터 회전 시키고 pitch 와 roll 0으로 초기화 */
		FRotator rotate_quat = UKismetMathLibrary::FindLookAtRotation(knock_back_point_vector, GetActorLocation());
		rotate_quat.Pitch = 0;
		rotate_quat.Roll = 0;
		rotated_vector = UKismetMathLibrary::Quat_RotateVector(rotate_quat.Quaternion(), __knock_back);
	}
	else if (__knock_back_type == EKnockBackType::RadialXYDistanceReverse) {
		/* RadialXYDistanceReverse 일 때 공격 시전자와의 look at 방향으로 넉백 벡터 회전 시키고 pitch 와 roll 0으로 초기화 한 후 시전자와의 거리를 곱해줌  */
		FRotator rotate_quat = UKismetMathLibrary::FindLookAtRotation(knock_back_point_vector, GetActorLocation());
		rotate_quat.Pitch = 0;
		rotate_quat.Roll = 0;
		rotated_vector = UKismetMathLibrary::Quat_RotateVector(rotate_quat.Quaternion(), __knock_back);
		float distance = UKismetMathLibrary::Vector_Distance(knock_back_point_vector, GetActorLocation());
		rotated_vector.X *= distance;
		rotated_vector.Y *= distance;
	}
	else {
		/* 이외의 타입에서는 directional 을 디폴트로 적용 ( 나머지는 아직 쓸 일이 없어서 미구현 ) */
		rotated_vector = UKismetMathLibrary::Quat_RotateVector(damage_causer->GetActorRotation().Quaternion(), __knock_back);
	}
	return rotated_vector;
}


/// <summary>
/// 서버에서 애니메이션과 사운드를 멀티캐스트로 실행
/// </summary>
/// <param name="anim">애니메이션</param>
/// <param name="sound">사운드</param>
void ABaseCharacter::animation_Sound_Multicast_Implementation(UAnimMontage* anim, USoundBase* sound) {
	if(anim != nullptr)
		PlayAnimMontage(anim);
	UGameplayStatics::SpawnSoundAtLocation(this, sound, GetActorLocation());
}

/// <summary>
/// 공격을 방어했는지 판단
/// </summary>
/// <param name="target_damage_data"></param>
/// <param name="damage_causer"></param>
/// <returns></returns>
bool ABaseCharacter::checkBlock(FdamageData target_damage_data, AActor* damage_causer)
{
	if (is_block) {
		FVector loc1 = GetActorForwardVector();
		loc1.Z = 0;
		FVector loc2 = damage_causer->GetActorLocation() - GetActorLocation();
		loc2.Z = 0;
		UKismetSystemLibrary::PrintString(this, loc1.GetSafeNormal().ToString());
		UKismetSystemLibrary::PrintString(this, loc2.GetSafeNormal().ToString());
		float angle = UKismetMathLibrary::Dot_VectorVector(loc1.GetSafeNormal(), loc2.GetSafeNormal());
		if (angle > 0.2f) {
			/* 적과의 각의 코사인이 0.2 보다 크면 가드로 인정*/
			return true;
		}
	}
	return false;
}

void ABaseCharacter::blockProcess_Implementation(FdamageData target_damage_data, FVector knock_back_vector, AActor* damage_causer)
{

}

void ABaseCharacter::spawnBloodEffect(FVector __location, FVector __rot_vector, FName __bone_name)
{
	UPWOGameInstance* gameinstance = Cast<UPWOGameInstance>(GetGameInstance());
	UNiagaraComponent* blood_effect = UNiagaraFunctionLibrary::SpawnSystemAttached(gameinstance->blood_effect, GetMesh(), __bone_name, FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::Type::SnapToTarget, true, true, ENCPoolMethod::AutoRelease, true);
	//UNiagaraComponent* blood_effect = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), gameinstance->blood_effect, __hit_location, blood_rot, FVector::OneVector, true, true, ENCPoolMethod::AutoRelease, true);
	if (blood_effect) {
		/* 피 이펙트를 rotation을 absolute 로 바꾸고 회전이 적용된 넉백 벡터의 반대 방향으로 world rotation 적용, location은 표면 트레스한 위치로 옮김 */
		blood_effect->SetUsingAbsoluteRotation(true);
		blood_effect->SetWorldLocation(__location);
		FRotator blood_rot = __rot_vector.Rotation();
		blood_rot.Pitch *= -1;
		blood_rot.Yaw += 180;
		blood_effect->SetWorldRotation(blood_rot);
	}
}

/// <summary>
/// 슈퍼아머 상태에서 히트시 히트 부위 덜렁거리는 피지컬 애니메이션
/// 히트본을 hit_bone_physics_weight_map 에 등록시켜 tick 함수에서 피직스 블렌딩 수행
/// </summary>
/// <param name="__hit_bone_name"></param>
void ABaseCharacter::playPhysicalHitAnimation(FName __hit_bone_name, AActor* damage_causer)
{
	if (character_state == ECharacterState::Walk_and_Jump && UKismetSystemLibrary::IsDedicatedServer(this) == false) {
		GetMesh()->SetAllBodiesBelowSimulatePhysics(__hit_bone_name, true);
		if (hit_bone_physics_weight_map.Contains(__hit_bone_name)) {
			hit_bone_physics_weight_map[__hit_bone_name] = 0.8f;
		}
		else {
			if (GetMesh()->IsSimulatingPhysics(GetMesh()->GetParentBone(__hit_bone_name)) == false) {
				hit_bone_physics_weight_map.Add(TTuple<FName, float>(__hit_bone_name, 0.8f));
			}
		}
		GetMesh()->AddImpulse(damage_causer->GetActorForwardVector() * 300, __hit_bone_name, true);
	}
	animation_Sound_Multicast(nullptr, sq_hit);
}

/// <summary>
/// 일반적인 히트시 damage_date 의 target_control 에 따라 넉백, 에어본을 적용시키고
/// 히트 애니메이션과 사운드를 출력
/// </summary>
/// <param name="target_damage_data"></param>
/// <param name="rotated_vector"></param>
void ABaseCharacter::setupTargetControl(FdamageData target_damage_data, FVector rotated_vector)
{
	rotate_interp_time = 0;
	if (character_state == ECharacterState::Ragdoll) {
		/* 래그돌 상태(다운 상태)에서 맞았을 때 airbone 으로 전환 */
		ragdollGetUp();
		setCharacterState(ECharacterState::Airbone);
	}
	FVector hitnormal;
	FVector hit_location;
	if (airbone_HitChk(rotated_vector, hitnormal, hit_location)) {
		/* 에어본 직후 벽과 충돌시 에어본 넉백 방향을 히트 표면의 노말방향으로 전환 */
		float rotated_std = rotated_vector.Size();
		rotated_vector = hitnormal * rotated_std;
	}
	if (target_damage_data.target_control == ETargetControlType::None) {
		/* 타겟 컨트롤이 None이면 에어본 중이 아니면 넉백을 실행하고 아닐시 LaunchCharacter로 캐릭터를 날림 */
		if (character_state == ECharacterState::Walk_and_Jump) {
			applyKnock_Back(rotated_vector);
		}
		else {
			LaunchCharacter(UKismetMathLibrary::MakeVector(rotated_vector.X * 2, rotated_vector.Y * 2, rotated_vector.Z), true, true);
		}
	}
	else if (target_damage_data.target_control == ETargetControlType::Ragdoll) {
		/* 타겟 컨트롤이 Ragdoll 일시 캐릭터를 날림 */
		/* 넉백 벡터 초기화 및 해제 */
		knock_back_unit_vector = FVector::ZeroVector;
		if (network_owner_type == ENetworkOwnerType::RemoteAI)
			current_velocty = FVector::ZeroVector;
		knock_back_speed = 0;
		/* 스프린트 해제 */
		if (is_on_sprint)
			GetCharacterMovement()->MaxWalkSpeed = sprint_speed;
		else
			GetCharacterMovement()->MaxWalkSpeed = walk_speed;
		GetCharacterMovement()->MaxAcceleration = 2048.0f;
		ConsumeMovementInputVector();
		LaunchCharacter(UKismetMathLibrary::MakeVector(rotated_vector.X * 2, rotated_vector.Y * 2, rotated_vector.Z), true, true);
		/* 한 프레임 뒤에 에어본 시작 */
		GetWorld()->GetTimerManager().SetTimerForNextTick(FTimerDelegate::CreateLambda([&, rotated_vector]() {
			airboneStart(rotated_vector);
			}));
	}
}

/// <summary>
/// 입력된 velocity의 방향에 대응되는 피격 애니메이션 출력
/// </summary>
/// <param name="velocity">넉백 벨로시티</param>
/// <param name="hit_anim">출력되는 피격 애니메이션 몽타주</param>
void ABaseCharacter::selectHitAnimation_Implementation(FVector velocity, UAnimMontage*& hit_anim) {
	float forward_angle = UKismetMathLibrary::Vector_CosineAngle2D(velocity, GetActorForwardVector());
	float right_angle = UKismetMathLibrary::Vector_CosineAngle2D(velocity, GetActorRightVector());
	if (forward_angle > 0.5f)
		hit_anim = hit_b_anim;
	else if (forward_angle < -0.5f)
		hit_anim = hit_f_anim;
	else if (right_angle > 0.0f)
		hit_anim = hit_l_anim;
	else
		hit_anim = hit_r_anim;
}

void ABaseCharacter::rotateProcess_Implementation() {
	if (rotate_interp_time_end > 0) {
		rotate_interp_time += d_time;
		float ease_alpha = rotate_interp_time / rotate_interp_time_end;
		FRotator ease_res = UKismetMathLibrary::REase(rotate_original_rotation, rotate_target_rotation, ease_alpha, true,EEasingFunc::EaseOut);
		SetActorRotation(ease_res, ETeleportType::None);
		if (ease_alpha >= 1.0f) {
			rotate_interp_time_end = 0.0f;
			SetActorRotation(rotate_target_rotation, ETeleportType::TeleportPhysics);
		}
		else {
			SetActorRotation(ease_res, ETeleportType::TeleportPhysics);
		}
	}
}

/// <summary>
/// 넉백 연산 수행 (매 틱 실행)
/// </summary>
/// <param name="velocity">넉백 벨로시티</param>
void ABaseCharacter::knock_BackProcess_Implementation() {
	if (knock_back_speed > 0) {
		//knock_back_unit_vector = UKismetMathLibrary::VInterpTo(knock_back_unit_vector, FVector::ZeroVector, d_time, 5);
		knock_back_speed = UKismetMathLibrary::FInterpTo(knock_back_speed, 0, d_time, 5);
		//UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("ASDF %f"),knock_back_speed));
		if (knock_back_speed <= 5.0f) {
			endKnock_Back();
		}
		else {
			GetCharacterMovement()->MaxWalkSpeed = knock_back_speed * 4;
			AddMovementInput(knock_back_unit_vector, 1.0f);
			if(network_owner_type == ENetworkOwnerType::RemoteAI)
				current_velocty = knock_back_unit_vector * (GetCharacterMovement()->MaxWalkSpeed);
			//UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("ASDF %f"), GetCharacterMovement()->MaxWalkSpeed));
		}
		//UKismetSystemLibrary::PrintString(this, GetCharacterMovement()->Velocity.ToString());
	}
}

/// <summary>
/// 넉백프로세스에서 사용되는 변수 초기화
/// </summary>
void ABaseCharacter::applyKnock_Back_Implementation(FVector velocity) {
	//UKismetSystemLibrary::PrintString(this, TEXT("아아"));
	float vel_length = velocity.Size();
	knock_back_unit_vector = UKismetMathLibrary::MakeVector(velocity.X, velocity.Y, 0);
	knock_back_speed = knock_back_unit_vector.Size();
	knock_back_unit_vector = knock_back_unit_vector.GetSafeNormal();
	rotate_interp_time_end = 0;
	is_on_action = true;
	if (velocity.Z > 0) {
		LaunchCharacter(UKismetMathLibrary::MakeVector(0.0f,0.0f,velocity.Z),false,false);
	}
	GetCharacterMovement()->MaxAcceleration = 1000000.f;
	//SetReplicateMovement(false);
}


void ABaseCharacter::endKnock_Back() {
	//SetReplicateMovement(true);
	knock_back_unit_vector = FVector::ZeroVector;
	if (network_owner_type == ENetworkOwnerType::RemoteAI)
		current_velocty = FVector::ZeroVector;
	knock_back_speed = 0;
	if (is_on_sprint)
		GetCharacterMovement()->MaxWalkSpeed = getStat_Speed()+500;
	else
		GetCharacterMovement()->MaxWalkSpeed = getStat_Speed();
	GetCharacterMovement()->MaxAcceleration = 2048.0f;
	
}

/// <summary>
/// 래그돌 상태에서 엉덩이 아래로 라인 트레이스 하여 래그돌이 지면에 있을 시 액터를 지면 바로 위로 붙임
/// </summary>
/// <param name="location">액터의 위치</param>
void ABaseCharacter::stickToTheGround(FVector location) {
	float capsule_half_height = this->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
	FVector trace_start = UKismetMathLibrary::MakeVector(location.X, location.Y, location.Z + capsule_half_height);
	FVector trace_end = UKismetMathLibrary::MakeVector(location.X, location.Y, location.Z - capsule_half_height);
	FHitResult trace_result(ForceInit);
	//FCollisionQueryParams collision_params;
	GetWorld()->LineTraceSingleByChannel(trace_result, trace_start, trace_end, ECC_GameTraceChannel1);
	// 라인트레이스 결과에 따른 액터의 방향과 위치 설정
	if (trace_result.IsValidBlockingHit()) {
		is_ragdoll_on_the_ground = true;
		// 엉덩이가 위를 향하고 있으면 액터의 방향을 180도 회전 ( 일어서는 애니메이션을 위함 )
		FRotator sock_rotator = GetMesh()->GetSocketRotation(TEXT("pelvis"));
		is_ragdoll_face_up = sock_rotator.Roll < 0.0f;

		sock_rotator.Roll = 0;
		sock_rotator.Pitch = 0;
		if (is_ragdoll_face_up) 
			sock_rotator.Yaw -= 180.0f;
		
		FVector landed_location = UKismetMathLibrary::MakeVector(location.X, location.Y, trace_result.Location.Z + capsule_half_height);
		SetActorLocation(landed_location);
		SetActorRotation(sock_rotator);
		//UKismetSystemLibrary::PrintString(this, sock_rotator.ToString());
	}
	else {
		is_ragdoll_on_the_ground = false;
		SetActorLocation(location);
	}
}

/// <summary>
/// 클라이언트 소유 액터 : 래그돌의 위치로 액터를 이동시키고 서버에서 위치를 리플리케이트 할 수 있도록 서버로 전달
/// 0.2초 간격으로 RPC를 통해 서버의 ragdoll_server_location를 업데이트
/// </summary>
void ABaseCharacter::ragdoll_ClientOnwer_Implementation() {
	FVector sock_location = GetMesh()->GetSocketLocation("pelvis");
	ragdoll_server_location = sock_location;
	stickToTheGround(sock_location);
	if (replication_delay_count >= last_replication_delay) {
		replication_delay_count += d_time;
		replication_delay_count -= last_replication_delay;
		last_replication_delay = 0.2f;
		//UKismetSystemLibrary::PrintString(this, GetMesh()->GetPhysicsLinearVelocity(TEXT("pelvis")).ToString());
		if (UKismetMathLibrary::Vector_Distance(prev_ragdoll_server_location, ragdoll_server_location) > 1) {
			Cast<ABaseCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn())->CtoS_targetLocation(this, ragdoll_server_location);
			prev_ragdoll_server_location = ragdoll_server_location;
		}
	}
	else {
		replication_delay_count += d_time;
	}
}


/// <summary>
/// 서버 소유 액터 or 스탠드얼론 : 래그돌의 위치로 액터를 이동
/// </summary>
void ABaseCharacter::ragdoll_ServerOnwer_Implementation() {
	FVector sock_location = GetMesh()->GetSocketLocation("pelvis");
	ragdoll_server_location = sock_location;
	stickToTheGround(ragdoll_server_location);
}


/// <summary>
/// 클라이언트 소유 액터에게 위치를 전달받으면 변수를 업데이트해서 리플리케이트
/// (AI캐릭터의 경우 target_actor 파라미터로 자신을 전달 해서 클라이언트 소유 액터를 통해 자신의 ragdoll_target_location 변수를 업데이트)
/// </summary>
/// <param name="target_actor">위치를 업데이트할 액터</param>
/// <param name="target_location">업데이트할 위치</param>
void ABaseCharacter::CtoS_targetLocation_Implementation(ABaseCharacter* target_actor, FVector target_location) {
	target_actor->ragdoll_server_location = target_location;
	target_actor->stickToTheGround(target_location);
	//target_actor->ragdoll_server_location = target_location;
}

/// <summary>
/// 래그돌 위치 동기화
/// 피직스 핸들을 이용해 서버의 타겟 위치로 래그돌을 옮기는 역할 수행
/// </summary>
void ABaseCharacter::ragdoll_SyncLocation_Implementation() {

	ragdoll_physics_handle->GrabComponent(GetMesh(), TEXT("pelvis"), GetMesh()->GetSocketLocation(TEXT("pelvis")), true);
	// 래그돌 위치 갱신 여부 확인
	if (ragdoll_server_location == last_ragdoll_server_location) {
		// 위치 갱신 안되었을 때
		replication_delay_count += d_time;
	}
	else {
		// 위치 갱신 되었을 때
		float tmp = replication_delay_count - last_replication_delay;
		last_replication_delay = replication_delay_count;
		replication_delay_count = tmp + d_time;
		prev_ragdoll_server_location = last_ragdoll_server_location;
	}

	// 피직스 핸들로 서버 갱신 주기 사이마다 래그돌을 서버 위치로 움직임 예측 보간 하며 옮김
	//if(ragdoll_physics_handle->IsActive())
	//if (UKismetMathLibrary::Vector_Distance(prev_ragdoll_server_location, ragdoll_server_location) >= 5) {
	//	float ease_alpha = replication_delay_count / last_replication_delay;
	//	FVector predicted_location = UKismetMathLibrary::VEase(prev_ragdoll_server_location, ragdoll_server_location, ease_alpha, EEasingFunc::Linear);
	//	ragdoll_physics_handle->SetTargetLocation(predicted_location);
	//	stickToTheGround(ragdoll_server_location);
	//}else{
	//	/*UKismetSystemLibrary::PrintString(this, TEXT("ASDAF"));
	//	GetRootComponent()->SetWorldLocationAndRotationNoPhysics(ragdoll_server_location, FRotator::ZeroRotator);*/
	//}
	float ease_alpha = replication_delay_count / last_replication_delay;
	FVector predicted_location = UKismetMathLibrary::VEase(prev_ragdoll_server_location, ragdoll_server_location, ease_alpha, EEasingFunc::Linear);
	ragdoll_physics_handle->SetTargetLocation(ragdoll_server_location);
	stickToTheGround(ragdoll_server_location);
	last_ragdoll_server_location = ragdoll_server_location;
}

/// <summary>
/// 캐릭터 에어본시 설정 초기화 및 에어본 애니메이션 재생
/// </summary>
void ABaseCharacter::airboneStart_Implementation(FVector __velocity) {
	is_on_sprint = false;
	is_on_action = false;
	USkeletalMeshComponent* mesh = GetMesh();
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	mesh->SetAllBodiesBelowSimulatePhysics("pelvis", true, false);
	//CastChecked<UAnimMontage>(CastChecked<UDynamicClass>(ABP_BaseCharacter_C__pf503790866::StaticClass())->UsedAssets[6]
	FVector forward = GetActorForwardVector();
	float speed_rate = __velocity.Size()/2000;
	if (FVector::DotProduct(__velocity, forward) < 0) {
		PlayAnimMontage(airbone_b_anim, speed_rate);
	}
	else {
		PlayAnimMontage(airbone_b_anim, -speed_rate);
	}
	character_state = ECharacterState::Airbone;
}

/// <summary>
/// 래그돌 상태에서 일어서는 설정 초기화 및 애니메이션 재생
/// </summary>
void ABaseCharacter::ragdollGetUp_Implementation() {
	// 무브먼트 리플리케이션 재 실행 후 현재 래그돌 포즈 스냅샷으로 저장
	SetReplicateMovement(true);
	if (main_anim_instance && main_anim_instance->IsValidLowLevel()) {
		main_anim_instance->SavePoseSnapshot(TEXT("RagdollPose"));
	}
	// 무브먼트 모드를 walking 으로 변경 후 엉덩이 방향에 따라 일어서는 애니메이션 몽타주 재생
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	if (is_ragdoll_face_up) {
		PlayAnimMontage(get_up_b_anim);
	}
	else {
		PlayAnimMontage(get_up_f_anim);
	}

	//메쉬와 액터의 콜리전,피직스 설정 변환
	USkeletalMeshComponent* mesh = GetMesh();

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	//mesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	mesh->SetSimulatePhysics(false);
	is_on_action = false;
	if(ragdoll_physics_handle && ragdoll_physics_handle->IsValidLowLevel())
		ragdoll_physics_handle->ReleaseComponent();
}

/// <summary>
/// 캐릭터 상태에 변화에 따른 세팅값 결정
/// </summary>
/// <param name="target_character_state">목표 캐릭터 상태</param>
void ABaseCharacter::setCharacterState_Implementation(ECharacterState target_character_state) {
	if (target_character_state != character_state) {
		switch (target_character_state)
		{
		case ECharacterState::Walk_and_Jump:
			GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
			GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
			//GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
			character_state = ECharacterState::Walk_and_Jump;
			break;
		case ECharacterState::Airbone:
			airboneStart(GetVelocity());
			break;
		case ECharacterState::Ragdoll:
			is_on_sprint = false;
			is_on_action = false;
			ragdoll_SetOnServer();
			break;
		case ECharacterState::Death:
			GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
			GetMesh()->SetAllBodiesBelowSimulatePhysics("pelvis", true, true);
			character_state = ECharacterState::Death;
		default:
			break;
		}
	}
}

/// <summary>
/// 서버에서 래그돌 세팅 수행
/// 가장 가까운 플레이어를 시뮬레이션 담당 액터로 저장
/// </summary>
void ABaseCharacter::ragdoll_SetOnServer_Implementation() {
	ragdoll_server_location = GetMesh()->GetSocketLocation("pelvis");
	/* 래그돌 시뮬레이션을 당담해야할 클라이언트 판단 기준 : 소유 클라이언트 or 클라이언트 소유 액터가 래그돌 전환 액터에게 가장 가까운 액터일 때 */
	findClosestPlayer(simulation_responsible_actor);
	ragdoll_SetMultiCast(simulation_responsible_actor);
}

/// <summary>
/// 멀티캐스트로 래그돌 세팅 동기화
/// 실행중인 모든 애니메이션 몽타주 종료하고 래그돌로 전환
/// 래그돌 동기화 관련 프로퍼티 초기화
/// </summary>
void ABaseCharacter::ragdoll_SetMultiCast_Implementation(AActor* responsible_actor) {
	/* 서버는 래그돌 시뮬레이션을 수행하지 않기 위해 데디케이티드 서버는 래그돌로 전환하지 않음  */
	if (UKismetSystemLibrary::IsDedicatedServer(this) == false) {
		ragdoll_server_location = GetMesh()->GetSocketLocation("pelvis");
		prev_ragdoll_server_location = ragdoll_server_location;
		last_ragdoll_server_location = ragdoll_server_location;
		replication_delay_count = 0.0f;
		last_replication_delay = 0.1f;
		is_ragdoll_on_the_ground = false;
		FVector tmpvec = GetVelocity();
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		GetMesh()->SetAllBodiesBelowSimulatePhysics("pelvis", true, true);
		is_simulation_responsible = responsible_actor == GetWorld()->GetFirstPlayerController()->GetPawn();
		if (is_simulation_responsible) {
			/*tmpvec.Z = 0;
			GetWorld()->GetTimerManager().SetTimerForNextTick(FTimerDelegate::CreateLambda([&, tmpvec]() {
				}));*/
		}
	}
	character_state = ECharacterState::Ragdoll;
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	main_anim_instance->Montage_StopGroupByName(0.0f, "DefaultGroup");
}

/// <summary>
/// 모든 APlayerController를 순회해 가장 가까이 있는 플레이어의 폰을 출력 ( 서버에서 실행하기 위함 )
/// </summary>
/// <param name="closest_player">가장 가까이있는 플레이어 폰</param>
void ABaseCharacter::findClosestPlayer_Implementation(AActor*& closest_player) {
	float min_distance = INFINITY;
	if (network_owner_type == ENetworkOwnerType::OwnedPlayer) {
		closest_player = this;
		return;
	}
	for (const auto& entity : TActorRange<APlayerController>(GetWorld())) {
		float tmp_dist = FVector::Distance(entity->GetPawn()->GetActorLocation(), GetActorLocation());
		if (tmp_dist < min_distance) {
			min_distance = tmp_dist;
			closest_player = entity->GetPawn();
		}
	}
}

/// <summary>
/// 현재 액터의 네트워크 소유 관계를 UEnum 타입으로 출력
/// </summary>
/// <param name="output">액터의 네트워크 소유 관계</param>
void ABaseCharacter::getNetworkOwnerType(/*out*/ ENetworkOwnerType& output) {
	//if (HasAuthority()) {
	//	if (UKismetSystemLibrary::IsDedicatedServer(this)) {
	//		// 데디케이티드 서버일 때
	//		output = ENetworkOwnerType::DedicatedServer;
	//	}else{
	//		if (IsLocallyControlled()) {
	//			// 리슨 서버 소유 액터 or 스탠드얼론일 때
	//			if (IsPlayerControlled()) {
	//				// 리슨 서버로 실행중인 플레이어일때
	//				output = ENetworkOwnerType::ListenServerPlayer;
	//			}
	//			else {
	//				//
	//				output = ENetworkOwnerType::ServerOwned;
	//			}
	//		}
	//		else {
	//			// 리슨 서버의 비소유 액터일 경우
	//			output = ENetworkOwnerType::ListenServerNotOwned;
	//		}
	//	}
	//}
	//else {
	//	if (IsLocallyControlled()) {
	//		// 클라이언트 소유 액터인 경우
	//		output = ENetworkOwnerType::ClientOwned;
	//	}
	//	else {
	//		// 클라이언트 비소유 액터인 경우
	//		output = ENetworkOwnerType::ClientNotOwned;
	//	}
	//}

	if (IsPlayerControlled()) {
		if (IsLocallyControlled()) {
			output = ENetworkOwnerType::OwnedPlayer;
		}
		else {
			output = ENetworkOwnerType::RemotePlayer;
		}
	}
	else {
		if (IsLocallyControlled()) {
			output = ENetworkOwnerType::OwnedAI;
		}
		else {
			output = ENetworkOwnerType::RemoteAI;
		}
	}
}

/// <summary>
/// 캡슐 컴포넌트의 OnComponentHit 델리게이트에 바인딩 될 함수
/// 에어본 상태에서 블로킹 히트 발생시 래그돌로 전환
/// </summary>
/// <param name="HitComp"></param>
/// <param name="OtherActor"></param>
/// <param name="OtherComp"></param>
/// <param name="NormalImpulse"></param>
/// <param name="Hit"></param>
void ABaseCharacter::onCapsuleComponentHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit){
	if (HasAuthority()) {
		if (character_state == ECharacterState::Airbone && OtherActor != nullptr && OtherActor->IsA(ABaseCharacter::StaticClass()) == false) {
			GetWorld()->GetTimerManager().SetTimerForNextTick(FTimerDelegate::CreateLambda([&]() {
				setCharacterState(ECharacterState::Ragdoll);
				}));
		}
	}
}

/// <summary>
/// 무기 컴포넌트에 동적 바인딩 될 함수
/// </summary>
/// <param name="OverlappedComp"></param>
/// <param name="OtherActor"></param>
/// <param name="OtherComp"></param>
/// <param name="OtherBodyIndex"></param>
/// <param name="bFromSweep"></param>
/// <param name="SweepResult"></param>
void ABaseCharacter::onWeaponBeginOverlap_Implementation(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	//if (OtherComp->IsA<USkeletalMeshComponent>()) {
	//	//UPhysicsAsset asd = Cast<USkeletalMeshComponent>(OtherComp)->GetPhysicsAsset()->physics;
	//	FName tmp = Cast<USkeletalMeshComponent>(OtherComp)->GetBoneName(OtherBodyIndex);
	//	if (tmp.ToString() == "None" && tmp.ToString().Contains("ik"))
	//		return;
	//	if (OtherBodyIndex <= 1)
	//		return;
	//	//if(Cast<USkeletalMeshComponent>(OtherActor)->getbones)
	//	Cast<USkeletalMeshComponent>(OtherComp)->SetAllBodiesBelowSimulatePhysics(tmp, true);
	//	Cast<USkeletalMeshComponent>(OtherComp)->SetAllBodiesBelowPhysicsBlendWeight(tmp, 0.5f);
	//	OtherComp->AddImpulse(FVector::OneVector * 100, tmp);
	//	FTimerHandle timer_handle;
	//	GetWorld()->GetTimerManager().SetTimer(timer_handle, FTimerDelegate::CreateLambda([&, OtherComp]() {
	//		//Cast<USkeletalMeshComponent>(OtherComp)->SetAllBodiesBelowPhysicsBlendWeight(tmp, 0);
	//		Cast<USkeletalMeshComponent>(OtherComp)->SetSimulatePhysics(false);
	//		}), 1, false);
	//	UKismetSystemLibrary::PrintString(this, tmp.ToString());
	//	UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("%d"), OtherBodyIndex));
	//	attackEvent(OtherActor, tmp);
	//}
	//else {
	//	return;
	//}
	//attackEvent(OtherActor, "spine_01");
}


/// <summary>
/// 클라이언트의 로테이션을 서버로 전송해서 동기화
/// </summary>
/// <param name="__target_rotation"></param>
void ABaseCharacter::CtoS_setRotation_Implementation(FRotator __target_rotation) {
	SetActorRotation(__target_rotation.Quaternion(), ETeleportType::TeleportPhysics);
}

/// <summary>
/// 에어본 상태에서 다음 프레임에 벽이나 바닥면과 충돌할지 체크
/// 충돌하면 래그돌로 전환
/// 충돌은 라인 트레이스로 캐릭터의 하단과 에어본 운동 방향을 예측 검사해서 다음 프레임에 충돌이 일어날 지 예측 검사
/// </summary>
/// <param name="__velocity"> 예측에 쓰일 벨로시티</param>
/// <param name="__hitnormal"></param>
/// <param name="__hit_location"></param>
/// <returns></returns>
bool ABaseCharacter::airbone_HitChk(FVector __velocity, FVector& __hitnormal, FVector& __hit_location) {
	FHitResult trace_result(ForceInit);
	FVector location = GetActorLocation();
	float capsule_half_height = GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
	FVector trace_start = UKismetMathLibrary::MakeVector(location.X, location.Y, location.Z + capsule_half_height + 10);
	FVector trace_end = UKismetMathLibrary::MakeVector(location.X, location.Y, location.Z - capsule_half_height + 20);
	TArray<AActor*> tmp;
	/* 캐릭터 하단을 라인 트레이스로 검사해 지면과의 충돌 검사 */
	bool tracebool = UKismetSystemLibrary::LineTraceSingle(GetWorld(), trace_start, trace_end, ETraceTypeQuery::TraceTypeQuery2, false, tmp, EDrawDebugTrace::Type::None, trace_result, true);
	if (tracebool) {
		__hitnormal = trace_result.ImpactNormal;
		__hit_location = trace_result.ImpactPoint;
		return true;
	}
	/* 캐릭터의 운동 방향을 검사해 다음 프레임에 지면 or 벽면과의 충돌이 있는지 검사 */
	bool tracebool2 = UKismetSystemLibrary::LineTraceSingle(GetWorld(), GetMesh()->GetSocketLocation(TEXT("pelvis")), GetActorLocation() + __velocity * (d_time * 2), ETraceTypeQuery::TraceTypeQuery2, false, tmp, EDrawDebugTrace::Type::None, trace_result, true);
	if (tracebool2) {
		__hitnormal = trace_result.ImpactNormal;
		__hit_location = trace_result.ImpactPoint;
		return true;
	}
	return false;
}

void ABaseCharacter::CtoS_setCharacterState_Implementation(ECharacterState __target_character_state) {
	Multicast_setCharacterState(__target_character_state);
}

void ABaseCharacter::Multicast_setCharacterState_Implementation(ECharacterState __target_character_state) {
	setCharacterState(__target_character_state);
}

/// <summary>
/// 히트 본 덜렁거리는 피지컬 애니메이션 적용 프로세스 ( 매 틱 실행 )
/// TMap 타입의 hit_bone_physics_weight_map 에 원소가 있다면 해당 원소의 밸류를 매 틱 감소시키고 해당 본의 PhysicsBlendWeight 를 밸류 값으로 업데이트
/// 원소의 밸류가 0이하가 되면 해당 본의 피직스 시뮬레이션을 종료하고 hit_bone_physics_weight_map 에서 제거
/// 상위 본이 피직스 시뮬레이션이면 hit_bone_physics_weight_map 에서 현재 본을 제거 ( 충돌과 꼬임 방지 )
/// </summary>
void ABaseCharacter::hitBonePhysicalReactionProcess_Implementation() {
	if (hit_bone_physics_weight_map.Num() == 0)
		return;
	// 캐릭터 스테이트가 Walk_and_Jump가 아니면 맵을 empty로 초기화 하고 모든 본의 weight를 초기화 한 뒤 함수 종료
	if (character_state != ECharacterState::Walk_and_Jump) {
		hit_bone_physics_weight_map.Empty();
		GetMesh()->SetAllBodiesPhysicsBlendWeight(1);
		return;
	}
	// 맵 순회 하며 웨이트 값 감소하고 0이면 삭제
	TArray<FName> removetarget;
	for (auto i : hit_bone_physics_weight_map) {
		i.Value -= d_time;
		hit_bone_physics_weight_map[i.Key] -= d_time * 1.5f;
		if (GetMesh()->IsSimulatingPhysics(GetMesh()->GetParentBone(i.Key))) {
			/*상위 본에서 피직스 이미 시뮬레이션 중이면 맵에서 제거하고 웨이트 조절 스킵*/
			removetarget.Add(i.Key);
			continue;
		}
		if (i.Value <= 0) {
			if (hit_bone_physics_weight_map.Num() == 1) {
				GetMesh()->SetSimulatePhysics(false);
			}
			else {
				GetMesh()->SetAllBodiesBelowSimulatePhysics(i.Key, false);
			}
			removetarget.Add(i.Key);
		}
		else {
			GetMesh()->SetAllBodiesBelowPhysicsBlendWeight(i.Key, hit_bone_physics_weight_map[i.Key]);
		}
	}
	for (auto i : removetarget) {
		hit_bone_physics_weight_map.Remove(i);
	}
}

/* ->>> 아이템 이펙트 관련 */

void ABaseCharacter::ItemEffect_onRegistration_Implementation(FName __item_id, int32 __inven_index)
{
	Multicast_ItemEffect_onRegistration(__item_id, __inven_index);
}

/// <summary>
/// 해당 아이템의 모든 아이템 이펙트의 onRegistration 을 발동시킴
/// </summary>
/// <param name="__item_id"></param>
/// <param name="__inven_index"> 해당 아이템의 인벤토리에서의 인덱스 </param>
void ABaseCharacter::Multicast_ItemEffect_onRegistration_Implementation(FName __item_id, int32 __inven_index)
{
	/* 게임 인스턴스에서 아이템 데이터 테이블을 검색해 item data를 찾아냄 */
	FItemData equipeditemdata = Cast<UPWOGameInstance>(GetGameInstance())->findItemData(__item_id);
	/* item data 의 item_effect_list를 순회하며 모든 ItemEffect의 클래스 디폴트 객체를 초기화 시키고 onRegistration 을 실행시킴 */
	for (FItemEffect i : equipeditemdata.item_effect_list) {
		auto item_effect_obj = i.item_effect.GetDefaultObject();
		item_effect_obj->value = i.value;
		item_effect_obj->item_id = __item_id;
		IInterface_ItemEffect::Execute_onRegistration(item_effect_obj, this, __inven_index);
	}
}
void ABaseCharacter::ItemEffect_onRemoveRegistration_Implementation(FName __item_id, int32 __inven_index)
{
	Multicast_ItemEffect_onRemoveRegistration(__item_id, __inven_index);
}
/// <summary>
/// 해당 아이템의 모든 아이템 이펙트의 onRemoveRegistration 을 발동시킴
/// </summary>
/// <param name="__item_id"></param>
/// <param name="__inven_index"></param>
void ABaseCharacter::Multicast_ItemEffect_onRemoveRegistration_Implementation(FName __item_id, int32 __inven_index)
{
	/* 게임 인스턴스에서 아이템 데이터 테이블을 검색해 item data를 찾아냄 */
	FItemData equipeditemdata = Cast<UPWOGameInstance>(GetGameInstance())->findItemData(__item_id);
	/* item data 의 item_effect_list를 순회하며 모든 ItemEffect의 클래스 디폴트 객체를 초기화 시키고 onRemoveRegistration 을 실행시킴 */
	for (FItemEffect i : equipeditemdata.item_effect_list) {
		auto item_effect_obj = i.item_effect.GetDefaultObject();
		item_effect_obj->value = i.value;
		item_effect_obj->item_id = __item_id;
		IInterface_ItemEffect::Execute_onRemoveRegistration(item_effect_obj, this, __inven_index);
	}
}

void ABaseCharacter::ItemEffect_onActivate_Implementation(FName __item_id, int32 __inven_index)
{
	Multicast_ItemEffect_onActivate(__item_id, __inven_index);
}
/// <summary>
/// 해당 아이템의 모든 아이템 이펙트의 onActivate 을 발동시킴 
/// </summary>
/// <param name="__item_id"></param>
/// <param name="__inven_index"></param>
void ABaseCharacter::Multicast_ItemEffect_onActivate_Implementation(FName __item_id, int32 __inven_index)
{
	/* 게임 인스턴스에서 아이템 데이터 테이블을 검색해 item data를 찾아냄 */
	FItemData equipeditemdata = Cast<UPWOGameInstance>(GetGameInstance())->findItemData(__item_id);
	/* item data 의 item_effect_list를 순회하며 모든 ItemEffect의 클래스 디폴트 객체를 초기화 시키고 onActivate 을 실행시킴 */
	for (FItemEffect i : equipeditemdata.item_effect_list) {
		auto item_effect_obj = i.item_effect.GetDefaultObject();
		item_effect_obj->value = i.value;
		item_effect_obj->item_id = __item_id;
		IInterface_ItemEffect::Execute_onActivate(item_effect_obj, this, __inven_index);
	}
}