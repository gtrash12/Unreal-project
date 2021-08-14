# Unreal-project
언리얼엔진4로 개발한 3D 온라인 액션 RPG

# 목차
- [영상](#영상)
- 래그돌 시스템
  - [네트워크 래그돌 위치 동기화 시스템](#네트워크-래그돌-위치-동기화-시스템)
- [전투 시스템](#전투-시스템)
  - [액션 시스템](#액션-시스템)
  - [데미지 시스템](#데미지-시스템)
  - [넉백 애니메이션 and 피격 애니메이션 and 피지컬 애니메이션](#넉백-애니메이션-and-피격-애니메이션-and-피지컬-애니메이션)
  - [방어 시스템](#방어-시스템)
- [아이템 데이터 구조와 작동 방식](#아이템-데이터-구조와-작동-방식)
  - [아이템 효과 시스템](#아이템-효과-시스템)
- [인벤토리 시스템](#인벤토리-시스템)
- [락온 타게팅 시스템](#락온타게팅-시스템)
- [머테리얼 and 이펙트](#머테리얼-and-이펙트)
  - [피 이펙트](#피-이펙트)
  - [히트 궤적 이펙트](#히트-궤적-이펙트)
  - [화면 흔들림](#화면-흔들림)
  - [에너지 필드](#에너지-필드)
- [애니메이션 시스템](#애니메이션-시스템)
  - [이동 방향 기반 캐릭터 rotation 회전 시스템](#이동-방향-기반-캐릭터-rotation-회전-시스템)
- [카메라 위치 조정 시스템](#카메라-위치-조정)
- [데모 레벨 캡처](#데모-레벨)
- [주요 cpp 파일 링크](#주요-파일)
- [조작법](#조작법)

# 영상
- 아래 이미지 클릭시 유튜브 영상 링크

[![썸네일](https://user-images.githubusercontent.com/12960463/121377323-34de2200-c97d-11eb-9992-825a908c26d4.jpg)](https://www.youtube.com/watch?v=25rv31yKPcY&list=PLAGMcKKJuvziGTJumbg1EP94LcSFXKdgT&index=1)

# 핵심 기능
# 네트워크 래그돌 위치 동기화 시스템
![마영전 래그돌 위치 동기화 안됨](https://user-images.githubusercontent.com/12960463/117229348-d2fa3c00-ae55-11eb-8c94-daab355dfa7a.gif)

마비노기 영웅전에서는 래그돌 위치 동기화가 정확히 이루어지지 않아 위와같이 래그돌 상태에서 부활을 통해 캐릭터가 살아날 시 서버의 위치로 클라이언트의 위치가 조정되는 움직임을 보입니다.

![래그돌 위치 동기화 1(sm)](https://user-images.githubusercontent.com/12960463/117230797-c88d7180-ae58-11eb-8803-ff39f4a5fec0.gif)

저는 이러한 현상을 막기위해 네트워크 래그돌 위치 동기화 시스템을 개발했습니다.
화면에 보이는 두 플레이어 모두 클라이언트입니다.
지뢰를 밟고 에어본되어 바닥에 닿는 순간 래그돌로 전환되어 구르게 됩니다.
spacebar를 누르면 래그돌 상태에서 일어서며 래그돌이 해제되는데 이 때 래그돌이 이미 서버의 래그돌 위치와 동기화 되어 있기 때문에 위치 조정이 일어나지 않습니다.

![래그돌 위치 동기화 2(sm)](https://user-images.githubusercontent.com/12960463/117230801-cb886200-ae58-11eb-8a8d-578d5977c9f0.gif)

먼 거리를 날아가 래그돌 상태에서 지면을 꽤 먼거리를 굴러서 서버와 클라이언트간 래그돌 위치의 차이가 꽤 벌어질 만 하지만 일어설 때 조금의 조정의 움직임도 보이지 않습니다.


위의 래그돌 동기화 시스템은 각 클라이언트가 자기 자신이 소유한 액터의 래그돌 연산만 전담하는 방식으로 구현되었습니다.
- step 0 :
캐릭터 무브먼트 리플리케이션 종료
(위치 조정에 의한 버벅거림 방지)
- step 1 :
액터 소유 클라이언트에서 래그돌 연산 수행
- step 2 :
액터 소유 클라이언트에서 래그돌의 위치를 RPC를 통해 서버로 전달
- step 3 :
서버는 해당 위치로 액터를 옮기고 위치 변수를 리플리케이트
- step 4 :
타 클라이언트에서는 피직스 핸들로 리모트 플레이어의 액터를 서버에서 리플리케이트된 타겟 래그돌 위치로 옮김
- step 5 :
타겟 위치가 리플리케이트 되는 주기가 클라이언트의 틱 주기보다 느리기 때문에 다음 리플리케이트 까지 액터의 위치를 보간이동


위는 플레이어 캐릭터의 경우이고
AI 캐릭터는 서버 소유의 액터이기 때문에 래그돌 전환 시 가장 가까이 있는 플레이어를 찾아 해당 플레이어를 소유한 클라이언트에서 래그돌 연산을 수행하게됩니다.

![몬스터 다운,기상(sm)](https://user-images.githubusercontent.com/12960463/118161935-2292c600-b45b-11eb-8d0f-0ff7b0546ae5.gif)

몬스터도 동일한 시스템을 적용해서 래그돌을 사망 모션 뿐만 아니라 다운 모션으로도 활용

이 시스템의 장점
- 래그돌 애니메이션과 특유의 불규칙한 움직임을 거의 온전히 유지하면서 위치를 거의 완벽하게 동기화 시켜줌
- 데디케이티드 서버에서는 피직스 연산을 일절 수행하지 않음
- 벡터 변수 하나로 래그돌의 위치를 동기화

이 시스템의 문제점
- 피직스 연산중인 클라이언트가 도중에 나가게되면 피직스 연산을 대신 이어나갈 클라이언트를 찾아야 함 ( 아직 미구현 )
- 피직스 연산을 수행해야할 클라이언트가 피직스 연산의 대상이되는 액터를 한 번도 보지 못했다면 피직스 연산 버그 발생 ( 피직스 연산을 온전히 수행 가능한 클라이언트만 선별 검색 하도록 구현할 예정 )

# 래그돌 위치 동기화 시스템 v2

### 래그돌 위치 동기화 Off
![래그돌 위치 동기화 off 1](https://user-images.githubusercontent.com/12960463/129445940-ea19b014-3ecb-408d-835a-785add5872c9.gif)
- 래그돌 상태에서 일어설 때 위치가 조정되는 것을 확인 가능

### 래그돌 위치 동기화 v.1 On
![래그돌 위치 동기화 개선전1](https://user-images.githubusercontent.com/12960463/129445995-f6a5713d-8f0b-4c9a-b0e0-a52788fbd908.gif)
![래그돌 위치 동기화 개선전2](https://user-images.githubusercontent.com/12960463/129445998-3e005c9c-8147-4b26-9a27-73bccd60c4ca.gif)
![래그돌 위치 동기화 개선전3](https://user-images.githubusercontent.com/12960463/129446000-c83380a9-6b66-4270-9511-fe738c980cfb.gif)
- 위치는 동기화되나 래그돌 전환 순간 0.2초간의 멈춤현상이 있음
  - 서버의 이전 시간에서 현재 시간까지 보간 이동 하기 때문에 서버의 다음 위치가 갱신되어야 래그돌 위치 동기화가 됨
  - 하지만 서버 위치의 갱신 주기가 0.2초이므로 그 사이 위치가 고정되어 버림
### 래그돌 위치 동기화 v2. On
![래그돌 위치 동기화 개선후1](https://user-images.githubusercontent.com/12960463/129446033-411df90e-1c54-4767-9c7b-34c628ca67fd.gif)
![래그돌 위치 동기화 개선후2](https://user-images.githubusercontent.com/12960463/129446036-620dc14d-6bcf-47b0-9d64-739ff62bc09c.gif)
![래그돌 위치 동기화 개선후3](https://user-images.githubusercontent.com/12960463/129446037-a7a4b52f-e809-4a01-bf6d-cf2018300ca5.gif)
![래그돌 위치 동기화 개선후4](https://user-images.githubusercontent.com/12960463/129446040-2271078b-2f6d-4bdd-be52-b0326f533c85.gif)
- 래그돌 전환 순간에 딜레이가 없음
  - 서버의 위치 갱신 전 까지 클라이언트에서 자체적으로 래그돌 시뮬레이션 실행
  - 서버의 위치 갱신 타이밍에 래그돌의 위치가 서버 위치에서 20 이상 차이나면 그 때 physics handle로 래그돌을 서버 위치로 옮김


#### 코드 : 에어본 상태에서 래그돌로 전환 여부를 결정하는 충돌 검사 함수
``` c++
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
```
#### 코드 : 캐릭터를 래그돌로 전환 함수
```c++
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
		last_replication_delay = 0.35f;
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
```
#### 코드 : Tick 함수 내에서 래그돌 동기화 함수 매 프레임 실행시키는 코드 ( Tick 함수 코드 일부 )
```c++
//... Tick함수
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
  // ...
```
#### 코드 : 동기화 함수 ( 액터가 클라이언트 소유인 경우 )
```c++
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
		if (UKismetMathLibrary::Vector_Distance(prev_ragdoll_server_location, ragdoll_server_location) > 10) {
			// 래그돌이 이전 동기화 위치에서 10 이상 이동했으면 RPC를 통해 서버로 동기화 될 위치를 보냄
			Cast<ABaseCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn())->CtoS_targetLocation(this, ragdoll_server_location);
			prev_ragdoll_server_location = ragdoll_server_location;
		}
	}
	else {
		replication_delay_count += d_time;
	}
}
```

#### 코드 : 동기화 함수 ( 스탠드얼론 or 데디케이티드 서버일 때 )
```c++
/// <summary>
/// 서버 소유 액터 or 스탠드얼론 : 래그돌의 위치로 액터를 이동
/// </summary>
void ABaseCharacter::ragdoll_ServerOnwer_Implementation() {
	FVector sock_location = GetMesh()->GetSocketLocation("pelvis");
	ragdoll_server_location = sock_location;
	stickToTheGround(ragdoll_server_location);
}
```

#### 코드 : 동기화 함수 ( 리모트 액터일 때 )
```c++
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
		// 이전 위치 갱신 시간과 현재 위치 갱신 시간의 텀을 계산해 갱신
		float tmp = replication_delay_count - last_replication_delay;
		last_replication_delay = replication_delay_count;
		replication_delay_count = tmp + d_time;
		prev_ragdoll_server_location = last_ragdoll_server_location;
		if (FVector::Dist(ragdoll_server_location, GetMesh()->GetSocketLocation("pelvis")) > 20) {
			// 현재 클라이언트에서 대상 래그돌이 서버의 위치와 20보다 차이난다면 physics handle 로 메쉬의 pelvis 를 잡아 서버 위치로 옮김
			ragdoll_physics_handle->GrabComponent(GetMesh(), TEXT("pelvis"), GetMesh()->GetSocketLocation(TEXT("pelvis")), true);
			ragdoll_physics_handle->SetTargetLocation(ragdoll_server_location);
			stickToTheGround(ragdoll_server_location);
			last_ragdoll_server_location = ragdoll_server_location;
		}
		else {
			// 갱신된 서버의 위치와의 거리가 20이하라면 자유롭게 래그돌 시뮬레이션이 작동하도록 physics handle을 release
			ragdoll_physics_handle->ReleaseComponent();
		}
	}
}
```

# 전투 시스템
## 액션 시스템
- 액션 데이터

![image](https://user-images.githubusercontent.com/12960463/124944008-f2753700-e047-11eb-8f99-97194d782d9d.png)
#### 코드 : 액션 데이터 구조체
```c++
USTRUCT(Atomic, BlueprintType)
struct FActionData : public FTableRowBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		FString name;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		UAnimMontage* anim_montage;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		float stamina;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		float stamina_penalty;
	/* 라스트 인풋으로 회전하는데 걸리는 시간 */
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		float rotate_time;
	/* 액션 시 회전 방향 결정 */
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		EActionRotateType action_rotate_type;
	/* 액션 키를 release 했을 때 실행될 액션(차지, 홀드 기술일시). "None" 이면 없음 */
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		FName release_action = "None";
};
```
- 액션 데이터 구조체의 각 프로퍼티 상세
  - name : 액션의 이름
  - anim_montage : 액션의 애니메이션 애셋(몽타주)
  - stamina : 액션의 소비 스테미너
  - stamina_penalty : 액션의 스테미너 패널티 ( 스테미너 최대값 감소량 )
  - rotate_time : 액션 실행시 타겟 방향으로 회전하는 시간
  - action_rotate_type : 액션 실행시 회전 방식
    - Target : 액션 실행시 타게팅 중인 적을 향해 회전
    - Input : 액션 실행시 last input vector 로 회전
    - Static : 액션 실행시 회전하지 않음
  - release_action : 키를 release 했을 때 실행될 액션
    - 차지기술이나 홀드 기술일 시 release에 반응할 액션 
- 액션은 BaseCharacter 의 상위 클래스인 BP_PlayerCharacter_cpp 블루프린트에서 구현되어 있음

## 데미지 시스템
- 데미지 데이터

![image](https://user-images.githubusercontent.com/12960463/124945527-33218000-e049-11eb-83a8-168392876127.png)
#### 코드 : 데미지 데이터 구조체
```c++
USTRUCT(Atomic, BlueprintType)
struct FdamageData : public FTableRowBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		FString name;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		float base_damage;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		float base_damage_percent;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		FVector knock_back;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		EKnockBackType knock_back_type;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		EDamageElementalType elemental;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		ETargetControlType target_control;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		EBuffAndDebuffType buff_and_debuff;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		EAttackType attack_type;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		float durability_level;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		FVector knock_back_offset;

};
```
- 데미지 데이터 구조체의 프로퍼티 상세
  - base_damage : 데미지의 기본 데미지
  - base_damage_percent : damage_causer 의 power 계수
  - knock_back : 넉백 벡터
  - knock_back_type : 넉백 벡터 적용 방식
    - Directional : damage_causer 가 보는 방향을 기준으로 knock_back 벡터 적용
    - Radial : damage_causer 와 피격된 객체의 look_at 방향을 기준으로 knock_back 벡터 적용 ( 방사형으로 퍼짐 )
    - RadialXY : Radial 과 동일하지만 XY 평면상의 위치 관계에서 적용
    - RadialXYDistanceReverse : RadialXY 에서 두 객체의 거리가 가까울 수록 knock_back 벡터의 크기를 줄여서 적용 ( damage_causer 위치로 끌어당김 )
  - knock_back_offset : knock_back 적용시 기준이 될 위치 ( damage_causer 의 위치에서 상대적인 위치로 계산 )
  - durability_level : 적을 경직시키는 레벨
    - 데미지 데이터의 durability_level 이 피격 캐릭터의 durability_level 보다 높으면 적은 경직과 넉백이 
#### 코드 : 넉백 벡터 계산 코드
```c++
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
```

### 충돌 판정
#### 코드 : 무기 메쉬에 위치한 소켓을 기준으로 trace 하여 충돌 판정하는 Anim Notify State
```c++
// Fill out your copyright notice in the Description page of Project Settings.


#include "NS_Attack_Weapon_2Sock_Trace.h"
#include "../Public/Interface_BaseCharacter.h"
#include "../Public/Interface_PlayerController.h"
#include "../CustomData.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "../Public/PWOGameInstance.h"

void UNS_Attack_Weapon_2Sock_Trace::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) {
	
	if (MeshComp->GetWorld()->GetFirstPlayerController() == NULL)
		return;
	AActor* actor = MeshComp->GetOwner();
	FdamageData damage_data;
	Cast<UPWOGameInstance>(actor->GetGameInstance())->findDamageData(damage_id, damage_data);
	if (actor->GetClass()->ImplementsInterface(UInterface_BaseCharacter::StaticClass())) {
		MeshComp->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::AlwaysTickPoseAndRefreshBones;
		IInterface_BaseCharacter::Execute_getWeapon(actor, weapon_key, weapon);
		IInterface_BaseCharacter::Execute_resetHitActorList(actor);
		IInterface_BaseCharacter::Execute_setDamageData(actor, damage_data);
		IInterface_BaseCharacter::Execute_setDamageID(actor, damage_id);
		trace_channel = IInterface_BaseCharacter::Execute_getAttackTraceChannel(actor);
	}
}

void UNS_Attack_Weapon_2Sock_Trace::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime) {
	AActor* actor = MeshComp->GetOwner();
	if(MeshComp->GetWorld()->GetFirstPlayerController() == NULL)
		return;
	if (actor->GetClass()->ImplementsInterface(UInterface_BaseCharacter::StaticClass())) {
		FdamageData damage_data;
		FVector cur_sock_start = weapon->GetSocketLocation(socket_start);
		FVector cur_sock_end = weapon->GetSocketLocation(socket_end);
		FRotator trace_rotation = UKismetMathLibrary::FindLookAtRotation(cur_sock_start, cur_sock_end);;
		TArray<FHitResult> hit_results;
		const TArray<AActor*> ignore_actors;
		UKismetSystemLibrary::BoxTraceMulti(MeshComp, cur_sock_start, cur_sock_end, volume * actor->GetActorScale().X, trace_rotation, trace_channel, false, ignore_actors, EDrawDebugTrace::Type::None, hit_results, true);
		TSet<AActor*> ign;
		for (auto i : hit_results) {
			if (i.GetActor()->GetClass()->ImplementsInterface(UInterface_BaseCharacter::StaticClass())) {
				if (ign.Contains(i.GetActor()) == false)
					IInterface_BaseCharacter::Execute_attackEvent(actor, i.GetActor(), i);
				ign.Add(i.GetActor());
			}
		}
	}
}

void UNS_Attack_Weapon_2Sock_Trace::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) {
	if (MeshComp->GetWorld()->GetFirstPlayerController() == NULL)
		return;
	MeshComp->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::OnlyTickMontagesWhenNotRendered;
}
```
![image](https://user-images.githubusercontent.com/12960463/124948453-ac21d700-e04b-11eb-8f74-2041241ae63f.png)
![image](https://user-images.githubusercontent.com/12960463/124948599-c956a580-e04b-11eb-87f8-f9abcae36725.png)

이런식으로 몽타주에서 notify 를 실행해 해당 기간 동안 충돌 판정
- 위의 충돌 판정 notify 는 데디케이티드 서버에서 실행하지 않고 클라이언트에서만 실행
  - 서버에서는 tick 속도가 느리기 때문에 비교적 부정확한 충돌판정이 발생
  - 따라서 충돌 판정은 클라이언트에서 전담

### 코드 : 위의 notify state를 통해 충돌이 감지되면 공격중인 캐릭터가 실행하는 함수
```c++
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
	}
}
```
- 위 attackEvent 함수를 통해 플레이어 컨트롤러로 applyDamage에 필요한 데이터를 전달
- 플레이어 컨트롤러에서 서버로 충돌이 발생했다고 알리는 함수를 실행
- 서버에서 피격 캐릭터로 multicast로 applyDamage 함수를 실행
- 충돌 관련 RPC에서 DamageData 가 아닌 damage_id를 전달하는 이유
  - damage_id 를 전달하는 경우 DamageDataTable을 다시 검색해야함
  - 하지만 DamageData 는 크기가 damage_id 보다 훨씬 크므로 네트워크에 부하가 더 큼

## 넉백 애니메이션 and 피격 애니메이션 and 피지컬 애니메이션

![전방향피격모션(sm)](https://user-images.githubusercontent.com/12960463/117236043-dea02f80-ae62-11eb-9aad-c63582fff7f7.gif)

전,후,좌,우의 4방향 피격모션에 로코모션을 블렌딩해서 넉백시 밀려나면서도 균형을 잡으려는 매우 자연스러운 모션 구현

![몬스터 사망(sm)](https://user-images.githubusercontent.com/12960463/117236948-cc26f580-ae64-11eb-91d2-6a16058f0c3d.gif)

몬스터가 사망할 때 넉백의 운동량이 그대로 사망 래그돌 모션에 적용되어 넉백 방향으로 쓰러져 죽는 모션 구현

![히트 본 덜렁 sm](https://user-images.githubusercontent.com/12960463/121346210-c8edc080-c960-11eb-9f05-f90f96262203.gif)

슈퍼아머인 적은 경직 hit 애니메이션을 실행하지 않기 때문에 기존 애니메이션 위에 피격 부위만 덜렁거리는 피지컬 애니메이션으로 구현

#### 코드 : applyDamage 코드 ( 피지컬 애니메이션, 넉백 )
```c++
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
```
#### 코드 : 실제 넉백&에어본 적용 함수
``` c++
/// <summary>
/// 일반적인 히트시 damage_date 의 target_control 에 따라 넉백, 에어본을 적용
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
	if (airbone_HitChk(rotated_vector, hitnormal)) {
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
```

#### 코드 : 피격 방향에 따른 피격 애니메이션 반환 함수
``` c++
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
```
#### 코드 : 공격을 방어했는지 여부를 반환
``` c++
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
		float angle = UKismetMathLibrary::Dot_VectorVector(loc1.GetSafeNormal(), loc2.GetSafeNormal());
		if (angle > 0.2f) {
			/* 적과의 각의 코사인이 0.2 보다 크면 가드로 인정*/
			return true;
		}
	}
	return false;
}
```
#### 코드 : 피 이펙트 스폰 함수
``` c++
void ABaseCharacter::spawnBloodEffect(FVector __location, FVector __rot_vector, FName __bone_name)
{
	UPWOGameInstance* gameinstance = Cast<UPWOGameInstance>(GetGameInstance());
	UNiagaraComponent* blood_effect = UNiagaraFunctionLibrary::SpawnSystemAttached(gameinstance->blood_effect, GetMesh(), __bone_name, FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::Type::SnapToTarget, true, true, ENCPoolMethod::AutoRelease, true);
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
```
#### 코드 : 무경직 히트시 피격 부위에 피지컬 애니메이션 실행 함수
``` c++
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
```

#### 코드 : 넉백 프로세스 ( 틱 함수 내에서 매 틱 실행 )
```c++
/// <summary>
/// 넉백 연산 수행 (매 틱 실행)
/// </summary>
/// <param name="velocity">넉백 벨로시티</param>
void ABaseCharacter::knock_BackProcess_Implementation() {
	if (knock_back_speed > 0) {
		knock_back_speed = UKismetMathLibrary::FInterpTo(knock_back_speed, 0, d_time, 5);
		if (knock_back_speed <= 5.0f) {
			knock_back_unit_vector = FVector::ZeroVector;
			if (network_owner_type == ENetworkOwnerType::RemoteAI)
				current_velocty = FVector::ZeroVector;
			knock_back_speed = 0;
			if (is_on_sprint)
				GetCharacterMovement()->MaxWalkSpeed = sprint_speed;
			else
				GetCharacterMovement()->MaxWalkSpeed = walk_speed;
			GetCharacterMovement()->MaxAcceleration = 2048.0f;
		}
		else {
			GetCharacterMovement()->MaxWalkSpeed = knock_back_speed * 4;
			AddMovementInput(knock_back_unit_vector, 1.0f);
			if(network_owner_type == ENetworkOwnerType::RemoteAI)
				current_velocty = knock_back_unit_vector * (GetCharacterMovement()->MaxWalkSpeed);
		}
	}
}
```

#### 코드 : 히트 본 피지컬 애니메이션 프로세스 ( 틱 함수 내에서 매 틱 실행 )
```c++
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
```

## 방어 시스템
![가드3](https://user-images.githubusercontent.com/12960463/127767069-876b15aa-17cc-41a8-a581-80c642ab4e84.gif)

- 가드 중일시 대략 전방 140도 범위의 공격 방어
- 가드시 에어본을 무시하고 넉백을 1/2 만 적용
- 가드시 불똥 이펙트와 하면 흔들림, 사운드 이펙트 실행
- 가드 중 걷기만 가능하며 기존 스프린트 상태와, 걷기 상태들과 별개로 항상 걷기 유지
  - 가드 중에도 스프린트 키를 누르면 is_sprint 변수는 true로 변하지만 캐릭터는 걷고있음
- 가드 해제시 is_sprint 에 따라 스프린트 전환

#### 코드 : 공격을 방어했는지 여부를 반환
``` c++
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
		float angle = UKismetMathLibrary::Dot_VectorVector(loc1.GetSafeNormal(), loc2.GetSafeNormal());
		if (angle > 0.2f) {
			/* 적과의 각의 코사인이 0.2 보다 크면 가드로 인정*/
			return true;
		}
	}
	return false;
}
```

# 아이템 데이터 구조와 작동 방식
- 보급형 롱소드 데이터
![image](https://user-images.githubusercontent.com/12960463/124911020-3e63b400-e027-11eb-915b-f160dcfa49bf.png)
- 보급형 체력 포션 데이터
![image](https://user-images.githubusercontent.com/12960463/124911056-4ae80c80-e027-11eb-9e1f-f313a343598d.png)

#### 코드 : 아이템 데이터 구조체
```c++
USTRUCT(Atomic, BlueprintType)
struct FItemData : public FTableRowBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		FText name;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		UTexture2D* icon;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		EItemType item_type;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		EItemRank item_rank;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, meta = (MultiLine="true"))
		FText item_info_text;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		TArray<FItemEffect> item_effect_list;
};
```
- 구조체의 각 프로퍼티 상세
  - name : 로컬라이징 가능한 아이템의 이름
  - icon : 아이템의 아이콘 레퍼런스
  - item_type : 아이템의 타입
    - Consumables : 소비 아이템
    - Raw : 재료 아이템
    - Key : 게임 진행 관련 중요 아이템
    - Equipment_Weapon : 무기 장비 아이템
    - Equipment_Head : 모자 장비 아이템
    - ...
  - item_rank : 아이템 등급
    - Common, Rare, Precious, Unique, Legendary
  - item_info_text : 아이템의 기본 설명
  - item_effect_list : 아이템의 효과들
## 아이템 효과 시스템
- 아이템의 실제 정체성과 효과를 정의
- ItemEffect 위치
![image](https://user-images.githubusercontent.com/12960463/124912056-76b7c200-e028-11eb-8c10-804f21fe429d.png)
- ItemEffect 목록
![image](https://user-images.githubusercontent.com/12960463/124912229-a8c92400-e028-11eb-8a72-d3df8e45777a.png)
- ItemEffect 클래스들
![image](https://user-images.githubusercontent.com/12960463/124914833-c6e45380-e02b-11eb-9eb5-d4b5519104f2.png)


#### 코드 : BaseItemEffect 헤더파일
```c++
#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Interface_ItemEffect.h"


#include "BaseItemEffect.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class CYPERARENA_API UBaseItemEffect : public UObject, public IInterface_ItemEffect
{
	GENERATED_BODY()
public :
	UBaseItemEffect();
	~UBaseItemEffect();
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "ItemEffect")
		float value;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "ItemEffect")
		FName item_id;
};
```
#### 코드 : 아이템 이펙트 인터페이스
```c++
#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CustomEnums.h"

#include "Interface_ItemEffect.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInterface_ItemEffect : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class CYPERARENA_API IInterface_ItemEffect
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	/* 아이템 효과 적용시 */
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Interface-ItemEffect")
		void applyItemEffect(ACharacter* causor, int32 __inven_index);
	/* 아이템 설명 텍스트 요구시 */
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Interface-ItemEffect")
		FText describeItemEffect();
	/* 아이템을 퀵슬롯이나 장비 슬롯에 등록시 */
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Interface-ItemEffect")
		void onRegistration(ACharacter* causor, int32 __inven_index);
	/* 아이템을 퀵슬롯이나 장비 슬롯에서 해제시 */
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Interface-ItemEffect")
		void onRemoveRegistration(ACharacter* causor, int32 __inven_index);
	/* 아이템 사용시 (인벤토리에서 우클릭하거나 퀵슬롯에 등록 후 퀵슬롯 키 입력시 ) */
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Interface-ItemEffect")
		void onActivate(ACharacter* causor, int32 __inven_index);
};
```
- ItemEffect 는 아이템의 데이터와 효과를 분리하여 컴포넌트 처럼 아이템효과를 아이템데이터에 추가하는 것 만으로 해당 아이템의 효과를 정의할 수 있게 만든 시스템
- 위의 BaseItemEffect 를 상속받은 클래스를 ItemEffect 리스트에 추가할 수 있음
- ItemEffect 를 사용할 일이 있으면 ItemEffect의 클래스 디폴트 오브젝트의 프로퍼티를 FItemEffect의 value로 초기화하고 인터페이스를 통해 필요한 역할을 수행시킴
- 아이템의 정보를 표시할 때 item_effect_list 의 모든 ItemEffect의 describeItemEffect() 를 실행해서 설명창에 추가
  - ![image](https://user-images.githubusercontent.com/12960463/124913259-e2e6f580-e029-11eb-9eae-9cc4ed767f96.png)![image](https://user-images.githubusercontent.com/12960463/124914499-5e957200-e02b-11eb-95ad-39ce26f401eb.png)
- 아이템을 사용할 때 item_effect_list 의 모든 ItemEffect의 applyItemEffect()를 실행
  - 위의 보급형 체력 포션의 경우 item_effect_list에 IE_Action_Drink, IE_HealBase, IE_DecreaseCount 이 세 개의 ItemEffect가 들어가 있음
  - IE_Action_Drink
    - onActivate() 실행시 캐릭터의 drink 애니메이션을 실행함.
    - 그 외 다른 인터페이스 함수는 구현하지 않음
  - IE_HealBase
    - applyItemEffect() 실행시 캐릭터의 hp를 value 만큼 회복
    - describeItemEffect() 실행시 "체력을 {value} 회복" 을 출력
    - 그 외 다른 인터페이스 함수는 구현하지 않음
  - IE_DecreaseCount
    - applyItemEffect() 실행시 아이템의 갯수를 1개 감소
    - 그 외 다른 인터페이스 함수는 구현하지 않음
- 아이템 장착시에도 동일한 ItemEffect 시스템을 사용해서 구현
  - 아이템 장착시 ItemEffect의 onRegistration() 함수를 실행하여 아이템의 공격력 + 20 같은 효과를 적용
  - 아이템 장착 해제시 ItemEffect의 onRemoveRegistration() 함수를 실행하여 아이템의 공격력 + 20 효과를 역으로 적용해서 캐릭터를 원상태로 돌려놓음
  - IE_Equip
    - onActivate() 실행시 아이템에 해당하는 장비 슬롯에 장착
  - IE_Stat_Power
    - onRegistration() 실행시 캐릭터의 공격력을 value 만큼 상승
    - onRemoveRegistration() 실행시 캐릭터의 공격력을 value 만큼 감소
    - describeItemEffect() 실행시 "공격력 + {value}"를 출력
  - IE_ChangeMesh_Weapon
    - onRegistration() 실행시 캐릭터의 무기 메쉬를 해당 아이템의 item_id 에 해당하는 메쉬로 바꿈 ( ItemMeshDataTable 참조 )
    - onRemoveRegistration() 실행시 캐릭터의 무기 메쉬를 ItemMeshDataTable 의 sword_none 으로 바꿈
  - 위의 세 가지 ItemEffect 를 추가하면 장착시 무기의 외형이 변하고 공격력이 20 상승하는 무기를 만들 수 있음. 

#### 코드 : 아이템 이펙트 실행 코드 ( 멀티캐스트를 위해 플레이어 캐릭터에서 실행됨 )
```c++
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
```
- 아이템 디테일 윈도우

![아이템 등급](https://user-images.githubusercontent.com/12960463/124884700-7fe76580-e00d-11eb-9303-29563c5ee4f3.gif)
#### 코드 : 아이템 디테일 위젯 초기화 함수
```c++
void UWidget_Detail::initDetail(FName __item_id) {
	FItemData itemdata = Cast<UPWOGameInstance>(GetGameInstance())->findItemData(__item_id);
	name_text->SetText(itemdata.name);
	/* 아이템 랭크에 따라 name_text의 폰트 색 변경 */
	switch (itemdata.item_rank)
	{
	case EItemRank::Common:
		name_text->SetColorAndOpacity(FSlateColor(FLinearColor::White));
		break;
	case EItemRank::Rare:
		name_text->SetColorAndOpacity(FSlateColor(FLinearColor::Blue));
		break;
	case EItemRank::Precious:
		name_text->SetColorAndOpacity(FSlateColor(FLinearColor::FromSRGBColor(FColor::Purple)));
		break;
	case EItemRank::Unique:
		name_text->SetColorAndOpacity(FSlateColor(FLinearColor::Red));
		break;
	case EItemRank::Legendary:
		name_text->SetColorAndOpacity(FSlateColor(FLinearColor::FromSRGBColor(FColor::Orange)));
		break;
	default:
		break;
	}
	image->SetBrushFromTexture(itemdata.icon);
	info_text->SetText(itemdata.item_info_text);
	/* itemdata 에서 item_effect_list 를 순회하며 모든 ItemEffect 의 describeItemEffect()를 실행해서 출력값을 effect_text에 추가 */
	if (itemdata.item_effect_list.Num() > 0) {
		FString final_effect_text;
		for (FItemEffect i : itemdata.item_effect_list) {
			auto item_effect_obj = i.item_effect.GetDefaultObject();
			item_effect_obj->value = i.value;
			if (i.item_effect->ImplementsInterface(UInterface_ItemEffect::StaticClass())) {
				FString cur_text = IInterface_ItemEffect::Execute_describeItemEffect(item_effect_obj).ToString();
				if (cur_text == "")
					continue;
				final_effect_text += TEXT("\n");
				final_effect_text += IInterface_ItemEffect::Execute_describeItemEffect(item_effect_obj).ToString();
			}
		}
		effect_text->SetText(FText::FromString(final_effect_text.TrimStartAndEnd()));
		effect_text->SetVisibility(ESlateVisibility::Visible);
	}
	else {
		effect_text->SetVisibility(ESlateVisibility::Collapsed);
	}
}
```
#### 코드 : 위젯이 화면을 벗어나지 않도록 위치 조정 ( 크기와 위치가 가변적인 위젯이라 필요. 항상 아이템 슬롯의 모서리를 기준으로 위치 )
```c++
/// <summary>
/// 위젯이 화면 밖을 벗어났는지 체크
/// 화면에서 벗어났으면 위치를 조정
/// </summary>
void UWidget_Detail::onViewPortCheck()
{
	FVector2D viewport_size;
	GetWorld()->GetGameViewport()->GetViewportSize(viewport_size);
	FGeometry geometry = GetTickSpaceGeometry();
	FVector2D abs_to_local_vector = geometry.GetAbsoluteSize() / geometry.GetLocalSize();	// 절대 크기와 local 크기를 나눠서 현재 screen 크기와 desire 크기의 비율을 알아냄
	FVector2D target_position = geometry.Position * abs_to_local_vector;	// 위에서 구한 화면비를 위젯 지오메트리 position 에 곱해서 절대 위치를 local 위치로 변환
	bool flag = false;
	/* x축과 y축에 대해서 위젯이 화면을 벗어났는지 체크하고 그에따라 위치변환 */
	/* 디폴트로 디테일 위젯은 아이템 슬롯의 우측 하단에 위치하게 되며 x축에서 화면을 벗어나면 슬롯 좌측으로 이동시키도록 target_position 조절.
	y축을 벗어나면 동일한 방식으로 슬롯 상단으로 이동시킴 */
	if (geometry.GetAbsoluteSize().X + target_position.X > viewport_size.X) {
		target_position.X -= 85 * abs_to_local_vector.X + geometry.GetAbsoluteSize().X;		// 85는 아이템 슬롯의 절대 사이즈
		flag = true;
	}
	if (geometry.GetAbsoluteSize().Y + target_position.Y > viewport_size.Y) {
		target_position.Y -= 85 * abs_to_local_vector.Y + geometry.GetAbsoluteSize().Y;
		flag = true;
	}
	if (flag)
		SetPositionInViewport(target_position);
}
```
# 인벤토리 시스템
- 아이템 등록

![아이템 등록50%](https://user-images.githubusercontent.com/12960463/124921237-25f99680-e033-11eb-9bea-b26386210702.gif)
- 아이템 장착 (장비 효과 적용)

![아이템 장착50%](https://user-images.githubusercontent.com/12960463/124920900-ca2f0d80-e032-11eb-93f5-6a30e75dbe2c.gif)
- 퀵슬롯 아이템 사용

![퀵슬롯 아이템 사용70%](https://user-images.githubusercontent.com/12960463/124921509-7244d680-e033-11eb-9fef-fcc32719ba26.gif)

![image](https://user-images.githubusercontent.com/12960463/124903877-618a6580-e01f-11eb-9dbe-2b4c4d29de3e.png)

인벤토리 시스템의 데이터 구조
- 인벤토리 데이터는 PlayerController 내에 존재
  - 이유
    - 인벤토리 데이터는 클라이언트와 서버 양쪽에 존재해야함
    - 클라이언트는 다른 클라이언트의 인벤토리 정보에 대해 알 필요가 없음
    - 따라서 서버에는 유저 수 만큼 존재하며 클라이언트에서는 자신의 PlayerController 하나만 존재하는 PlayerController가 적합하다고 판단했음
#### 코드 : 인벤토리 관련 4가지 프로퍼티
```c++
UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	TMap<int32, FInventoryData> inventory_list;
UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	TMap<FKey, int32> quickslot_list;
UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	TMap<int32, FKey> reverse_quickslot_list;
UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	TMap<EEquipmentType, FInventoryData> equipment_list;
```
- 각 프로퍼티 
  - inventory_list
    - 키 : 인벤토리내 해당 아이템의 인덱스
    - 밸류 : 인벤토리 데이터 ( 아이템의 ID, 갯수 )
    - 인벤토리의 특성상 희소 행렬인 경우가 많기 때문에 array 보다는 map 을 이용하여 구현했음
    - map을 이용함으로써 메모리를 절약하고, 저장시 구현을 간편하게 함
  - quickslot_list
    - 키 : 퀵슬롯을 발동하는 입력 키
    - 밸류 : 등록된 아이템의 inventory_list 내의 index(키)
    - 퀵슬롯은 인벤토리의 인덱스를 가리키도록 구현
  - reverse_quickslot_list
    - 키 : 인벤토리내 해당 아이템의 인덱스
    - 밸류 : 해당 아이템이 등록된 퀵슬롯의 키
    - 퀵슬롯을 순회하지 않고 인벤토리 내 아이템에 연결된 퀵슬롯의 키를 얻어내기 위한 데이터
      - 모든 아이템에 퀵슬롯 매칭 정보를 넣으면 더 간편하게 구현할 수 있겠지만 인벤토리 아이템에서 퀵슬롯에 등록된 아이템의 수는 매우 적기 때문에 불필요한 메모리 사용을 줄이기 위해 quickslot_list 과 reverse_quickslot_list 가 서로를 가리키는 양방향 맵으로 구현하여 퀵슬롯, 인벤토리 슬롯 어디에서도 빠르게 연관된 퀵슬롯 데이터, 인벤토리 데이터에 접근할 수 있도록 구현
  - equipment_list
    - 키 : 장비 슬롯의 고유 키
    - 밸류 : 인벤토리 데이터 ( 아이템의 ID, 갯수 )
    - inventory_list 동일한 구성으로 동일한 방식으로 작동

- GameInstance를 이용한 아이템 슬롯 위젯 레퍼런스 관리
![image](https://user-images.githubusercontent.com/12960463/124918626-365c4200-e030-11eb-8c8c-5cb60f600c5a.png)
![image](https://user-images.githubusercontent.com/12960463/124918816-702d4880-e030-11eb-9094-2004c5b12e47.png)
- 위 블루프린트 처럼 위젯 Construct 이벤트에서 GameInstance 의 Inventory_Slot_Reference 맵에 등록
- 위젯 Destruct 시 GameInstance 의 Inventory_Slot_Reference 맵에서 삭제
- 이를통해 인벤토리를 열었을 때만 위젯의 레퍼런스를 관리해서 메모리를 절약할 수 있음
- 게임 인스턴스를 통해 프로그램 내 어디에서든 위젯에 접근할 수 있음
- 위젯은 반드시 하나만 존재하는 것이기 때문에 게임에서 단 하나만 존재하는 GameInstance에서 관리하는 것이 효과적

#### 코드 : 인벤토리에서 빈 index 반환 함수
```c++
/// <summary>
/// 인벤토리에서 빈 index 반환
/// </summary>
/// <returns></returns>
int32 AController_Player::findInventoryEmptyIndex_Implementation()
{
	TArray<int32> keys;
	inventory_list.GetKeys(keys);
	keys.Sort();
	int32 min = 0;
	for (auto i : keys) {
		if (i != min)
			return min;
		else
			min += 1;
	}
	return min;
}
```
#### 코드 : 인벤토리에서 동일한 item_id 를 가진 index를 반환하는 함수
```c++
/// <summary>
/// 동일한 __item_id를 가진 슬롯의 index 반환
/// </summary>
/// <param name="__item_id"></param>
/// <returns></returns>
int32 AController_Player::findSameItem_Implementation(FName __item_id)
{
	TArray<FInventoryData> values;
	TArray<int32> keys;
	inventory_list.GenerateValueArray(values);
	inventory_list.GetKeys(keys);
	int nums = values.Num();
	for (int i = 0; i < nums; i++) {
		if (values[i].item_id == __item_id) {
			return keys[i];
		}
	}
	return -1;
}
```
#### 코드 : 아이템 획득 함수
```c++
/// <summary>
/// 아이템 획득
/// 획득 아이템이 이미 소지하고 있는 stackable 아이템이라면 해당 수치만 증가시키고 UI 를 업데이트
/// 아니라면 findInventoryEmptyIndex() 함수를 이용해 인벤토리의 빈 인덱스를 찾아 그 곳에 아이템 데이터 삽입
/// </summary>
/// <param name="__item_id">획득 아이템 id</param>
/// <param name="__num">획득 아이템 갯수</param>
void AController_Player::getItem_Implementation(FName __item_id, int32 __num)
{
	if (inventory_list.Num() >= max_slot_size)
		return;
	FItemData itemdata;
	bool add_in_empty_slot = false;
	itemdata = Cast<UPWOGameInstance>(GetGameInstance())->findItemData(__item_id);
	/* stackable 한 아이템일 때 findSameItem 결과가 있으면 그 슬롯의 count만 증가시킴*/
	if (isStackable(itemdata.item_type)) {
		int32 same_item_index = findSameItem(__item_id);
		if (same_item_index >= 0) {
			inventory_list[same_item_index].count += __num;
			if (reverse_quickslot_list.Contains(same_item_index))
				refreshQuickSlot(reverse_quickslot_list[same_item_index]);
		}
		else {
			add_in_empty_slot = true;
		}
	}
	else {
		add_in_empty_slot = true;
	}
	/* empty index에 아이템 add */
	if (add_in_empty_slot) {
		int32 empty_index = findInventoryEmptyIndex();
		FInventoryData data;
		data.item_id = __item_id;
		data.count = __num;
		inventory_list.Add(TTuple<int32, FInventoryData>(empty_index, data));
	}

	/* 현재 서버라면 소유 클라이언트에서도 동일한 작업을 수행하기 위해 클라이언트 함수 실행 */
	if (HasAuthority() && IsLocalController() == false) {
		Client_getItem(__item_id, __num);
	}
}

/// <summary>
/// 클라이언트 에서도 서버에서 수행한 작업을 수행해서 인벤토리가 동기화 되도록 함
/// </summary>
/// <param name="__item_id"></param>
/// <param name="__num"></param>
void AController_Player::Client_getItem_Implementation(FName __item_id, int32 __num)
{
	getItem(__item_id, __num);
}
```
#### 코드 : 인벤토리 슬롯간 스왑 처리 함수
```c++
/// <summary>
/// 인벤토리 슬롯간 drag&drop 으로 스왑했을 때 처리함수
/// 동일 item_id 의 stackable 아이템이라면 두 슬롯을 결합
/// 아니라면 inventory_list의 두 슬롯의 데이터를 단순 스왑
/// 빈 슬롯과 스왑하면 해당 슬롯 인덱스에 add 후 이전 슬롯을 remove
/// </summary>
/// <param name="__from"></param>
/// <param name="__to"></param>
void AController_Player::swapInvenSlot_Implementation(int32 __from, int32 __to)
{
	if (__from == __to)
		return;
	FInventoryData fromdata = inventory_list[__from];
	if (inventory_list.Contains(__to)) {
		/* 동일 item_id 의 stackable 아이템이라면 두 슬롯을 결합 */
		if (fromdata.item_id == inventory_list[__to].item_id && isStackable(Cast<UPWOGameInstance>(GetGameInstance())->findItemData(fromdata.item_id).item_type)) {
			inventory_list[__to].count += fromdata.count;
			inventory_list.Remove(__from);
		}
		else {
			/* 아니라면 단순 스왑*/
			inventory_list[__from] = inventory_list[__to];
			inventory_list[__to] = fromdata;
		}
	}
	else {
		/* 빈 슬롯과 스왑했으면 빈 슬롯에 이전 데이터 add 후 이전의 데이터는 remove */
		inventory_list.Add(TTuple<int32, FInventoryData>(__to, fromdata));
		inventory_list.Remove(__from);
	}
	/* 위젯이 존재한다면 퀵슬롯 정보를 업데이트하고 게임 인스턴스에서 각 슬롯의 위젯 레퍼런스를 받아와서 ui 업데이트 */
	updateQuickSlotData(__from, __to);
	if (IsLocalController()) {
		UPWOGameInstance* gameinstance = Cast<UPWOGameInstance>(GetGameInstance());
		UWidget_ItemSlot* from_widget = gameinstance->inventory_slot_reference.FindRef(__from);
		if(from_widget)
			from_widget->initSlot();
		UWidget_ItemSlot* to_widget = gameinstance->inventory_slot_reference.FindRef(__to);
		if (to_widget)
			to_widget->initSlot();
	}
	/* 현재 컨트롤러가 클라이언트에 있으면 서버에서도 동일한 작업을 수행하기 위해 서버함수 실행 */
	if (HasAuthority() == false)
		Server_swapInvenSlot(__from, __to);
}

/// <summary>
/// 서버에서 클라이언트와 동일한 작업을 수행해서 인벤토리 동기화
/// </summary>
/// <param name="__from"></param>
/// <param name="__to"></param>
void AController_Player::Server_swapInvenSlot_Implementation(int32 __from, int32 __to)
{
	swapInvenSlot(__from, __to);
}
```
#### 코드 : 퀵슬롯간의 스왑 처리 함수
```c++
/// <summary>
/// 퀵슬롯간 스왑 이벤트 처리 함수
/// </summary>
/// <param name="__from"></param>
/// <param name="__to"></param>
void AController_Player::swapQuickSlot_Implementation(FKey __from, FKey __to)
{
	if (__from == __to)
		return;
	int32 fromdata = quickslot_list[__from];
	if (quickslot_list.Contains(__to)) {
		/* 다른 아이템이 등록된 퀵슬롯과의 스왑 */
		quickslot_list[__from] = quickslot_list[__to];
		quickslot_list[__to] = fromdata;
		reverse_quickslot_list[fromdata] = __to;
		reverse_quickslot_list[quickslot_list[__from]] = __from;
	}
	else {
		/* 빈 퀵슬롯과의 스왑 */
		quickslot_list.Add(TTuple<FKey, int32>(__to, fromdata));
		quickslot_list.Remove(__from);
		reverse_quickslot_list[fromdata] = __to;
	}
	/* 스왑 후 refreshQuickSlot() 함수로 퀵슬롯 UI 갱신 */
	refreshQuickSlot(__from);
	refreshQuickSlot(__to);
	/* 현재 컨트롤러가 클라이언트에 있으면 서버에서도 동일한 작업을 수행하기 위해 서버함수 실행 */
	if (HasAuthority() == false)
		Server_swapQuickSlot(__from, __to);
}

/// <summary>
/// 서버에서도 클라이언트와 동일한 작업을 수행해서 인벤토리 동기화
/// </summary>
/// <param name="__from"></param>
/// <param name="__to"></param>
void AController_Player::Server_swapQuickSlot_Implementation(FKey __from, FKey __to)
{
	swapQuickSlot(__from, __to);
}
```
#### 코드 : 퀵슬롯 등록 함수
```c++
/// <summary>
/// 인벤토리에 있는 아이템을 퀵슬롯에 등록하는 함수
/// </summary>
/// <param name="__from"></param>
/// <param name="__to"></param>
void AController_Player::registerInventoQuick_Implementation(int32 __from, FKey __to)
{
	if (reverse_quickslot_list.Contains(__from)) {
		/* 옮기려는 인벤토리 슬롯(__from)이 이전에 이미 다른 퀵슬롯에 등록되어 있을 때의 처리 */
		/* prev_key 옮기려는 슬롯이 이전에 등록되어있던 퀵슬롯 키 */
		FKey prev_key = reverse_quickslot_list[__from];
		if (__to == prev_key)
			return;
		if (quickslot_list.Contains(__to)) {
			/* 목표 슬롯에 이미 다른 아이템이 등록되어 있을때의 처리 */
			/* __from이 이전에 등록되어 있던 퀵슬롯의 데이터를 지우고 __to를 __from으로 변경 reverse_quickslot_list 정보는 새로운 키로 갱신하고 __to 퀵슬롯에 등록되어 있던 슬롯의 정보는 삭제 */
			quickslot_list.Remove(prev_key);
			reverse_quickslot_list[__from] = __to;
			reverse_quickslot_list.Remove(quickslot_list[__to]);
			quickslot_list[__to] = __from;
		}
		else {
			/* 목표슬롯이 빈 슬롯일 때의 처리 */
			/* quickslot_list 에서 이전 슬롯을 제거하고 새로운 정보를 삽입 reverse_quickslot_list 정보는 새로운 키로 갱신*/
			quickslot_list.Remove(prev_key);
			reverse_quickslot_list[__from] = __to;
			quickslot_list.Add(TTuple<FKey, int32>(__to, __from));
		}
		refreshQuickSlot(prev_key);
		refreshQuickSlot(__to);
	}
	/* 옮기려는 슬롯이 퀵슬롯에 등록되어 있지 않은 슬롯일 때의 처리 */
	else if (quickslot_list.Contains(__to)) {
		reverse_quickslot_list.Remove(quickslot_list[__to]);
		quickslot_list[__to] = __from;
		reverse_quickslot_list.Add(TTuple<int32, FKey>(__from, __to));
	}
	else {
		quickslot_list.Add(TTuple<FKey, int32>(__to, __from));
		reverse_quickslot_list.Add(TTuple<int32, FKey>(__from, __to));
	}
	refreshQuickSlot(__to);
	if (IsLocalController()) {
		UWidget_ItemSlot* slot_ref = Cast<UPWOGameInstance>(GetGameInstance())->inventory_slot_reference.FindRef(__from);
		if (slot_ref)
			slot_ref->initSlot();
	}
	

	/* 현재 컨트롤러가 클라이언트에 있으면 서버에서도 동일한 작업을 수행하기 위해 서버함수 실행 */
	if (HasAuthority() == false)
		Server_registerInventoQuick(__from, __to);
}

/// <summary>
/// 서버에서도 클라이언트와 동일한 작업을 수행해서 인벤토리 동기화
/// </summary>
/// <param name="__from"></param>
/// <param name="__to"></param>
void AController_Player::Server_registerInventoQuick_Implementation(int32 __from, FKey __to)
{
	registerInventoQuick(__from, __to);
}
```
#### 코드 : 퀵슬롯 위젯 업데이트 함수
```c++
/// <summary>
/// 퀵슬롯 데이터로 부터 해당 키에 해당하는 퀵슬롯 위젯의 프로퍼티를 갱신하고 UI를 업데이트
/// </summary>
/// <param name="__key"></param>
void AController_Player::refreshQuickSlot(FKey __key)
{
	UPWOGameInstance* gameinstance = Cast<UPWOGameInstance>(GetGameInstance());
	/* 게임인스턴스로 부터 __key에 해당하는 퀵슬롯의 위젯 레퍼런스를 받아옴 */
	UWidget_ItemSlot* prev_quick_slot = gameinstance->quickslot_references[__key];
	FInventoryData invendatadata;
	if (quickslot_list.Contains(__key) && inventory_list.Contains(quickslot_list[__key])) {
		/* __key에 해당하는 퀵슬롯에 아이템이 등록되어 있다면 인벤토리 데이터의 해당 정보로 갱신 */
		prev_quick_slot->my_index = quickslot_list[__key];
		invendatadata = inventory_list[prev_quick_slot->my_index];
	}
	else {
		/* 빈 퀵슬롯이라면 데이터 삭제 */
		prev_quick_slot->my_index = -1;
		invendatadata.item_id = "None";
		invendatadata.count = 0;
	}
	/* 위젯 업데이트 */
	prev_quick_slot->updateUI(invendatadata);
}
```
#### 코드 : 인벤토리 스왑시  퀵슬롯 데이터 조정 함수
```c++
/// <summary>
/// 인벤토리 슬롯시 해당 슬롯들이 퀵슬롯에 등록된 슬롯일 경우
/// 퀵슬롯 관련 데이터인 reverse_quickslot_list, quickslot_list 을 적합하게 업데이트 하여 데이터 꼬임 방지를 위한 함수
/// 인벤토리 슬롯간 스왑으로 인해 변경된 인벤토리 데이터에 맞게 퀵슬롯 데이터를 업데이트
/// - 단순 과정 요약
/// 1. 각 슬롯이 이전에 등록된 퀵슬롯의 키를 저장
/// 2. 각 슬롯의 reverse_quickslot_list 를 삭제하고 quickslot_list 업데이트
/// 3. reverse_quickslot_list에 변경된 quickslot_list 데이터에 맞게 삽입
/// </summary>
/// <param name="__from"></param>
/// <param name="__to"></param>
void AController_Player::updateQuickSlotData(int32 __from, int32 __to) {
	FKey prevfromkey;
	FKey prevtokey;
	/* __from과 __to가 퀵슬롯에 등록되어 있다면 해당 퀵슬롯 키를 저장 */
	if (reverse_quickslot_list.Contains(__from)) {
		prevfromkey = reverse_quickslot_list[__from];
	}
	if (reverse_quickslot_list.Contains(__to)) {
		prevtokey = reverse_quickslot_list[__to];
	}
	bool fromflag = false;
	bool toflag = false;
	/* __from 이 퀵슬롯에 등록되어 있을 경우 reverse_quickslot_list 에서 __from을 지우고 quickslot_list를 __to로 갱신 */
	if (prevfromkey.IsValid()) {
		reverse_quickslot_list.Remove(__from);
		quickslot_list[prevfromkey] = __to;
		fromflag = true;
	}
	/* __to 가 퀵슬롯에 등록되어 있을 경우 reverse_quickslot_list 에서 __to를 지우고 quickslot_list를 __from으로 갱신 */
	/* 예외상황 : 인벤토리 결합에 의해 퀵슬롯이 결합되어 사라진 빈 슬롯의 인덱스를 가리키는 문제가 생길 수 있음 */
	/* 만약 __from 슬롯이 퀵슬롯에 등록되어 있었는데 현재 인벤토리의 __from 슬롯이 비어있다면 __from과 __to 두 슬롯이 결합된 것이기 때문에 이에 맞게 데이터 업데이트 */
	if (prevtokey.IsValid()) {
		if (inventory_list.Contains(__from)) {
			reverse_quickslot_list.Remove(__to);
			quickslot_list[prevtokey] = __from;
			toflag = true;
		}
		else if(prevfromkey.IsValid()){
			/* 인벤토리 슬롯이 결합되었을때 */
			/* fromflag 를 false 로 만듦으로써 reverse_quickslot_list 에 결합되어 사라진 __from슬롯의 정보가 삽입되지 않도록 함 */
			quickslot_list.Remove(prevfromkey);
			fromflag = false;
		}
	}
	/* reverse_quickslot_list에 옳은 정보를 삽입하고 위젯 업데이트 */
	if (prevfromkey.IsValid()) {
		if(fromflag)
			reverse_quickslot_list.Add(TTuple<int32, FKey>(__to, prevfromkey));
		refreshQuickSlot(prevfromkey);
	}
	if (prevtokey.IsValid()) {
		if(toflag)
			reverse_quickslot_list.Add(TTuple<int32, FKey>(__from, prevtokey));
		refreshQuickSlot(prevtokey);
	}
}
```
#### 코드 : 퀵슬롯 위젯 업데이트 함수
```c++
/// <summary>
/// 퀵슬롯 데이터로 부터 해당 키에 해당하는 퀵슬롯 위젯의 프로퍼티를 갱신하고 UI를 업데이트
/// </summary>
/// <param name="__key"></param>
void AController_Player::refreshQuickSlot(FKey __key)
{
	UPWOGameInstance* gameinstance = Cast<UPWOGameInstance>(GetGameInstance());
	/* 게임인스턴스로 부터 __key에 해당하는 퀵슬롯의 위젯 레퍼런스를 받아옴 */
	UWidget_ItemSlot* prev_quick_slot = gameinstance->quickslot_references[__key];
	FInventoryData invendatadata;
	if (quickslot_list.Contains(__key) && inventory_list.Contains(quickslot_list[__key])) {
		/* __key에 해당하는 퀵슬롯에 아이템이 등록되어 있다면 인벤토리 데이터의 해당 정보로 갱신 */
		prev_quick_slot->my_index = quickslot_list[__key];
		invendatadata = inventory_list[prev_quick_slot->my_index];
	}
	else {
		/* 빈 퀵슬롯이라면 데이터 삭제 */
		prev_quick_slot->my_index = -1;
		invendatadata.item_id = "None";
		invendatadata.count = 0;
	}
	/* 위젯 업데이트 */
	prev_quick_slot->updateUI(invendatadata);
}
```
#### 코드 : 퀵슬롯 등록 해제 함수
```c++
/// <summary>
/// 퀵슬롯 등록 해제
/// </summary>
/// <param name="__key"></param>
void AController_Player::removeQuickSlot_Implementation(FKey __key)
{
	reverse_quickslot_list.Remove(quickslot_list[__key]);
	quickslot_list.Remove(__key);
	refreshQuickSlot(__key);
	/* 현재 컨트롤러가 클라이언트에 있으면 서버에서도 동일한 작업을 수행하기 위해 서버함수 실행 */
	if (HasAuthority() == false)
		Server_removeQuickSlot(__key);
}
/// <summary>
/// 서버에서도 클라이언트와 동일한 작업을 수행해서 인벤토리 동기화
/// </summary>
/// <param name="__key"></param>
void AController_Player::Server_removeQuickSlot_Implementation(FKey __key)
{
	removeQuickSlot(__key);
}
```
#### 코드 : 아이템 장착 함수
```c++
/// <summary>
/// 아이템 장착
/// 기본적으론 아이템 슬롯 스왑과 동일한 프로세스를 진행하는 시스템
/// 추가적으로 해제될 아이템의 아이템 해제 효과를 실행하고 등록될 아이템의 아이템 등록 효과를 실행
/// 이를통해 이전 장비의 효과를 모두 제거하고 새로운 장비의 효과를 모두 적용시킴
/// </summary>
/// <param name="__from"></param>
/// <param name="__to"></param>
void AController_Player::equipItem_Implementation(int32 __from, EEquipmentType __to)
{
	UPWOGameInstance* gameinstance = Cast<UPWOGameInstance>(GetGameInstance());
	FInventoryData fromdata = inventory_list[__from];
	if (equipment_list.Contains(__to)) {
		/* multicast로 실행하기 위해 컨트롤 중인 액터에서 해제될 아이템의 모든 아이템 이펙트의 onRemoveRegistration() 를 실행시킴 */
		FName previtemid = equipment_list[__to].item_id;
		if (HasAuthority()) {
			if (GetCharacter()->GetClass()->ImplementsInterface(UInterface_BaseCharacter::StaticClass())) {
				IInterface_BaseCharacter::Execute_ItemEffect_onRemoveRegistration(GetCharacter(), previtemid, __from);
			}
		}
		inventory_list[__from] = equipment_list[__to];
		equipment_list[__to] = fromdata;
	}
	else {
		equipment_list.Add(TTuple<EEquipmentType, FInventoryData>(__to, fromdata));
		inventory_list.Remove(__from);;
	}
	/* multicast로 실행하기 위해 컨트롤 중인 액터에서 새로 장착한 아이템의 모든 아이템 이펙트의 onRegistration() 실행 */
	FName equiped_item_id = equipment_list[__to].item_id;
	if (HasAuthority()) {
		if (GetCharacter()->GetClass()->ImplementsInterface(UInterface_BaseCharacter::StaticClass())) {
			IInterface_BaseCharacter::Execute_ItemEffect_onRegistration(GetCharacter(), equiped_item_id, __from);
		}
	}
	if (IsLocalController()) {
		UWidget_ItemSlot* slot_ref = gameinstance->inventory_slot_reference.FindRef(__from);
		if (slot_ref)
			slot_ref->initSlot();
	}
	refreshEquipmentSlot(__to);
	UKismetSystemLibrary::PrintString(this, TEXT("장착"));
	/* 현재 컨트롤러가 클라이언트에 있으면 서버에서도 동일한 작업을 수행하기 위해 서버함수 실행, 디테일 위젯 열려있으면 삭제 */
	if (HasAuthority() == false) {
		Server_equipItem(__from, __to);
		if(gameinstance->detail_widget_reference->IsValidLowLevel())
			gameinstance->detail_widget_reference->RemoveFromParent();
	}
}
/// <summary>
/// 서버에서도 클라이언트와 동일한 작업을 수행해서 인벤토리 동기화
/// </summary>
/// <param name="__from"></param>
/// <param name="__to"></param>
void AController_Player::Server_equipItem_Implementation(int32 __from, EEquipmentType __to)
{
	equipItem(__from, __to);
}
```
#### 코드 : 아이템 장착 해제 함수
```c++
/// <summary>
/// 장착중인 아이템 장비 해제
/// 장비 슬롯과 인벤토리 슬롯간의 drag&drop이나 장비 슬롯의 우클릭에 의해 실행
/// 빈슬롯과의 스왑에서만 실행됨
/// 해제되는 아이템의 아이템 효과들에게 onRemoveRegistration()를 실행시켜 모든 장비의 효과를 해제를 수행 
/// </summary>
/// <param name="__from"></param>
/// <param name="__to"></param>
void AController_Player::unequipItem_Implementation(EEquipmentType __from, int32 __to)
{
	UPWOGameInstance* gameinstance = Cast<UPWOGameInstance>(GetGameInstance());
	/* __to < 0 이면 우클릭으로 장비를 해제한 것이라 findInventoryEmptyIndex()함수로 인벤토리의 empty 슬롯과 스왑 */
	if (__to < 0)
		__to = findInventoryEmptyIndex();
	FName previtemid = equipment_list[__from].item_id;
	if (HasAuthority()) {
		if (GetCharacter()->GetClass()->ImplementsInterface(UInterface_BaseCharacter::StaticClass())) {
			IInterface_BaseCharacter::Execute_ItemEffect_onRemoveRegistration(GetCharacter(), previtemid, __to);
		}
	}
	FInventoryData fromdata = equipment_list[__from];
	if (inventory_list.Contains(__to)) {
		/* __to 위치에 다른 아이템이 있을 경우인데 이 부분은 현재는 절대 실행되지 않음 */
		/* __to 위치에 다른 아이템이 있다면 위젯에서 unequipItem 함수가 아니라 equipItem 함수를 대신 실행함 */
		equipment_list[__from] = inventory_list[__to];
		inventory_list[__to] = fromdata;
	}
	else {
		inventory_list.Add(TTuple<int32, FInventoryData>(__to, fromdata));
		equipment_list.Remove(__from);
	}
	refreshEquipmentSlot(__from);
	if (IsLocalController()) {
		UWidget_ItemSlot* slot_ref = gameinstance->inventory_slot_reference.FindRef(__to);
		if (slot_ref)
			slot_ref->initSlot();
	}
	/* 현재 컨트롤러가 클라이언트에 있으면 서버에서도 동일한 작업을 수행하기 위해 서버함수 실행, 디테일 위젯 열려있으면 삭제 */
	if (HasAuthority() == false) {
		Server_unequipItem(__from, __to);
		if (gameinstance->detail_widget_reference->IsValidLowLevel())
			gameinstance->detail_widget_reference->RemoveFromParent();
	}
}

/// <summary>
/// 서버에서도 클라이언트와 동일한 작업을 수행해서 인벤토리 동기화
/// </summary>
/// <param name="__from"></param>
/// <param name="__to"></param>
void AController_Player::Server_unequipItem_Implementation(EEquipmentType __from, int32 __to)
{
	unequipItem(__from, __to);
}
```
#### 코드 : 장비 슬롯 위젯 프로퍼티 및 UI 업데이트 
```c++
/// <summary>
/// 장비 슬롯 위젯의 프로퍼티를 업데이트 하고 UI를 업데이트
/// </summary>
/// <param name="__type"></param>
void AController_Player::refreshEquipmentSlot_Implementation(EEquipmentType __type)
{
	if (IsLocalController() == false)
		return;
	UPWOGameInstance* gameinstance = Cast<UPWOGameInstance>(GetGameInstance());
	UWidget_ItemSlot* equipment_slot = gameinstance->equipment_slot_reference[__type];
	FInventoryData invendata;
	if (equipment_list.Contains(__type)) {
		invendata = equipment_list[__type];
	}
	else {
		invendata.item_id = "None";
		invendata.count = 0;
	}
	equipment_slot->updateUI(invendata);
}
```
#### 코드 : 아이템 갯수 감소 함수
```c++
/// <summary>
/// 아이템 갯수 감소 함수
/// 아이템의 수를 감소시키고 아이템이 0개가 되면 인벤토리 슬롯과 퀵슬롯에서 제거
/// UI 업데이트도 항상 수행
/// </summary>
/// <param name="__index"></param>
/// <param name="__decrease_num"></param>
void AController_Player::decreseItem_Implementation(int32 __index, int32 __decrease_num)
{
	if (inventory_list.Contains(__index) == false)
		return;
	int32 after_decreased = inventory_list[__index].count - __decrease_num;
	if (after_decreased <= 0) {
		/* 아이템 감소 결과 수가 0개 이하가 되었을 경우 */
		/* inventory_list에서 제거 */
		inventory_list.Remove(__index);
		if (reverse_quickslot_list.Contains(__index)) {
			/* 퀵슬롯에 등록된 아이템일 경우 퀵슬롯에서도 제거 */
			FKey tmpkey = reverse_quickslot_list[__index];
			reverse_quickslot_list.Remove(__index);
			quickslot_list.Remove(tmpkey);
			refreshQuickSlot(tmpkey);
		}
	}
	else {
		/* 감소 후에도 수가 1 이상이면 수를 감소시키고 퀵슬롯 ui 업데이트 */
		inventory_list[__index].count -= __decrease_num;
		if (reverse_quickslot_list.Contains(__index)) {
			refreshQuickSlot(reverse_quickslot_list[__index]);
		}
	}
	if (IsLocalController()) {
		UPWOGameInstance* gameinstance = Cast<UPWOGameInstance>(GetGameInstance());
		/* 인벤토리 슬롯의 레퍼런스가 gameinstance에 존재하는 경우 ( 인벤토리 창이 열려있는 경우 ) 인벤토리 슬롯 UI 업데이트 */
		if (gameinstance->inventory_slot_reference.Contains(__index))
			gameinstance->inventory_slot_reference[__index]->initSlot();
		/* 아이템이 삭제되었을 때 디테일 위젯 열려있으면 삭제 */
		if (after_decreased > 0 && gameinstance->detail_widget_reference->IsValidLowLevel())
			gameinstance->detail_widget_reference->RemoveFromParent();
	}
}
```

# 락온타게팅 시스템

![락온시스템(sm)](https://user-images.githubusercontent.com/12960463/117236527-d09ede80-ae63-11eb-9b9f-d41ccebae083.gif)

내적을 이용하여 플레이어가 보는 방향에서 가장 가까운 적을 타게팅하며, 휠을 통해 우측, 좌측에 있는 적으로 타게팅을 선택해서 전환할 수 있음

![락온 릴리즈(sm)](https://user-images.githubusercontent.com/12960463/117236828-836f3c80-ae64-11eb-8c3d-ef1bade18afc.gif)

일정 시간 적이 시야에서 사라지면 타게팅을 해제하고 체력바 위젯을 숨김

#### 코드 : 타게팅 대상 서치 
```c++
/// <summary>
/// 타게팅 대상을 선정
/// ABaseEnemy를 상속하는 대상 중 카메라 각과 최소각을 이루는 대상을 선택
/// </summary>
/// <returns> 타게팅 대상 </returns>
AActor* AController_Player::findLockOnTarget()
{
	TArray<AActor*> founded;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABaseEnemy::StaticClass(), founded);
	float min_angle = -1;
	AActor* result = nullptr;
	for (AActor* i : founded) {
		/* 거리 1000 이상이면 타게팅 후보에서 제외 */
		if (GetPawn()->GetDistanceTo(i) > 2000)
			continue;
		/* 라인트레이스 해서 히트가 발생하면 후보에서 제외*/
		FHitResult hit_result;
		FVector cam_location = follow_cam->camera->GetComponentLocation();
		FVector target_location = i->GetActorLocation();
		bool is_hit = UKismetSystemLibrary::LineTraceSingle(GetWorld(), cam_location, target_location, ETraceTypeQuery::TraceTypeQuery1, false, founded, EDrawDebugTrace::Type::None, hit_result, true);
		if (is_hit)
			continue;

		/* 액터간 lookat 벡터와 카메라 벡터를 내적한 결과가 가장 큰 ( 카메라와 이루는 각이 가장 작은 ) 액터를 선택 */
		FVector lookat_vector = UKismetMathLibrary::GetDirectionUnitVector(cam_location, target_location);
		FVector camera_vector = UKismetMathLibrary::Normal(UKismetMathLibrary::GetForwardVector(follow_cam->camera->GetComponentRotation()), 0.0001f);
		float angle = UKismetMathLibrary::Dot_VectorVector(lookat_vector, camera_vector);
		if (angle > min_angle) {
			min_angle = angle;
			result = i;
		}
	}
	return result;
}
```

#### 코드 : 타게팅 전환 대상 서치
```c++
AActor* AController_Player::changeLockOnTarget(float __direction)
{
	FVector camera_location = follow_cam->camera->GetComponentLocation();
	FVector prev_look_vector = UKismetMathLibrary::GetDirectionUnitVector(camera_location, follow_cam->look_target->GetActorLocation());
	FRotator camera_rotation = follow_cam->camera->GetComponentRotation();
	FVector camera_forward_vector = UKismetMathLibrary::GetForwardVector(camera_rotation);
	TArray<AActor*> founded;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABaseEnemy::StaticClass(), founded);
	float min_angle = -1;
	AActor* result = nullptr;
	for (AActor* i : founded) {
		if (follow_cam->look_target == i)
			continue;
		if (GetPawn()->GetDistanceTo(i) > 2000)
			continue;
		FHitResult hit_result;
		FVector target_location = i->GetActorLocation();
		bool is_hit = UKismetSystemLibrary::LineTraceSingle(GetWorld(), camera_location, target_location, ETraceTypeQuery::TraceTypeQuery1, false, founded, EDrawDebugTrace::Type::None, hit_result, true);
		if (is_hit)
			continue;
		FVector target_look_vector = UKismetMathLibrary::GetDirectionUnitVector(camera_location, target_location);
		float angle = UKismetMathLibrary::Dot_VectorVector(target_look_vector, camera_forward_vector);
		/* angle 이 0.6 이하라면 화면에 적이 없다고 판단하고 후보에서 제외, min_angle 과 비교하여 최소각을 가진 타겟을 찾음 */
		if (angle > 0.6f && angle > min_angle) {
			FVector rotated_target_prev_lookat_vector = UKismetMathLibrary::Quat_UnrotateVector(camera_rotation.Quaternion(), target_look_vector - camera_forward_vector);
			float prev_target_angle;
			float dummy;
			UKismetMathLibrary::GetYawPitchFromVector(rotated_target_prev_lookat_vector, prev_target_angle, dummy);
			/* 현재 타겟과 이전 타겟과의 좌우 방향 관계가 입력된 __direction 과 일치 하는지 검사 */
			if (prev_target_angle * __direction > 10) {
				min_angle = angle;
				result = i;
			}
		}
	}
	return result;
}
```
# 머테리얼 and 이펙트
## 피 이펙트
- 피 이펙트

![피 이펙트5](https://user-images.githubusercontent.com/12960463/125896956-c1326a5e-e1e3-4b71-a365-386f516fa637.gif)
![피 이펙트6](https://user-images.githubusercontent.com/12960463/125898091-85bbceac-67fa-4425-a673-f9902c593faa.gif)
- 피 머테리얼

![image](https://user-images.githubusercontent.com/12960463/125896969-22835adc-985c-4fc2-971a-990a39f619cc.png)

#### 코드 : 피 이펙트 스폰 ( attackEvent 에서 충돌하는 매 프레임 스폰 )
```c++
void ABaseCharacter::attackEvent_Implementation(AActor* __hit_actor, FHitResult __hit_result) {
/* 피 이펙트 스폰 */
...
	if (hit_actor_is_dodge == false) {
		UPWOGameInstance* gameinstance = Cast<UPWOGameInstance>(GetGameInstance());
		UNiagaraComponent* blood_effect = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), gameinstance->blood_effect, __hit_result.ImpactPoint, __hit_result.ImpactNormal.ToOrientationRotator(), FVector::OneVector, true, true, ENCPoolMethod::AutoRelease, true);
	}
...
}
```
#### 코드 : 피 이펙트 스폰 ( applyDamage 에서 넉백벡터의 역방향으로 스폰 )
```c++
void ABaseCharacter::applyDamage_Multicast_Exec_Implementation(FName __target_damage_id, AActor* damage_causer, FName __hit_bone_name) {
...
/* 피 나이아가라 이펙트 스폰 */
	UNiagaraComponent* blood_effect = UNiagaraFunctionLibrary::SpawnSystemAttached(gameinstance->blood_effect, GetMesh(), __hit_bone_name, FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::Type::SnapToTarget, true, true, ENCPoolMethod::AutoRelease, true);
	if (blood_effect) {
		/* 피 이펙트를 rotation을 absolute 로 바꾸고 회전이 적용된 넉백 벡터의 반대 방향으로 world rotation 적용, location은 표면 트레스한 위치로 옮김 */
		blood_effect->SetUsingAbsoluteRotation(true);
		FRotator blood_rot = rotated_vector.Rotation();
		blood_rot.Pitch *= -1;
		blood_rot.Yaw += 180;
		blood_effect->SetWorldRotation(blood_rot);
	}
...	
}
```
## 히트 궤적 이펙트
![히트 부위 데칼 이펙트5](https://user-images.githubusercontent.com/12960463/126039652-78323ee8-2668-4d8b-b8ad-08b254bdae6c.gif)
![히트 부위 데칼 이펙트6](https://user-images.githubusercontent.com/12960463/126039553-13eb8083-539c-45ee-9fc0-b78421bf91c8.gif)
![히트 부위 데칼 이펙트7](https://user-images.githubusercontent.com/12960463/126039555-adfaf58d-3b76-4c93-b365-567b838a076f.gif)

- 번쩍이는 데칼을 히트 부위에 히트 노말의 역방향으로 생성하고 Roll 을 트레이스 궤적의 방향과 일치시킴
#### 데칼 머테리얼 노드
![image](https://user-images.githubusercontent.com/12960463/126039604-444c0b6b-cace-47e7-85d3-b30ad89cbcbe.png)

## 화면 흔들림
![흔들림](https://user-images.githubusercontent.com/12960463/127766828-d40ed212-e275-42df-9210-380d8f3b1ac1.gif)
![흔들림3](https://user-images.githubusercontent.com/12960463/127766871-815480dd-e57a-414c-8c2b-e9e082863633.gif)

- 타격, 피격, 방어, 지면과 충돌시 화면 흔들림 효과 재생

#### 코드 : camera shake 클래스 코드 (헤더)
``` c++
#include "CoreMinimal.h"
#include "Camera/CameraShake.h"
#include "CameraShake_Hit.generated.h"
UCLASS()
class CYPERARENA_API UCameraShake_Hit : public UMatineeCameraShake
{
	GENERATED_BODY()
	
public:
	UCameraShake_Hit();
};
```
#### 코드 : camera shake 클래스 코드 (cpp)
``` c++
#include "CameraShake_Hit.h"
UCameraShake_Hit::UCameraShake_Hit()
{
    OscillationDuration = 0.25f;
    OscillationBlendInTime = 0.0f;
    OscillationBlendOutTime = 0.05f;

    RotOscillation.Pitch.Amplitude = FMath::RandRange(2, 4);
    RotOscillation.Pitch.Frequency = FMath::RandRange(10, 15);

    RotOscillation.Pitch.Amplitude = FMath::RandRange(2, 4);
    RotOscillation.Pitch.Frequency = FMath::RandRange(10, 15);
}
```
#### 코드 : 에어본 상태에서 지면과 충돌시 먼지이펙트와 사운드 이펙트, 카메라 쉐이크 이펙트를 실행하는 코드 ( tick 함수 코드 일부 )
``` c++
void ABaseCharacter::Tick(float DeltaTime){
...
 	//에어본시 피지컬 애니메이션의 관절 각 운동량 조절
	// 중력 트레이스와 예측 트레이스를 투 개를 이용해 매 틱 충돌판정
	if (character_state == ECharacterState::Airbone) {
		FVector hit_normal;
		FVector hit_location;
		if (airbone_HitChk(GetVelocity(), hit_normal, hit_location)) {
			setCharacterState(ECharacterState::Ragdoll);
			/* 충돌이 감지되었으면 이펙트 실행 */
			if (UKismetSystemLibrary::IsDedicatedServer(this) == false) {
				/* 카메라 쉐이크 */
				if (GetController()->IsValidLowLevel() && GetController()->IsA<APlayerController>()) {
					Cast<APlayerController>(GetController())->ClientPlayCameraShake(UCameraShake_Hit::StaticClass(), GetVelocity().Size() / 1000);
				}
				/* 먼지 이펙트 & 사운드 */
				UPWOGameInstance* gameinstance = Cast<UPWOGameInstance>(GetGameInstance());
				FRotator effect_rotator = hit_normal.ToOrientationRotator();
				effect_rotator.Pitch -= 90;
				UNiagaraComponent* blood_effect = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), gameinstance->ground_dust_effect, hit_location, effect_rotator, FVector::OneVector, true, true, ENCPoolMethod::AutoRelease, true);
				UGameplayStatics::SpawnSoundAtLocation(this, gameinstance->sq_ground_hit, GetActorLocation());
			}
		}
	}
	...
}
```



## 에너지 필드
![에너지필드 머테리얼](https://user-images.githubusercontent.com/12960463/125792608-6694d8de-08fd-46ac-bb9a-940621f531e0.gif)
- 머테리얼 노드

![image](https://user-images.githubusercontent.com/12960463/125792690-92a1cd7f-1f99-4d54-be14-33860c850f84.png)

# 애니메이션 시스템
![디렉셔널 로코모션 블렌딩](https://user-images.githubusercontent.com/12960463/117234762-908a2c80-ae60-11eb-87a2-aad9e5d3ae29.gif)

- 기울이는 모션도 더 자연스러운 모션을 위해 애니메이션을 블렌딩 한 결과
- 180도 와 180도 에서의 블렌딩을 자연스럽게 하기 위해 백워드 디렉션을 함께 구해서 사용
- 90 ~ -180, 90 ~ 180도 구간에서는 백워드 디렉션을 구해 -90 ~ 90도로 변환한 뒤 lean 계산

![디렉셔널 로코모션 블렌딩2](https://user-images.githubusercontent.com/12960463/117235046-0b534780-ae61-11eb-93de-7e2f9104c2bb.gif)

걷기도 자연스럽게
스피드에 따라 lean 수치 조정

![디렉셔널 로코모션 블렌딩3(린)](https://user-images.githubusercontent.com/12960463/117235254-59684b00-ae61-11eb-85f1-f7707509fbd7.gif)

착지모션과 로코모션을 블렌딩 해서 착지의 불안정한 모션을 구현하면서, 걸리적거리지 않는 시원시원한 조작감 구현

## 이동 방향 기반 캐릭터 rotation 회전 시스템
![스프린트 모션](https://user-images.githubusercontent.com/12960463/127928662-dfbde943-3adf-4a7c-a3b1-9318dd3c6796.gif)

( 언리얼의 character movement 의 orient rotation to movement 기능을 사용할 경우의 애니메이션 )
- 캐릭터가 항상 이동방향으로 향해있기 때문에 lean 이 제대로 적용되지 않음

![스프린트 모션2](https://user-images.githubusercontent.com/12960463/127929096-5fb82acd-9b81-433b-adee-3431583152ca.gif)

( 이동 방향으로 캐릭터를 보간하며 회전한 경우의 애니메이션  )
- 상대적인 이동 방향을 구할 수 있어 올바른 lean 각도가 계산되기 때문에 훨씬 자연스러운 모션 생성

# 카메라 위치 조정

![카메라 위치 조정](https://user-images.githubusercontent.com/12960463/117242789-e8c92a80-ae70-11eb-9c74-ab0ccb0a2107.gif)

캐릭터가 등을 벽에 대고 있어도 시야 확보를 보장하는 카메라 위치를 자동으로 찾아줌

#### 코드 : 카메라 위치 조정 코드 및 타게팅 코드
```c++
void AFollowCam_Base::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
    if (follow_target->IsValidLowLevel() == false)
        return;
    FVector trace_start = follow_target->GetActorLocation();
    trace_start.Z += 50;
    FVector trace_end = spring_arm->GetRightVector() * 60 + trace_start;

    const TArray<AActor*> ignore;
    FHitResult hit_result;
    /* 캐릭터의 카메라 기준 우측으로 LineTraceSingle 을 수행해 벽이 감지되면 카메라를 충돌 거리만큼 좌측으로 옮김 */
    UKismetSystemLibrary::LineTraceSingle(this, trace_start, trace_end, ETraceTypeQuery::TraceTypeQuery2, false, ignore, EDrawDebugTrace::Type::None, hit_result, true);
    if (hit_result.bBlockingHit) {
        location_offset.Y = hit_result.Distance - 60;
    }
    else {
        location_offset.Y = 0;
    }

    FVector target_location = follow_target->GetActorLocation() + follow_target->GetActorRightVector() * location_offset.Y;
    FRotator target_rotation;
    /* 락온 타게팅 상태라면 락온 타겟의 look at 방향으로 controlRotation 을 대체 */
    if (is_lock_on) {
        target_rotation = UKismetMathLibrary::FindLookAtRotation(camera->GetComponentLocation(), look_target->GetActorLocation());
        GetWorld()->GetFirstPlayerController()->SetControlRotation(UKismetMathLibrary::FindLookAtRotation(follow_target->GetActorLocation(), look_target->GetActorLocation()));
    }
    else {
        target_rotation = GetWorld()->GetFirstPlayerController()->GetControlRotation();
    }
    SetActorLocationAndRotation(target_location, target_rotation);
}
```

# 데모 레벨

일반 몬스터와 보스 몬스터가 있는 매우 짧은 데모 레벨 구현 완료

![데모레벨초입](https://user-images.githubusercontent.com/12960463/118310475-a1563480-b529-11eb-80b6-7e6bdc3e4f20.gif)
![데모레벨전투](https://user-images.githubusercontent.com/12960463/118310458-9bf8ea00-b529-11eb-8ba0-9f58d192846f.gif)
![보스 Idle](https://user-images.githubusercontent.com/12960463/118364188-60662a80-b5d2-11eb-89d9-93200f5138b2.gif)
![보스 공격 패턴1](https://user-images.githubusercontent.com/12960463/118364184-58a68600-b5d2-11eb-9104-50a2fade0bc2.gif)
![보스 공격 패턴2](https://user-images.githubusercontent.com/12960463/118811397-ec849480-b8e7-11eb-90a1-5224a5e2004c.gif)
![보스 공격 패턴1-2(sm)](https://user-images.githubusercontent.com/12960463/118811404-ee4e5800-b8e7-11eb-9220-d93b082b2527.gif)

# 주요 파일
## c++ 파일
- 최상위 베이스 캐릭터 클래스 파일
  - https://github.com/gtrash12/Unreal-project/blob/main/Source/CyperArena/BaseCharacter.h
  - https://github.com/gtrash12/Unreal-project/blob/main/Source/CyperArena/BaseCharacter.cpp
- 베이스 캐릭터를 상속받는 최상위 BaseEnemy 클래스 파일
  - https://github.com/gtrash12/Unreal-project/blob/main/Source/CyperArena/Public/BaseEnemy.h
  - https://github.com/gtrash12/Unreal-project/blob/main/Source/CyperArena/Private/BaseEnemy.cpp
- 블루프린트와 c++ 모두에서 사용하는 데이터 타입 선언 파일
  - https://github.com/gtrash12/Unreal-project/blob/main/Source/CyperArena/CustomData.h
  - https://github.com/gtrash12/Unreal-project/blob/main/Source/CyperArena/Public/CustomEnums.h
- 플레이어 컨트롤러
  - https://github.com/gtrash12/Unreal-project/blob/main/Source/CyperArena/Public/Controller_Player.h
  - https://github.com/gtrash12/Unreal-project/blob/main/Source/CyperArena/Private/Controller_Player.cpp
- 카메라
  - https://github.com/gtrash12/Unreal-project/blob/main/Source/CyperArena/Public/FollowCam_Base.h
  - https://github.com/gtrash12/Unreal-project/blob/main/Source/CyperArena/Private/FollowCam_Base.cpp
- c++ 인터페이스 파일
  - https://github.com/gtrash12/Unreal-project/blob/main/Source/CyperArena/Public/Interface_BaseCharacter.h
  - https://github.com/gtrash12/Unreal-project/blob/main/Source/CyperArena/Public/Interface_PlayerController.h
  - https://github.com/gtrash12/Unreal-project/blob/main/Source/CyperArena/Public/Interface_AI.h
  - https://github.com/gtrash12/Unreal-project/blob/main/Source/CyperArena/Public/Interface_ItemEffect.h
  - https://github.com/gtrash12/Unreal-project/blob/main/Source/CyperArena/Public/Interface_General.h
- anim notify state ( 충돌 판정, 회피 판정 )
  - https://github.com/gtrash12/Unreal-project/blob/main/Source/CyperArena/Public/NS_Attack_Weapon_Collision_Bind.h 
  - https://github.com/gtrash12/Unreal-project/blob/main/Source/CyperArena/Private/NS_Attack_Weapon_Collision_Bind.cpp
  - https://github.com/gtrash12/Unreal-project/blob/main/Source/CyperArena/Public/NS_Attack_Weapon_2Sock_Trace.h
  - https://github.com/gtrash12/Unreal-project/blob/main/Source/CyperArena/Private/NS_Attack_Weapon_2Sock_Trace.cpp
  - https://github.com/gtrash12/Unreal-project/blob/main/Source/CyperArena/Public/NS_Attack_1Sock_Trace.h
  - https://github.com/gtrash12/Unreal-project/blob/main/Source/CyperArena/Private/NS_Attack_1Sock_Trace.cpp
  - https://github.com/gtrash12/Unreal-project/blob/main/Source/CyperArena/Public/NS_Attack_2Sock_Trace.h
  - https://github.com/gtrash12/Unreal-project/blob/main/Source/CyperArena/Private/NS_Attack_2Sock_Trace.cpp
  - https://github.com/gtrash12/Unreal-project/blob/main/Source/CyperArena/Public/NS_Dodge.h
  - https://github.com/gtrash12/Unreal-project/blob/main/Source/CyperArena/Private/NS_Dodge.cpp
- 커스텀 함수 라이브러리
  - https://github.com/gtrash12/Unreal-project/blob/main/Source/CyperArena/Public/CustomBPFunctionLibrary.h
  - https://github.com/gtrash12/Unreal-project/blob/main/Source/CyperArena/Private/CustomBPFunctionLibrary.cpp
- 게임 인스턴스 ( 위젯, 데이터 테이블 관리 )
  - https://github.com/gtrash12/Unreal-project/blob/main/Source/CyperArena/Public/PWOGameInstance.h
  - https://github.com/gtrash12/Unreal-project/blob/main/Source/CyperArena/Private/PWOGameInstance.cpp
- 위젯
  - https://github.com/gtrash12/Unreal-project/blob/main/Source/CyperArena/Public/Widget_ItemSlot.h
  - https://github.com/gtrash12/Unreal-project/blob/main/Source/CyperArena/Private/Widget_ItemSlot.cpp
  - https://github.com/gtrash12/Unreal-project/blob/main/Source/CyperArena/Public/Widget_Detail.h
  - https://github.com/gtrash12/Unreal-project/blob/main/Source/CyperArena/Private/Widget_Detail.cpp

# 조작법
- 이동 : WASD
- 공격 : 좌클릭
- 콤보공격 : 좌클릭 연타
- 찌르기 : 우클릭
- 대쉬공격 : 달리는 중 좌클릭 or 구르기 중 좌클릭
- 달리기 : Shift
- 구르기, 일어서기 : Spacebar
- 점프 : Shift + Spacebar
- 걷기 : ctrl
- 타게팅 : 마우스 휠 버튼
- 락온 타게팅 타겟 전환 : 마우스 휠 회전
