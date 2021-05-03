// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacter.h"

#include "Kismet/KismetSystemLibrary.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Net/UnrealNetwork.h"
#include "Engine.h"

// Sets default values
ABaseCharacter::ABaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//리플리케이션 설정
	bReplicates = true;

	look_yaw = 0.0f;
	look_pitch = 0.0f;
	R_look_rotation = FRotator(0.000000, 0.000000, 0.000000);

	ragdoll_physics_handle = CreateDefaultSubobject<UPhysicsHandleComponent>(TEXT("PhysicsHandle"));
	ragdoll_physics_handle->CreationMethod = EComponentCreationMethod::Native;
	ragdoll_physics_handle->InterpolationSpeed = 10000000272564224.000000f;
	ragdoll_physics_handle->bAutoActivate = false;
}

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	//Make sure the mesh and animbp update after the CharacterBP
	//ACharacter::GetMesh()->AddTickPrerequisiteActor(this);


}

// Called every frame
void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	d_time = DeltaTime;
	
	//바라보기에 필요한 변수 갱신
	look_Implementation();

	//래그돌 위치 동기화 시스템
	if (character_state == CharacterState::Ragdoll) {
		this->SetReplicateMovement(false);
		if (this->HasAuthority()) {
			if (IsRunningDedicatedServer() == false) {
				if (IsLocallyControlled()) {
					// 리슨 서버 소유 액터 or 스탠드얼론일 때
					if (is_simulation_owner) {
						ragdoll_ServerOnwer();
					}
				}
				else {
					// 리슨 서버의 비소유 액터일 경우
					ragdoll_SyncLocation();
				}
			}
		}
		else {
			// 클라이언트 소유 액터인 경우
			if (IsLocallyControlled()) {
				ragdoll_ClientOnwer();
			}
			// AI 캐릭터의 접속이 시뮬레이션 담당 클라이언트인 경우
			else if (IsPlayerControlled() == false && is_simulation_owner == true) {
				ragdoll_ClientOnwer();
			}
			else {
				ragdoll_SyncLocation();
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
	DOREPLIFETIME(ABaseCharacter, ragdoll_server_location);
}

// 캐릭터가 바라보는 방향을 결정하는 변수 설정
void ABaseCharacter::look_Implementation()
{
	FRotator tmp_rotator1 = UKismetMathLibrary::MakeRotator(0, look_pitch, look_yaw);
	FRotator tmp_rotator2 = UKismetMathLibrary::NormalizedDeltaRotator(R_look_rotation, AActor::K2_GetActorRotation());
	FRotator interp_tmp = UKismetMathLibrary::RInterpTo(tmp_rotator1, tmp_rotator2, d_time, 15.000000);
	look_pitch = UKismetMathLibrary::ClampAngle(interp_tmp.Pitch, -90.000000, 90.000000);
	look_yaw = UKismetMathLibrary::ClampAngle(interp_tmp.Yaw, -90.000000, 90.000000);
	UKismetSystemLibrary::PrintString(this, TEXT("한글"));
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
void ABaseCharacter::ragdoll_ClientOnwer() {
	FVector sock_location = GetMesh()->GetSocketLocation("pelvis");
	ragdoll_server_location = sock_location;
	stickToTheGround(ragdoll_server_location);
	CtoS_targetLocation(this, ragdoll_server_location);
}

// 서버 소유 액터 or 스탠드얼론 : 래그돌의 위치로 액터를 이동
void ABaseCharacter::ragdoll_ServerOnwer() {
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
void ABaseCharacter::ragdoll_SyncLocation() {

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

//void ABaseCharacter::rotateActorTimeline(FRotator target_rotation, float time)
//{
//
//}