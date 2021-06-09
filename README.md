# Unreal-project
언리얼엔진4로 개발한 3D 액션 MMORPG

# 영상
[![썸네일](https://user-images.githubusercontent.com/12960463/121377323-34de2200-c97d-11eb-9992-825a908c26d4.jpg)](https://youtube.com/playlist?list=PLAGMcKKJuvziGTJumbg1EP94LcSFXKdgT)
클릭하면 유튜브 영상 링크

# 주요 파일
## c++ 파일
- 최상위 베이스 캐릭터 클래스 파일
  - https://github.com/gtrash12/Unreal-project/blob/main/Source/CyperArena/BaseCharacter.h
  - https://github.com/gtrash12/Unreal-project/blob/main/Source/CyperArena/BaseCharacter.cpp
- 블루프린트와 c++ 모두에서 사용하는 데이터 타입 선언 파일
  - https://github.com/gtrash12/Unreal-project/blob/main/Source/CyperArena/CustomData.h
- c++ 인터페이스 파일
  - https://github.com/gtrash12/Unreal-project/blob/main/Source/CyperArena/Public/Interface_BaseCharacter.h
  - https://github.com/gtrash12/Unreal-project/blob/main/Source/CyperArena/Public/Interface_PlayerController.h
- anim notify state c++ 파일
  - https://github.com/gtrash12/Unreal-project/blob/main/Source/CyperArena/Public/NS_Attack_Weapon_Collision_Bind.h 
  - https://github.com/gtrash12/Unreal-project/blob/main/Source/CyperArena/Private/NS_Attack_Weapon_Collision_Bind.cpp
  - https://github.com/gtrash12/Unreal-project/blob/main/Source/CyperArena/Public/NS_Attack_Weapon_2Sock_Trace.h
  - https://github.com/gtrash12/Unreal-project/blob/main/Source/CyperArena/Private/NS_Attack_Weapon_2Sock_Trace.cpp
  - https://github.com/gtrash12/Unreal-project/blob/main/Source/CyperArena/Public/NS_Attack_1Sock_Trace.h
  - https://github.com/gtrash12/Unreal-project/blob/main/Source/CyperArena/Private/NS_Attack_1Sock_Trace.cpp
  - https://github.com/gtrash12/Unreal-project/blob/main/Source/CyperArena/Public/NS_Attack_2Sock_Trace.h
  - https://github.com/gtrash12/Unreal-project/blob/main/Source/CyperArena/Private/NS_Attack_2Sock_Trace.cpp
## 블루프린트 파일
- c++ 베이스 캐릭터를 상속받는 플레이어 캐릭터
  - https://github.com/gtrash12/Unreal-project/blob/main/Content/Blueprint/BP_PlayerCharacter_cpp.uasset
- c++ 베이스 캐릭터를 상속받는 적(AI) 캐릭터
  - https://github.com/gtrash12/Unreal-project/blob/main/Content/Blueprint/bpbp_cpp.uasset
-  블루프린트 베이스 캐릭터
  - https://github.com/gtrash12/Unreal-project/blob/main/Content/Blueprint/BP_BaseCharacter.uasset
- 블루프린트 베이스 캐릭터를 상속받는 플레이어 캐릭터
  - https://github.com/gtrash12/Unreal-project/blob/main/Content/Blueprint/BP_PlayerCharacter.uasset
- 블루프린트 베이스 캐릭터를 상속받는 적(AI) 캐릭터
  - https://github.com/gtrash12/Unreal-project/blob/main/Content/Blueprint/bpbp.uasset
-  플레이어 콘트롤러 블루프린트
  - https://github.com/gtrash12/Unreal-project/blob/main/Content/Blueprint/Controller_PlayerCharacter_cpp.uasset
- 모든 액션의 데미지 데이터를 정의한 테이블
  - https://github.com/gtrash12/Unreal-project/blob/main/Content/Blueprint/DamageDataTable.uasset
- 모든 휴머노이드 형식의 액터들이 상속받아 override만으로 쉽게 애니메이션을 구현할 수 있는 베이스 애니메이션 블루프린트
  - https://github.com/gtrash12/Unreal-project/blob/main/Content/Blueprint/Player_Anim_BP.uasset

- 그외 접두사에 따라
  - AN : 애님 노티파이
  - NS : 애님 노티파이 스테이트
  - EQS : 환경 쿼리 시스템
  - Task : 비헤이비어 트리의 태스크

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
- 락온 타게팅 오른쪽에 위치한 타겟으로 전환 : 마우스 휠 위로 회전
- 락온 타게팅 왼쪽에 위치한 타겟으로 전환 : 마우스 휠 아래로 회전


# 핵심 기능
## 네트워크 래그돌 동기화 시스템
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

#### 코드 : Tick 함수 내에서 래그돌 동기화 함수 매 프레임 실행시키는 코드 ( Tick 함수 코드 일부 )
```
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
```
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

```

#### 코드 : 동기화 함수 ( 스탠드얼론 or 데디케이티드 서버일 때 )
```
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
```
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
	float ease_alpha = replication_delay_count / last_replication_delay;
	FVector predicted_location = UKismetMathLibrary::VEase(prev_ragdoll_server_location, ragdoll_server_location, ease_alpha, EEasingFunc::Linear);
	ragdoll_physics_handle->SetTargetLocation(predicted_location);
	stickToTheGround(ragdoll_server_location);
	last_ragdoll_server_location = ragdoll_server_location;
}
```


## 전방향 피격모션 & 발동작 블렌딩

![전방향피격모션(sm)](https://user-images.githubusercontent.com/12960463/117236043-dea02f80-ae62-11eb-9aad-c63582fff7f7.gif)

전,후,좌,우의 4방향 피격모션에 로코모션을 블렌딩해서 넉백시 밀려나면서도 균형을 잡으려는 매우 자연스러운 모션 구현

![몬스터 사망(sm)](https://user-images.githubusercontent.com/12960463/117236948-cc26f580-ae64-11eb-91d2-6a16058f0c3d.gif)

몬스터가 사망할 때 넉백의 운동량이 그대로 사망 래그돌 모션에 적용되어 넉백 방향으로 쓰러져 죽는 모션 구현

![히트 본 덜렁 sm](https://user-images.githubusercontent.com/12960463/121346210-c8edc080-c960-11eb-9f05-f90f96262203.gif)

슈퍼아머인 적은 경직 hit 애니메이션을 실행하지 않기 때문에 기존 애니메이션 위에 피격 부위만 덜렁거리는 피지컬 애니메이션으로 구현

#### 코드 : applyDamage 코드 ( 피지컬 애니메이션, 넉백 )
```
/// <summary>
/// applyDamage_Multicast의 실제 구현 (블루프린트에서 오버라이딩 할 수 있게 하기 위함)
/// 피격 액터의 durability_level 이 공격의 durability_level 보다 크면 넉백과 에어본, 경직을 무시하고 히트 부위에 피지컬 애니메이션 실행
/// 이외의 상황에서 damage_id를 통해 DamageData를 구하고 해당 DamageData의 넉백 벡터와 offset, 공격 액터의 방향과 현재 액터의 위치 관계에 따라 넉백 벡터를 회전해서 적용
/// </summary>
/// <param name="__target_damage_id">데미지 id</param>
/// <param name="damage_causor">공격한 액터</param>
/// <param name="__hit_bone_name">피격된 본</param>
void ABaseCharacter::applyDamage_Multicast_Exec_Implementation(FName __target_damage_id, AActor* damage_causor, FName __hit_bone_name) {
	FdamageData target_damage_data;
	if (GetMesh()->GetWorld()->GetFirstPlayerController()->GetClass()->ImplementsInterface(UInterface_PlayerController::StaticClass()))
	{
		IInterface_PlayerController::Execute_findDamageData(GetMesh()->GetWorld()->GetFirstPlayerController(), __target_damage_id, target_damage_data);
	}
	if (durability_level >= target_damage_data.durability_level) {
		// 슈퍼아머 상태에서 히트시 히트 부위 덜렁거리는 피지컬 애니메이션 초기화부
		if (character_state == ECharacterState::Walk_and_Jump && UKismetSystemLibrary::IsDedicatedServer(this) == false) {
			GetMesh()->SetAllBodiesBelowSimulatePhysics(__hit_bone_name, true);
			if (hit_bone_physics_weight_map.Contains(__hit_bone_name)) {
				hit_bone_physics_weight_map[__hit_bone_name] = 0.5f;
			}
			else {
				hit_bone_physics_weight_map.Add(TTuple<FName, float>(__hit_bone_name, 0.5f));
			}
			GetMesh()->AddImpulse(damage_causor->GetActorForwardVector() * 1200, __hit_bone_name, true);
		}
		//끝
		animation_Sound_Multicast(nullptr, sq_hit);
		return;
	}
	// 넉백 벡터를 넉백타입과 방향에 맞게 회전
	FVector rotated_vector;
	FVector rotated_offset = UKismetMathLibrary::Quat_RotateVector(damage_causor->GetActorRotation().Quaternion(), target_damage_data.knock_back_offset);
	FVector knock_back_point_vector = damage_causor->GetActorLocation() + rotated_offset;
	if (target_damage_data.knock_back_type == EKnockBackType::Directional)
		rotated_vector = UKismetMathLibrary::Quat_RotateVector(damage_causor->GetActorRotation().Quaternion(), target_damage_data.knock_back);
	else if(target_damage_data.knock_back_type == EKnockBackType::RadialXY) {
		FRotator rotate_quat = UKismetMathLibrary::FindLookAtRotation(knock_back_point_vector, GetActorLocation());
		rotate_quat.Pitch = 0;
		rotate_quat.Roll = 0;
		rotated_vector = UKismetMathLibrary::Quat_RotateVector(rotate_quat.Quaternion(), target_damage_data.knock_back);
	}
	else if (target_damage_data.knock_back_type == EKnockBackType::RadialXYDistanceReverse) {
		FRotator rotate_quat = UKismetMathLibrary::FindLookAtRotation(knock_back_point_vector, GetActorLocation());
		rotate_quat.Pitch = 0;
		rotate_quat.Roll = 0;
		rotated_vector = UKismetMathLibrary::Quat_RotateVector(rotate_quat.Quaternion(), target_damage_data.knock_back);
		float distance = UKismetMathLibrary::Vector_Distance(knock_back_point_vector, GetActorLocation());
		rotated_vector.X *= distance;
		rotated_vector.Y *= distance;
	}
	else {
		rotated_vector = UKismetMathLibrary::Quat_RotateVector(damage_causor->GetActorRotation().Quaternion(), target_damage_data.knock_back);
	}
	UAnimMontage* hit_anim = nullptr;
	selectHitAnimation(rotated_vector, hit_anim);
	animation_Sound_Multicast(hit_anim, sq_hit);

	rotate_interp_time = 0;
	if (character_state == ECharacterState::Ragdoll) {
		ragdollGetUp();
		setCharacterState(ECharacterState::Airbone);
	}
	FVector hitnormal;
	if (airbone_HitChk(rotated_vector, hitnormal)) {
		float rotated_std = rotated_vector.Size();
		rotated_vector = hitnormal * rotated_std;
	}
	if (target_damage_data.target_control == ETargetControlType::None) {
		if (character_state == ECharacterState::Walk_and_Jump) {
			applyKnock_Back(rotated_vector);
		}
		else {
			LaunchCharacter(UKismetMathLibrary::MakeVector(rotated_vector.X * 2 , rotated_vector.Y * 2, rotated_vector.Z), true, true);
		}
	}
	else if (target_damage_data.target_control == ETargetControlType::Ragdoll) {
		knock_back_unit_vector = FVector::ZeroVector;
		if (network_owner_type == ENetworkOwnerType::RemoteAI)
			current_velocty = FVector::ZeroVector;
		knock_back_speed = 0;
		if (is_on_sprint)
			GetCharacterMovement()->MaxWalkSpeed = sprint_speed;
		else
			GetCharacterMovement()->MaxWalkSpeed = walk_speed;
		GetCharacterMovement()->MaxAcceleration = 2048.0f;
		ConsumeMovementInputVector();
		LaunchCharacter(UKismetMathLibrary::MakeVector(rotated_vector.X * 2, rotated_vector.Y * 2, rotated_vector.Z), true, true);
		GetWorld()->GetTimerManager().SetTimerForNextTick(FTimerDelegate::CreateLambda([&, rotated_vector]() {
			//UKismetSystemLibrary::PrintString(this, rotated_vector.ToString());
			airboneStart(rotated_vector);
			}));
	}
}
```

#### 코드 : 넉백 프로세스 ( 틱 함수 내에서 매 틱 실행 )
```
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
```
/// <summary>
/// 히트 본 덜렁거리는 피지컬 애니메이션 적용 프로세스 ( 매 틱 실행 )
/// TMap 타입의 hit_bone_physics_weight_map 에 원소가 있다면 해당 원소의 밸류를 매 틱 감소시키고 해당 본의 PhysicsBlendWeight 를 밸류 값으로 업데이트
/// 원소의 밸류가 0이하가 되면 해당 본의 피직스 시뮬레이션을 종료하고 hit_bone_physics_weight_map 에서 제거
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
	for (auto i : hit_bone_physics_weight_map) {
		i.Value -= d_time;
		hit_bone_physics_weight_map[i.Key] -= d_time * 2;
		if (i.Value <= 0) {
			if (hit_bone_physics_weight_map.Num() == 1) {
				GetMesh()->SetSimulatePhysics(false);
			}
			else {
				GetMesh()->SetAllBodiesBelowSimulatePhysics(i.Key, false);
			}
			hit_bone_physics_weight_map.Remove(i.Key);
		}
		else {
			if (GetMesh()->IsSimulatingPhysics(i.Key) == false)
				GetMesh()->SetAllBodiesBelowSimulatePhysics(i.Key, true);
			GetMesh()->SetAllBodiesBelowPhysicsBlendWeight(i.Key, hit_bone_physics_weight_map[i.Key]);
		}
	}
}
```

## 발 IK
![발IK](https://user-images.githubusercontent.com/12960463/117233132-7d299200-ae5d-11eb-8fdf-ce9a459c60a6.gif)
PowerIK 플러그인 사용

## 디렉셔널 로코모션 & 기울이기 & 블렌딩
![디렉셔널 로코모션 블렌딩](https://user-images.githubusercontent.com/12960463/117234762-908a2c80-ae60-11eb-87a2-aad9e5d3ae29.gif)

- 엉덩이의 방향이 매우 자유분방한 모션을 자연스럽게 블렌딩하는 것은 꽤 힘든 작업이었음
- 기울이는 모션도 더 자연스러운 모션을 위해 애니메이션을 블렌딩 한 결과
- 180도 와 180도 에서의 블렌딩을 자연스럽게 하기 위해 백워드 디렉션을 함께 구해서 사용
- 90 ~ -180, 90 ~ 180도 구간에서는 백워드 디렉션을 구해 -90 ~ 90도로 변환한 뒤 린 계산

![디렉셔널 로코모션 블렌딩2](https://user-images.githubusercontent.com/12960463/117235046-0b534780-ae61-11eb-93de-7e2f9104c2bb.gif)

걷기도 자연스럽게
스피드에 따라 lean 수치 조정

![디렉셔널 로코모션 블렌딩3(린)](https://user-images.githubusercontent.com/12960463/117235254-59684b00-ae61-11eb-85f1-f7707509fbd7.gif)

착지모션과 로코모션을 블렌딩 해서 착지의 불안정한 모션을 구현하면서, 걸리적거리지 않는 시원시원한 조작감 구현

## 락온타게팅 시스템

![락온시스템(sm)](https://user-images.githubusercontent.com/12960463/117236527-d09ede80-ae63-11eb-9b9f-d41ccebae083.gif)

내적을 이용하여 플레이어가 보는 방향에서 가장 가까운 적을 타게팅하며, 휠을 통해 우측, 좌측에 있는 적으로 타게팅을 선택해서 전환할 수 있음

![락온 릴리즈(sm)](https://user-images.githubusercontent.com/12960463/117236828-836f3c80-ae64-11eb-8c3d-ef1bade18afc.gif)

일정 시간 적이 시야에서 사라지면 타게팅을 해제하고 체력바 위젯을 숨김

## 카메라 위치 조정

![카메라 위치 조정](https://user-images.githubusercontent.com/12960463/117242789-e8c92a80-ae70-11eb-9c74-ab0ccb0a2107.gif)

캐릭터가 등을 벽에 대고 있어도 시야 확보를 보장하는 카메라 위치를 자동으로 찾아줌

## 데모 레벨 (WIP)

일반 몬스터와 보스 몬스터가 있는 매우 짧은 데모 레벨 구현중 

![데모레벨초입](https://user-images.githubusercontent.com/12960463/118310475-a1563480-b529-11eb-80b6-7e6bdc3e4f20.gif)
![데모레벨전투](https://user-images.githubusercontent.com/12960463/118310458-9bf8ea00-b529-11eb-8ba0-9f58d192846f.gif)
![보스 Idle](https://user-images.githubusercontent.com/12960463/118364188-60662a80-b5d2-11eb-89d9-93200f5138b2.gif)
![보스 공격 패턴1](https://user-images.githubusercontent.com/12960463/118364184-58a68600-b5d2-11eb-9104-50a2fade0bc2.gif)
![보스 공격 패턴2](https://user-images.githubusercontent.com/12960463/118811397-ec849480-b8e7-11eb-90a1-5224a5e2004c.gif)
![보스 공격 패턴1-2(sm)](https://user-images.githubusercontent.com/12960463/118811404-ee4e5800-b8e7-11eb-9220-d93b082b2527.gif)

