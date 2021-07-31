// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller_Player.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "../Public/Interface_AI.h"
#include "../Public/Interface_General.h"
#include "../Public/Interface_BaseCharacter.h"
#include "PWOGameInstance.h"


AController_Player::AController_Player()
{

}

/// <summary>
/// 팔로우 카메라를 생성해서 viewtarget 대체
/// </summary>
void AController_Player::BeginPlay()
{
	Super::BeginPlay();
	if (IsLocalPlayerController() == false)
		return;
	PlayerCameraManager->SetManualCameraFade(1, FLinearColor::Black, true);
	FTimerHandle timer_handle;
	GetWorld()->GetTimerManager().SetTimer(timer_handle, FTimerDelegate::CreateLambda([&]() {
		PlayerCameraManager->StartCameraFade(1, 0, 2, FLinearColor::Black, true);
		follow_cam = Cast<AFollowCam_Base>(GetWorld()->SpawnActor(AFollowCam_Base::StaticClass()));
		follow_cam->follow_target = GetPawn();
		SetViewTarget(follow_cam);
		}), 2, false);
}

/// <summary>
/// 락온 쿨타임을 매 프레임 감소시킴
/// 매 프레임 카메라 전방으로 linetrace 해서 interactable 객체를 찾아서 등록
/// </summary>
/// <param name="DeltaTime"></param>
void AController_Player::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	lock_on_cooltime -= DeltaTime;

	/* 라인트레이스로 인터랙션 가능 물체 탐색 */
	if (IsLocalPlayerController() && follow_cam->IsValidLowLevel()) {
		FHitResult hit_result;
		FVector trace_start = GetPawn()->GetActorLocation();
		FVector trace_end = trace_start + follow_cam->camera->GetForwardVector()*(100);
		TArray<AActor*> dummy;
		bool is_hit = UKismetSystemLibrary::LineTraceSingle(GetWorld(), trace_start, trace_end, ETraceTypeQuery::TraceTypeQuery6, false, dummy, EDrawDebugTrace::Type::None, hit_result, true);
		if (is_hit && hit_result.GetActor()->ActorHasTag("interactable")) {
			if (interaction_target != hit_result.GetActor()) {
				if (interaction_target != nullptr && interaction_target->IsValidLowLevel()) {
					if (interaction_target->GetClass()->ImplementsInterface(UInterface_General::StaticClass())) {
						IInterface_General::Execute_removeInteractionInfo(interaction_target, GetPawn());
					}
				}
				interaction_target = hit_result.GetActor();
				if (interaction_target->GetClass()->ImplementsInterface(UInterface_General::StaticClass())) {
					IInterface_General::Execute_popInteractionInfo(interaction_target, GetPawn());
				}
			}
		}
		else {
			if (interaction_target != nullptr && interaction_target->IsValidLowLevel()) {
				if (interaction_target->GetClass()->ImplementsInterface(UInterface_General::StaticClass())) {
					IInterface_General::Execute_removeInteractionInfo(interaction_target, GetPawn());
				}
			}
			interaction_target = nullptr;
		}
	}
}


/// <summary>
/// 키매핑
/// </summary>
void AController_Player::SetupInputComponent()
{
	Super::SetupInputComponent();
	if (InputComponent) {
		InputComponent->BindAction("LockOn", IE_Pressed, this, &AController_Player::lockOnEvent);
		InputComponent->BindAxis("ChangeLockOn", this, &AController_Player::changeLockOnAxisEvent);
	}
}

/// <summary>
/// 락온타게팅 실행 ( 액션 매핑 이벤트 )
/// </summary>
void AController_Player::lockOnEvent()
{
	if (follow_cam->IsValidLowLevel() == false)
		return;
	if (follow_cam->is_lock_on) {
		releaseLock_ON();
		return;
	}
	AActor* target = findLockOnTarget();
	if (target != nullptr && target->IsValidLowLevel()) {
		if (target->GetClass()->ImplementsInterface(UInterface_AI::StaticClass())) {
			IInterface_AI::Execute_setStateBarVisibility(target, true);
			IInterface_AI::Execute_setLockOnMarker(target, true);
		}
		follow_cam->is_lock_on = true;
		is_lock_on = true;
		follow_cam->look_target = target;
	}
}


/// <summary>
/// 타게팅 전환 (축매핑 이벤트)
/// changeLockOnTarget 메소드로 타게팅이 전환될 액터를 구하고 이전 타겟의 마커를 화면에서 지우고 새로운 타겟의 마커를 띄움
/// </summary>
/// <param name="__axis_value"></param>
void AController_Player::changeLockOnAxisEvent(float __axis_value)
{
	if (__axis_value == 0 || lock_on_cooltime > 0 || follow_cam->IsValidLowLevel() == false || is_lock_on == false)
		return;
	AActor* target = changeLockOnTarget(__axis_value);
	if (target != nullptr && target->IsValidLowLevel()) {
		if (follow_cam->look_target->GetClass()->ImplementsInterface(UInterface_AI::StaticClass())) {
			IInterface_AI::Execute_setLockOnMarker(follow_cam->look_target, false);
		}
		if (target->GetClass()->ImplementsInterface(UInterface_AI::StaticClass())) {
			IInterface_AI::Execute_setStateBarVisibility(target, true);
			IInterface_AI::Execute_setLockOnMarker(target, true);
		}
		follow_cam->look_target = target;
		lock_on_cooltime = 0.2f;
	}
}

