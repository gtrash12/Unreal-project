﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacter.h"

#include "Kismet/KismetSystemLibrary.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Net/UnrealNetwork.h"
#include "Engine.h"
#include "EngineUtils.h"
#include "Public/Interface_PlayerController.h"

// Sets default values
ABaseCharacter::ABaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//리플리케이션 설정
	bReplicates = true;

	ragdoll_physics_handle = CreateDefaultSubobject<UPhysicsHandleComponent>(TEXT("PhysicsHandle"));
	ragdoll_physics_handle->CreationMethod = EComponentCreationMethod::Native;
	ragdoll_physics_handle->InterpolationSpeed = 10000000272564224.000000f;
	ragdoll_physics_handle->bAutoActivate = false;

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
	static ConstructorHelpers::FObjectFinder<USoundBase> SQ_Hit(TEXT("SoundCue'/Game/Blueprint/SQ_Hit.SQ_Hit'"));
	if (SQ_Hit.Succeeded())
		sq_hit = SQ_Hit.Object;

}

// 컴포넌트 초기화 후 함수 바인딩 등
void ABaseCharacter::PostInitializeComponents() {

	Super::PostInitializeComponents();

	// 캡슐 컴포넌트의 히트 델리게이트에 함수 등록
	GetCapsuleComponent()->OnComponentHit.AddDynamic(this, &ABaseCharacter::onCapsuleComponentHit);

	//Make sure the mesh and animbp update after the CharacterBP
	GetMesh()->AddTickPrerequisiteActor(this);
	main_anim_instance = GetMesh()->GetAnimInstance();
	getNetworkOwnerType(network_owner_type);
}

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	d_time = DeltaTime;
	
	//바라보기에 필요한 변수 갱신
	/*IInterface_BaseCharacter * basecharacter_interface = Cast<IInterface_BaseCharacter>(this);
	if (basecharacter_interface) {
		basecharacter_interface->look();
	}*/
	setLookRotation();
	current_velocty = GetVelocity();
	
	 //에어본시 피지컬 애니메이션의 관절 각 운동량 조절
	if (character_state == ECharacterState::Airbone) {
		UMeshComponent* mesh = GetMesh();
		mesh->SetAllPhysicsAngularVelocityInRadians(GetVelocity());
	}
	/*
	//래그돌 위치 동기화 시스템
	else if (character_state == ECharacterState::Ragdoll) {
		SetReplicateMovement(false);

		switch (network_owner_type)
		{
		case ENetworkOwnerType::ServerOwned:
			if (is_simulation_responsible) {
				ragdoll_ServerOnwer_Implementation();
			}
			break;
		case ENetworkOwnerType::ListenServerNotOwned:
			ragdoll_SyncLocation_Implementation();
			break;
		case ENetworkOwnerType::DedicatedServer:
			break;
		case ENetworkOwnerType::ClientOwned:
			ragdoll_ClientOnwer_Implementation();
			break;
		case ENetworkOwnerType::ClientNotOwned:
			ragdoll_SyncLocation_Implementation();
			break;
		default:
			break;
		}
	}
	*/

	knock_BackProcess();
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
	DOREPLIFETIME(ABaseCharacter, knock_back_count);
	DOREPLIFETIME(ABaseCharacter, simulation_responsible_actor);
}

/// --> 인터페이스 함수 정의

/// <summary>
/// 캐릭터가 바라보는 방향을 결정
/// </summary>
void ABaseCharacter::setLookRotation_Implementation()
{
	FRotator tmp_rotator1 = UKismetMathLibrary::MakeRotator(0.0f, look_pitch, look_yaw);
	FRotator tmp_rotator2 = UKismetMathLibrary::NormalizedDeltaRotator(R_look_rotation, AActor::GetActorRotation());
	FRotator interp_tmp = UKismetMathLibrary::RInterpTo(tmp_rotator1, tmp_rotator2, d_time, 15.000000);
	look_pitch = UKismetMathLibrary::ClampAngle(interp_tmp.Pitch, -90.000000, 90.000000);
	look_yaw = UKismetMathLibrary::ClampAngle(interp_tmp.Yaw, -90.000000, 90.000000);
}

