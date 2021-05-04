// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacter.h"

#include "Kismet/KismetSystemLibrary.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Net/UnrealNetwork.h"
#include "Engine.h"
#include "EngineUtils.h"

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
}

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	//Make sure the mesh and animbp update after the CharacterBP
	GetMesh()->AddTickPrerequisiteActor(this);
	main_anim_instance = GetMesh()->GetAnimInstance();

}

// Called every frame
void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	d_time = DeltaTime;
	
	//바라보기에 필요한 변수 갱신
	look_Implementation();

	
	 //에어본시 피지컬 애니메이션의 관절 각 운동량 조절
	if (character_state == ECharacterState::Airbone) {
		UMeshComponent* mesh = GetMesh();
		mesh->SetAllPhysicsAngularVelocityInRadians(GetVelocity());
	}
	//래그돌 위치 동기화 시스템
	else if (character_state == ECharacterState::Ragdoll) {
		SetReplicateMovement(false);
		if (this->HasAuthority()) {
			if (IsRunningDedicatedServer() == false) {
				if (IsLocallyControlled()) {
					// 리슨 서버 소유 액터 or 스탠드얼론일 때
					if (is_simulation_owner) {
						ragdoll_ServerOnwer_Implementation();
					}
				}
				else {
					// 리슨 서버의 비소유 액터일 경우
					ragdoll_SyncLocation_Implementation();
				}
			}
		}
		else {
			// 클라이언트 소유 액터인 경우
			if (IsLocallyControlled()) {
				ragdoll_ClientOnwer_Implementation();
			}
			// AI 캐릭터의 접속이 시뮬레이션 담당 클라이언트인 경우
			else if (IsPlayerControlled() == false && is_simulation_owner == true) {
				ragdoll_ClientOnwer_Implementation();
			}
			else {
				ragdoll_SyncLocation_Implementation();
			}
		}
	}
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
	//DOREPLIFETIME(ABaseCharacter, ragdoll_server_location);
}

// 캐릭터가 바라보는 방향을 결정하는 변수 설정
void ABaseCharacter::look_Implementation()
{
	FRotator tmp_rotator1 = UKismetMathLibrary::MakeRotator(0, look_pitch, look_yaw);
	FRotator tmp_rotator2 = UKismetMathLibrary::NormalizedDeltaRotator(R_look_rotation, AActor::K2_GetActorRotation());
	FRotator interp_tmp = UKismetMathLibrary::RInterpTo(tmp_rotator1, tmp_rotator2, d_time, 15.000000);
	look_pitch = UKismetMathLibrary::ClampAngle(interp_tmp.Pitch, -90.000000, 90.000000);
	look_yaw = UKismetMathLibrary::ClampAngle(interp_tmp.Yaw, -90.000000, 90.000000);
	//UKismetSystemLibrary::PrintString(this, TEXT("한글"));
	return;
}

// 래그돌 상태에서 엉덩이 아래로 라인 트레이스 하여 래그돌이 지면에 있을 시 액터를 지면 바로 위로 붙임
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

// 클라이언트 소유 액터 : 래그돌의 위치로 액터를 이동시키고 서버에서 위치를 리플리케이트 할 수 있도록 서버로 전달
void ABaseCharacter::ragdoll_ClientOnwer_Implementation() {
	FVector sock_location = GetMesh()->GetSocketLocation("pelvis");
	ragdoll_server_location = sock_location;
	stickToTheGround(ragdoll_server_location);
	CtoS_targetLocation(this, ragdoll_server_location);
}

// 서버 소유 액터 or 스탠드얼론 : 래그돌의 위치로 액터를 이동
void ABaseCharacter::ragdoll_ServerOnwer_Implementation() {
	FVector sock_location = GetMesh()->GetSocketLocation("pelvis");
	ragdoll_server_location = sock_location;
	stickToTheGround(ragdoll_server_location);
}

// 서버 : 클라이언트 소유 액터에게 위치를 전달받으면 변수를 업데이트해서 리플리케이트
void ABaseCharacter::CtoS_targetLocation_Implementation(ABaseCharacter* target_actor, FVector target_location) {
	target_actor->ragdoll_server_location = target_location;
	target_actor->stickToTheGround(target_location);
}

// 래그돌 위치 동기화
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

// 캐릭터 에어본시 설정 초기화 및 에어본 애니메이션 재생
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

// 래그돌 상태에서 일어서는 설정 초기화 및 애니메이션 재생
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

// 캐릭터 상태에 변화에 따른 세팅값 결정
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
			airboneStart_Implementation();
			break;
		case ECharacterState::Ragdoll:
			is_on_sprint = false;
			is_on_action = false;
			break;
		default:
			break;
		}
	}
}

void ABaseCharacter::ragdoll_SetOnServer_Implementation() {
	
}

void ABaseCharacter::findClosestPlayer_Implementation(AActor*& closest_player) {
	float min_distance = INFINITY;
	
	for (const auto& entity : TActorRange<APlayerController>(GetWorld())) {
		float tmp_dist = FVector::Distance(entity->GetPawn()->GetActorLocation(), GetActorLocation());
		if (tmp_dist < min_distance) {
			min_distance = tmp_dist;
			closest_player = entity;
		}
	}
}

//void ABaseCharacter::rotateActorTimeline(FRotator target_rotation, float time)
//{
//
//}