/// <summary>
/// 타게팅 해제
/// </summary>
void AController_Player::releaseLock_ON_Implementation()
{
	if (follow_cam->IsValidLowLevel() == false)
		return;
	follow_cam->is_lock_on = false;
	is_lock_on = false;
	if (follow_cam->look_target != nullptr && follow_cam->look_target->IsValidLowLevel()) {
		if (follow_cam->look_target->GetClass()->ImplementsInterface(UInterface_AI::StaticClass())) {
			IInterface_AI::Execute_setLockOnMarker(follow_cam->look_target, false);
		}
	}
	SetControlRotation(follow_cam->camera->GetComponentRotation());
}



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


/// <summary>
/// 타게팅 대상을 변경
/// __direction이 + 이면 이전 타겟의 우측, -이면 좌측 대상을 출력
/// </summary>
/// <param name="__direction"> 마우스 휠 축 방향 </param>
/// <returns> 다음 타게팅 대상 </returns>
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

/// <summary>
/// stackable한 아이템인지 결과 반환
/// </summary>
/// <param name="__item_type"></param>
/// <returns></returns>
bool AController_Player::isStackable(EItemType __item_type)
{
	return __item_type == EItemType::Consumables || __item_type == EItemType::Raw;
}


/// <summary>
/// 서버에서 applyDamage를 실행
/// </summary>
/// <param name="__damaged_actor"></param>
/// <param name="__damage_id"></param>
/// <param name="__damage_causer"></param>
/// <param name="__hit_bone_name"></param>
void AController_Player::CtoS_applyDamage_Implementation(AActor* __damaged_actor, FName __damage_id, AActor* __damage_causer, FName __hit_bone_name, FVector __hit_location)
{
	Server_ApplyDamage(__damaged_actor, __damage_id, __damage_causer, __hit_bone_name, __hit_location);
}

/// <summary>
/// 클라이언트 소유 액터의 applyDamage를 실행
/// </summary>
/// <param name="__damaged_actor"></param>
/// <param name="__damage_id"></param>
/// <param name="__damage_causer"></param>
/// <param name="__hit_bone_name"></param>
void AController_Player::Server_ApplyDamage_Implementation(AActor* __damaged_actor, FName __damage_id, AActor* __damage_causer, FName __hit_bone_name, FVector __hit_location)
{
	if (__damaged_actor->GetClass()->ImplementsInterface(UInterface_BaseCharacter::StaticClass())) {
		IInterface_BaseCharacter::Execute_applyDamage(__damaged_actor, __damage_id, __damage_causer, __hit_bone_name, __hit_location);
	}
}

/// <summary>
/// 액터가 타게팅 중인 액터인지 반환
/// </summary>
/// <param name="actor"></param>
/// <param name="result"></param>
void AController_Player::isLock_On_Target_Implementation(AActor* actor, bool& result)
{
	result = false;
	if (follow_cam->IsValidLowLevel())
		if (actor == follow_cam->look_target)
			result = true;
}


/// <summary>
/// 현재 타겟팅 중인 적 액터 포인터 반환
/// </summary>
/// <returns></returns>
AActor* AController_Player::getLockOnTargetActor_Implementation()
{
	if (follow_cam->IsValidLowLevel())
		return follow_cam->look_target;
	else
		return nullptr;
}

bool AController_Player::getIsLockOn_Implementation()
{
	return is_lock_on;
}

AFollowCam_Base* AController_Player::getFollowCam_Implementation()
{
	return follow_cam;
}

void AController_Player::showInteractionText_Implementation(const FText& __text)
{

}

void AController_Player::removeInteractionText_Implementation()
{

}

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

/// <summary>
/// 퀵슬롯 데이터로 부터 해당 키에 해당하는 퀵슬롯 위젯의 프로퍼티를 갱신하고 UI를 업데이트
/// </summary>
/// <param name="__key"></param>
void AController_Player::refreshQuickSlot(FKey __key)
{
	if (IsLocalController() == false)
		return;
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

FInventoryData AController_Player::getInventoryData_Implementation(int32 __index)
{
	if(inventory_list.Contains(__index))
		return inventory_list[__index];
	else {
		FInventoryData dummy;
		dummy.item_id = "None";
		return dummy;
	}
}

bool AController_Player::isInteractionTarget_Implementation(AActor* __actor)
{
	return interaction_target == __actor;
}

bool AController_Player::isRegisteredQuickSlotKey_Implementation(FKey __key)
{
	return quickslot_list.Contains(__key);
}

int32 AController_Player::getRegisteredQuickSlotInvenIndex_Implementation(FKey __key)
{
	if (quickslot_list.Contains(__key)) {
		return quickslot_list[__key];
	}
	else {
		return -1;
	}
}

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