/// <summary>
/// 피격 액터가 데미지 프로세스를 처리하도록 전달
/// 체력 연산 후 멀티캐스트 함수 실행
/// </summary>
/// <param name="target_damage_data">전달할 데미지 데이터</param>
/// <param name="damage_causor">데미지를 입힌 액터</param>
void ABaseCharacter::applyDamage_Implementation(FdamageData __target_damage_data, AActor* __damage_causor)
{
	hp -= __target_damage_data.base_damage + __target_damage_data.base_damage_percent * Cast<ABaseCharacter>(__damage_causor)->base_power;
	applyDamage_Multicast(__target_damage_data, __damage_causor);
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
/// 공격이 적에게 맞았을 때 피격액터에게 데미지 이벤트를 전달
/// owned 액터에서만 실행되도록 구현하여 네트워크 상황에서 한 번만 실행되도록 보장
/// </summary>
/// <param name="hit_actor">피격 액터</param>
void ABaseCharacter::attackEvent_Implementation(AActor* __hit_actor) {
	if (network_owner_type == ENetworkOwnerType::OwnedPlayer || network_owner_type == ENetworkOwnerType::OwnedAI) {
		if (hit_actors_list.Contains(__hit_actor) == false) {
			IInterface_PlayerController* controller_interface = Cast<IInterface_PlayerController>(__hit_actor);
			if (controller_interface) {
				controller_interface->CtoS_applyDamage(__hit_actor, damage_data, this);
			}
			hit_actors_list.Add(__hit_actor);
		}
	}
}

/// <summary>
/// is_on_action을 초기화 시켜 캐릭터가 액션을 수행할 수 있게 하고, 공격 시퀀스를 초기화
/// </summary>
void ABaseCharacter::resetNextAttack_Implementation() {
	next_attack_montage = normal_attack_montage;
	is_on_action = false;
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
void ABaseCharacter::getAttackTraceChannel_Implementation(/*out*/ TEnumAsByte<ETraceTypeQuery>& __attack_trace_channel) {
	__attack_trace_channel = ETraceTypeQuery::TraceTypeQuery4;
}

/// <summary>
/// 캐릭터 보간 회전
/// </summary>
/// <param name="__target_rotation">회전 목표 로테이션</param>
/// <param name="__delta_time">델타 타임</param>
/// <param name="__speed">보간 속도</param>
void ABaseCharacter::rotateActorInterp_Implementation(FRotator __target_rotation, float __delta_time, float __speed) {
	FRotator interp_rotator = UKismetMathLibrary::RInterpTo(GetActorRotation(),__target_rotation,__delta_time,__speed);
	//GetMovementComponent().repli 
	if (HasAuthority()) {
		if (UKismetSystemLibrary::IsDedicatedServer(this) == false) {
			SetActorRotation(interp_rotator.Quaternion(), ETeleportType::TeleportPhysics);
		}
	}
	else {
		CtoS_setRotation(interp_rotator);
		SetActorRotation(interp_rotator.Quaternion(), ETeleportType::TeleportPhysics);
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
	__output_is_on_sprint = is_on_sprint;
}

/// <summary>
/// current_velocity 변수 반환
/// : 무브먼트 벨로시티에 넉백 벨로시티등을 다 합친 벨로시티의 합
/// </summary>
/// <param name="__output_current_velocity">출력 current_velocity</param>
void ABaseCharacter::getCurrentVelocity_Implementation(FVector& __output_current_velocity) {
	__output_current_velocity = current_velocty;
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

// <-- 인터페이스 함수 정의 끝



// --> 클래스 멤버 함수 정의


/// <summary>
/// 서버에서 체력연산 후 멀티캐스트로 넉백 등을 처리
/// </summary>
/// <param name="target_damage_data">전달할 데미지 데이터</param>
/// <param name="damage_causor">데미지를 입힌 액터</param>
void ABaseCharacter::applyDamage_Multicast_Implementation(FdamageData target_damage_data, AActor* damage_causor)
{
	// 넉백 벡터를 넉백타입과 방향에 맞게 회전
	FVector rotated_vector;
	if (target_damage_data.knock_back_type == EKnockBackType::Directional)
		rotated_vector = UKismetMathLibrary::Quat_RotateVector(GetActorRotation().Quaternion(), target_damage_data.knock_back);
	else {
		FQuat rotate_quat = UKismetMathLibrary::FindLookAtRotation(damage_causor->GetActorLocation(), GetActorLocation()).Quaternion();
		rotated_vector = UKismetMathLibrary::Quat_RotateVector(rotate_quat, target_damage_data.knock_back);
	}

	UAnimMontage* hit_anim = nullptr;
	selectHitAnimation(rotated_vector, hit_anim);
	animation_Sound_Multicast(hit_anim, sq_hit);

	if (target_damage_data.target_control == ETargetControlType::None) {
		knock_Back(rotated_vector);
	}
	else if (target_damage_data.target_control == ETargetControlType::Ragdoll) {
		LaunchCharacter(UKismetMathLibrary::MakeVector(rotated_vector.X * 2, rotated_vector.Y * 2, rotated_vector.Z), true, true);
		FTimerHandle timer_handle;
		GetWorld()->GetTimerManager().SetTimer(timer_handle, FTimerDelegate::CreateLambda([&]() {
			setCharacterState(ECharacterState::Airbone);
			}), 0.1f, false);
	}
}

/// <summary>
/// 서버에서 애니메이션과 사운드를 멀티캐스트로 실행
/// </summary>
/// <param name="anim">애니메이션</param>
/// <param name="sound">사운드</param>
void ABaseCharacter::animation_Sound_Multicast_Implementation(UAnimMontage* anim, USoundBase* sound) {
	PlayAnimMontage(anim);
	UGameplayStatics::SpawnSoundAtLocation(this, sound, GetActorLocation());
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

/// <summary>
/// 넉백 연산 수행 (매 틱 실행)
/// </summary>
/// <param name="velocity">넉백 벨로시티</param>
void ABaseCharacter::knock_BackProcess_Implementation() {
	if (knock_back_count_end > 0 && knock_back_count < knock_back_count_end) {
		knock_back_count = UKismetMathLibrary::Min(knock_back_count_end, knock_back_count + d_time);
		float ease_alpha = knock_back_count / knock_back_count_end;
		float ease_res = UKismetMathLibrary::Ease(1.0f, 0.0f,ease_alpha,EEasingFunc::EaseOut);
		FVector knock_back_delta = UKismetMathLibrary::Multiply_VectorFloat(knock_back_velocity, ease_res);
		GetMovementComponent()->MoveUpdatedComponent(knock_back_delta, GetActorRotation(), true);
		UKismetMathLibrary::Add_VectorVector(current_velocty, knock_back_delta);
	}
}

/// <summary>
/// 넉백프로세스에서 사용되는 변수 초기화
/// </summary>
void ABaseCharacter::knock_Back_Implementation(FVector velocity) {
	float vel_length = velocity.Size();
	knock_back_count = 0;
	knock_back_velocity = velocity;
	knock_back_count_end = vel_length / 300.0f;
	is_on_action = true;
	if (velocity.Z > 0) {
		LaunchCharacter(UKismetMathLibrary::MakeVector(0.0f,0.0f,velocity.Z),false,false);
	}
}

/// <summary>
/// 래그돌 상태에서 엉덩이 아래로 라인 트레이스 하여 래그돌이 지면에 있을 시 액터를 지면 바로 위로 붙임
/// </summary>
/// <param name="location">액터의 위치</param>
void ABaseCharacter::stickToTheGround(FVector location) {
	float capsule_half_height = this->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
	FVector trace_end = UKismetMathLibrary::MakeVector(location.X, location.Y, location.Z - capsule_half_height);
	FHitResult trace_result(ForceInit);
	FCollisionQueryParams collision_params;
	GetWorld()->LineTraceSingleByChannel(trace_result, location, trace_end, ECC_Visibility, collision_params);

	// 라인트레이스 결과에 따른 액터의 방향과 위치 설정
	if (trace_result.IsValidBlockingHit()) {
		if (is_ragdoll_on_the_ground == false) {
			is_ragdoll_on_the_ground = true;
		}
		// 엉덩이가 위를 향하고 있으면 액터의 방향을 180도 회전 ( 일어서는 애니메이션을 위함 )
		FRotator sock_rotator = GetMesh()->GetSocketRotation("pelvis");
		is_ragdoll_face_up = sock_rotator.Roll < 0.0f;
		sock_rotator.Roll = 0;
		sock_rotator.Pitch = 0;
		if (is_ragdoll_face_up) 
			sock_rotator.Yaw -= 180.0f;
		FVector landed_location = UKismetMathLibrary::MakeVector(location.X, location.Y, trace_result.Location.Z + capsule_half_height);
		SetActorLocationAndRotation(landed_location,sock_rotator);
	}
	else {
		is_ragdoll_on_the_ground = false;
		SetActorLocation(location);
	}
}

/// <summary>
/// 클라이언트 소유 액터 : 래그돌의 위치로 액터를 이동시키고 서버에서 위치를 리플리케이트 할 수 있도록 서버로 전달
/// </summary>
void ABaseCharacter::ragdoll_ClientOnwer_Implementation() {
	FVector sock_location = GetMesh()->GetSocketLocation("pelvis");
	ragdoll_server_location = sock_location;
	stickToTheGround(ragdoll_server_location);
	CtoS_targetLocation(this, ragdoll_server_location);
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
}

/// <summary>
/// 래그돌 위치 동기화
/// 피직스 핸들을 이용해 서버의 타겟 위치로 래그돌을 옮기는 역할 수행
/// </summary>
void ABaseCharacter::ragdoll_SyncLocation_Implementation() {

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
	ragdoll_physics_handle->SetTargetLocation(ragdoll_server_location);
	float ease_alpha = UKismetMathLibrary::Min(1.0f, replication_delay_count / last_replication_delay);
	FVector predicted_location = UKismetMathLibrary::VEase(prev_ragdoll_server_location, ragdoll_server_location, ease_alpha, EEasingFunc::Linear);
	stickToTheGround(predicted_location);
	last_ragdoll_server_location = ragdoll_server_location;
	
}

/// <summary>
/// 캐릭터 에어본시 설정 초기화 및 에어본 애니메이션 재생
/// </summary>
void ABaseCharacter::airboneStart_Implementation() {
	USkeletalMeshComponent* mesh = GetMesh();

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	mesh->SetAllBodiesBelowSimulatePhysics("pelvis", true, false);
	//CastChecked<UAnimMontage>(CastChecked<UDynamicClass>(ABP_BaseCharacter_C__pf503790866::StaticClass())->UsedAssets[6]
	FVector vel = GetVelocity();
	FVector forward = GetActorForwardVector();
	float speed_rate = vel.Size()/2000;
	if (FVector::DotProduct(vel, forward) >= 0) {
		PlayAnimMontage(airbone_b_anim, speed_rate);
	}
	else {
		PlayAnimMontage(airbone_b_anim, -speed_rate);
	}
	
}

/// <summary>
/// 래그돌 상태에서 일어서는 설정 초기화 및 애니메이션 재생
/// </summary>
void ABaseCharacter::ragdollGetUp_Implementation() {
	// 무브먼트 리플리케이션 재 실행 후 현재 래그돌 포즈 스냅샷으로 저장
	SetReplicateMovement(true);
	if (main_anim_instance->IsValidLowLevel()) {
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
	mesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	mesh->SetSimulatePhysics(false);
	is_on_action = false;

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
			GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
			character_state = ECharacterState::Walk_and_Jump;
			break;
		case ECharacterState::Airbone:
			is_on_sprint = false;
			is_on_action = false;
			airboneStart();
			break;
		case ECharacterState::Ragdoll:
			is_on_sprint = false;
			is_on_action = false;
			ragdoll_SetOnServer();
			break;
		default:
			break;
		}
	}
}

/// <summary>
/// 서버에서 래그돌 세팅 수행
/// </summary>
void ABaseCharacter::ragdoll_SetOnServer_Implementation() {
	ragdoll_server_location = GetMesh()->GetSocketLocation("pelvis");
	findClosestPlayer(simulation_responsible_actor);
	ragdoll_SetMultiCast(simulation_responsible_actor);
}

/// <summary>
/// 멀티캐스트로 래그돌 세팅 동기화
/// </summary>
void ABaseCharacter::ragdoll_SetMultiCast_Implementation(AActor* responsible_actor) {
	if (UKismetSystemLibrary::IsDedicatedServer(this) == false) {
		prev_ragdoll_server_location = ragdoll_server_location;
		last_ragdoll_server_location = ragdoll_server_location;
		replication_delay_count = 0.0f;
		is_ragdoll_on_the_ground = false;
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		GetMesh()->SetAllBodiesBelowSimulatePhysics("pelvis", true, true);

		is_simulation_responsible = simulation_responsible_actor == GetWorld()->GetFirstPlayerController()->GetPawn();
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
		if (character_state == ECharacterState::Airbone)
			ragdoll_SetOnServer();
		UKismetSystemLibrary::PrintString(this,TEXT("아아"));
	}
}



void ABaseCharacter::CtoS_setRotation_Implementation(FRotator __target_rotation) {
	SetActorRotation(__target_rotation.Quaternion(), ETeleportType::TeleportPhysics);
}

//void ABaseCharacter::rotateActorTimeline(FRotator target_rotation, float time)
//{
//
//}